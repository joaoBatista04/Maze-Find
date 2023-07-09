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

HashTableItem *_hash_pair_construct(void *key, void *val)
{
    HashTableItem *p = calloc(1, sizeof(HashTableItem));
    p->key = key;
    p->val = val;
    return p;
}

HashTableItem *_hash_pair_get(HashTable *h, void *key){
    int key_val = h->hash_fn(h, key);

    if(!h->buckets[key_val]){
        return NULL;
    }

    HashTableItem *item = NULL;
    Node *node = h->buckets[key_val]->head;

    while(node){
        HashTableItem *aux = node->value;

        if(!h->cmp_fn(key, aux->key)){
            item = aux;
            break;
        }

        node = node->next;
    }

    return item;
}

void _hash_pair_destroy(HashTableItem *p)
{
    free(p);
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
void hash_table_set(HashTable *h, void *key, void *val){
    int key_val = h->hash_fn(h, key);
    void *value;
    
    if(h->buckets[key_val] == NULL){
        h->buckets[key_val] = forward_list_construct();
    }

    HashTableItem *item = _hash_pair_get(h, key);

    if(item == NULL){
        forward_list_push_front(h->buckets[key_val], _hash_pair_construct(key, val));
        h->n_elements++;
        h->buckets[key_val]->size++;
    }

    else{
        value = item->val;
        
        if(h->free_key != NULL){
            h->free_key(key);
        }

        if(h->free_val != NULL){
            h->free_val(value);
        }

        item->val = val;
    }
}

// retorna o valor associado com a chave key ou NULL se ela nao existir em O(1).
void *hash_table_get(HashTable *h, void *key){
    HashTableItem *item = _hash_pair_get(h, key);

    if(item == NULL){
        return NULL;
    }

    if(h->free_key != NULL){
        h->free_key(key);
    }

    return item->val;
}

// remove o par chave-valor e retorna o valor ou NULL se nao existir tal chave em O(1).
void *hash_table_pop(HashTable *h, void *key){
    int key_val = h->hash_fn(h, key);
    HashTableItem *item;

    if(h->buckets[key_val] == NULL){
        return NULL;
    }

    Node *curr = h->buckets[key_val]->head;
    Node *prev = NULL;

    while(curr){
        item = curr->value;

        if(!h->cmp_fn(key, item->key)){
            if(prev == NULL){
                h->buckets[key_val]->head = curr->next;
            }

            else{
                prev->next = curr->next;
            }

            if(h->free_key != NULL){
                h->free_key(item->key);
            }

            if(h->free_val != NULL){
                h->free_val(item->val);
            }

            free(curr);
            free(item);
            h->buckets[key_val]->size--;
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    return item->val;
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
    for (int i = 0; i < h->table_size; i++)
    {
        if (h->buckets[i] != NULL)
        {
            Node *n = h->buckets[i]->head;

            while (n != NULL)
            {
                HashTableItem *pair = n->value;
                
                if(h->free_key != NULL){
                    h->free_key(pair->key);
                }

                if(h->free_val != NULL){
                    h->free_val(pair->val);
                }

                free(pair);

                n = n->next;
            }

            forward_list_destroy(h->buckets[i]);
        }
    }

    free(h->buckets);
    free(h);
}