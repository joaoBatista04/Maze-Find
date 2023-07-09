#include "deque.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Deque{
    data_type **blocks;
    DequeFree deque_free;
    int block_size;
    int blocks_allocated;
    int idx_first_block;
    int idx_last_block;
    int idx_first_item;
    int idx_last_item;
};

void _center_blocks_deque(Deque *deque){
    data_type **centered_blocks = (data_type **)calloc(deque->blocks_allocated, sizeof(data_type *));

    int size = deque->idx_last_block - deque->idx_first_block;
    int shift = (deque->blocks_allocated - size) / 2;

    memcpy(&centered_blocks[shift], &deque->blocks[deque->idx_first_block], size * sizeof(data_type *));

    deque->idx_first_block = shift;
    deque->idx_last_block = size + shift;

    free(deque->blocks);
    deque->blocks = centered_blocks;
}

void _push_back_deque(Deque *deque){
    if(deque->idx_first_block >= 5){
        int shift = deque->idx_first_block / 2;

        for(int i = deque->idx_first_block; i < deque->idx_last_block; i++){
            deque->blocks[i - shift] = deque->blocks[i];
            deque->blocks[i] = NULL;
        }

        deque->idx_first_block -= shift;
        deque->idx_last_block -= shift;
    }

    else{
        deque->blocks_allocated *= 2;
        _center_blocks_deque(deque);
    }
}

void _push_front_deque(Deque *deque){
    if(deque->idx_last_block < (deque->blocks_allocated - 5)){
        int shift = (deque->blocks_allocated - deque->idx_last_block) / 2;

        for(int i = deque->idx_last_block; i > deque->idx_first_block; i--){
            deque->blocks[i + shift] = deque->blocks[i];
            deque->blocks[i] = NULL;
        }

        deque->idx_first_block += shift;
        deque->idx_last_block += shift;
    }

    else{
        deque->blocks_allocated *= 2;
        _center_blocks_deque(deque);
    }
}

// criacao do deque
Deque *deque_construct(DequeFree deque_free){
    Deque *deque = (Deque *)calloc(1, sizeof(Deque));

    deque->blocks_allocated = 5;
    deque->block_size = 5;
    deque->idx_first_block = (int)(deque->blocks_allocated / 2);
    deque->idx_last_block = (int)(deque->blocks_allocated / 2);
    deque->idx_first_item = 0;
    deque->idx_last_item = 0;

    deque->blocks = (data_type **)calloc(deque->blocks_allocated, sizeof(data_type *));

    deque->deque_free = deque_free;

    return deque;
}

// funcoes para insercao na direita e esquerda (devem ser feitas em O(1), com eventual realloc)
void deque_push_back(Deque *d, void *val){
    if(d->idx_last_block >= d->blocks_allocated){
        _push_back_deque(d);
    }

    if(d->blocks[d->idx_last_block] == NULL){
        d->blocks[d->idx_last_block] = calloc(d->block_size, sizeof(data_type *));
    }

    d->blocks[d->idx_last_block][d->idx_last_item] = val;

    d->idx_last_item++;

    if(d->idx_last_item >= d->block_size){
        d->idx_last_item = 0;
        d->idx_last_block++;
    }
}

void deque_push_front(Deque *d, void *val){
    d->idx_first_item--;

    if(d->idx_first_item < 0){
        d->idx_first_item = d->block_size - 1;
        d->idx_first_block--;
    }

    if(d->idx_first_block < 0){
        _push_front_deque(d);
    }

    if(d->blocks[d->idx_first_block] == NULL){
        d->blocks[d->idx_first_block] = calloc(d->block_size, sizeof(data_type *));
    }

    d->blocks[d->idx_first_block][d->idx_first_item] = val;
}

// funcoes para remocao na direita e esquerda (devem ser feitas em O(1))
void *deque_pop_back(Deque *d){
    if(d->idx_first_item == d->idx_last_item && d->idx_first_block == d->idx_last_block){
        printf("ERROR: trying to pop an empty Deque\n");
        exit(1);
    }

    data_type val = deque_get(d, deque_size(d) - 1);

    d->idx_last_item--;

    if(d->idx_last_item == 0){
        free(d->blocks[d->idx_last_block]);
        d->blocks[d->idx_last_block] = NULL;
    }

    if(d->idx_last_item < 0){
        d->idx_last_item = d->block_size - 1;
        d->idx_last_block--;
    }

    return val;
}
void *deque_pop_front(Deque *d){
    if(d->idx_first_item == d->idx_last_item && d->idx_first_block == d->idx_last_block){
        printf("ERROR: trying to pop an empty Deque\n");
        exit(1);
    }

    data_type val = deque_get(d, 0);

    d->idx_first_item++;

    if(d->idx_first_item >= d->block_size){
        d->idx_first_item = 0;
        
        free(d->blocks[d->idx_first_block]);

        d->blocks[d->idx_first_block] = NULL;
        d->idx_first_block++;
    }

    return val;
}

// numero de elementos
int deque_size(Deque *d){
    return (d->idx_last_item - d->idx_first_item + (d->block_size * (d->idx_last_block - d->idx_first_block)));
}

// obtem o i-esimo elemento
void *deque_get(Deque *d, int idx){
    if(idx >= deque_size(d)){
        printf("ERROR: invalid index\n");
        return NULL;
    }

    int index = (idx + d->idx_first_item) % d->block_size;
    int block = d->idx_first_block + ((idx + d->idx_first_item) / d->block_size);

    data_type val = d->blocks[block][index];
    
    return val;
}

// libera o espaco alocado para o deque
void deque_destroy(Deque *d){
    for(int i = 0; i < deque_size(d); i++){
        data_type val = deque_pop_back(d);

        if(d->deque_free != NULL){
            d->deque_free(val);
        }
    }

    for(int i = 0; i < d->blocks_allocated; i++){
        free(d->blocks[i]);
    }

    free(d->blocks);
    free(d);
}
