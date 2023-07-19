#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "forward_list.h"

struct HashTable
{
    ForwardList **buckets;
    HashFunction hash_fn;
    CmpFunction cmp_fn;
    HashTableFree free_key;
    HashTableFree free_val;
    int table_size;
    int n_elements;
};

HashTableItem *_hash_pair_get(HashTable *h, void *key){
    int key_val = h->hash_fn(h, key);

    ForwardList *bucket = h->buckets[key_val];

    if(bucket != NULL){
        HashTableItem *item = forward_list_find(bucket, key, h->cmp_fn);

        if(item != NULL){
            return item;
        }
    }

    return NULL;
}

HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn, HashTableFree free_key, HashTableFree free_val){
    HashTable *hash_tbl = calloc(1, sizeof(HashTable));

    hash_tbl->table_size = table_size;
    hash_tbl->hash_fn = hash_fn;
    hash_tbl->cmp_fn = cmp_fn;
    hash_tbl->free_key = free_key;
    hash_tbl->free_val = free_val;
    hash_tbl->buckets = calloc(table_size, sizeof(ForwardList *));
    hash_tbl->n_elements = 0;

    return hash_tbl;
}

// funcao para insercao/atualizacao de pares chave-valor em O(1).
// Se a chave ja existir, atualiza o valor e retorna o valor antigo para permitir desalocacao.
void *hash_table_set(HashTable *h, void *key, void *val){
    int key_val = h->hash_fn(h, key);
    HashTableItem *item;

    if(h->buckets[key_val] != NULL){
        item = forward_list_find(h->buckets[key_val], key, h->cmp_fn);

        if(item != NULL){
            void *aux = item->val;
            item->val = val;

            return aux;
        }
    }

    else{
        h->buckets[key_val] = forward_list_construct();
    }

    item = (HashTableItem *)calloc(1, sizeof(HashTableItem));
    item->key = key;
    item->val = val;

    forward_list_push_front(h->buckets[key_val], item);
    h->n_elements++;

    return NULL;
}

// retorna o valor associado com a chave key ou NULL se ela nao existir em O(1).
void *hash_table_get(HashTable *h, void *key){
    HashTableItem *item = _hash_pair_get(h, key);

    if(item != NULL){
        return item->val;
    }

    return NULL;
}

// remove o par chave-valor e retorna o valor ou NULL se nao existir tal chave em O(1).
void *hash_table_pop(HashTable *h, void *key){
    int key_val = h->hash_fn(h, key);
    HashTableItem *item = _hash_pair_get(h, key);
    void *val = NULL;

    if(item != NULL){
        val =  item->val;
    }

    forward_list_remove(h->buckets[key_val], val);
    h->n_elements--;

    free(item);

    return val;
}

// numero de buckets
int hash_table_size(HashTable *h){
    return h->table_size;
}

// numero de elementos inseridos
int hash_table_num_elems(HashTable *h){
    return h->n_elements;
}

// libera o espaco alocado para a tabela hash
void hash_table_destroy(HashTable *h){
    for(int i = 0; i < h->table_size; i++){
        if(h->buckets[i] != NULL){
            while(forward_list_size(h->buckets[i])){
                HashTableItem *data = (HashTableItem *)forward_list_pop_front(h->buckets[i]);

                if(h->free_val){
                    h->free_val(data->val);
                }

                if(h->free_key){
                    h->free_key(data->key);
                }

                free(data);
            }
        }

        free(h->buckets[i]);
    }

    free(h->buckets);
    free(h);
}