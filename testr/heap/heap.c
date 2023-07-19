#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "heap.h"

typedef struct{
    double priority;
    void *data;
} HeapNode;

struct Heap{
    int capacity;
    int size;
    HeapNode *nodes;
    HashTable *hash_tbl;
};

int _heap_parent_index(int idx){
    return ((idx - 1) / 2);
}

int _heap_left_child(int idx){
    return ((idx * 2) + 1);
}

int _heap_right_child(int idx){
    return ((idx * 2) + 2);
}

void _heapify_up(Heap *heap, int idx){
    int pos = idx;
    int parentPos = _heap_parent_index(idx);
    HeapNode aux = heap->nodes[pos];

    while(pos > 0 && aux.priority < heap->nodes[parentPos].priority){
        heap->nodes[pos] = heap->nodes[parentPos];

        int *new_idx = (int *)calloc(1, sizeof(int));
        *new_idx = pos;
        void *tbl_idx = hash_table_set(heap->hash_tbl, heap->nodes[pos].data, new_idx);

        if(tbl_idx){
            free(tbl_idx);
        }

        pos = parentPos;
        parentPos = _heap_parent_index(pos);
    }

    heap->nodes[pos] = aux;

    int *new_idx = (int *)calloc(1, sizeof(int));
    *new_idx = pos;
    void *tbl_idx = hash_table_set(heap->hash_tbl, heap->nodes[pos].data, new_idx);

    if(tbl_idx){
        free(tbl_idx);
    }
}

void _heapify_down(Heap *heap){
    int pos = 0;
    int index = 0;
    HeapNode aux = heap->nodes[pos];

    while (pos < (heap->size / 2)){
        int left = _heap_left_child(pos);
        int right = _heap_right_child(pos);

        index = left;

        if(right < heap->size){
            if(heap->nodes[left].priority > heap->nodes[right].priority){
                index = right;
            }
        }

        if(aux.priority <= heap->nodes[index].priority){
            break;
        }

        heap->nodes[pos] = heap->nodes[index];

        int *new_idx = (int *)calloc(1, sizeof(int));
        *new_idx = pos;
        void *tbl_idx = hash_table_set(heap->hash_tbl, heap->nodes[pos].data, new_idx);

        if(tbl_idx){
            free(tbl_idx);
        }

        pos = index;
    }

    heap->nodes[pos] = aux;

    int *new_idx = (int *)calloc(1, sizeof(int));
    *new_idx = pos;
    void *tbl_idx = hash_table_set(heap->hash_tbl, heap->nodes[pos].data, new_idx);

    if(tbl_idx){
        free(tbl_idx);
    }
}

Heap *heap_construct(HashTable *h){
    Heap *heap = (Heap *)calloc(1, sizeof(Heap));

    heap->capacity = 100;
    heap->size = 0;

    heap->nodes = (HeapNode *)calloc(heap->capacity, sizeof(HeapNode));
    heap->hash_tbl = h;

    return heap;
}

void* heap_push(Heap *heap, void *data, double priority){
    int *index = (int *)hash_table_get(heap->hash_tbl, data);

    if (index) {
        if (priority < heap->nodes[*index].priority) {
            void* aux = heap->nodes[*index].data;
            heap->nodes[*index].priority = priority;
            heap->nodes[*index].data = data;

            int *old_val = hash_table_pop(heap->hash_tbl, aux);

            void *tmp_idx = hash_table_set(heap->hash_tbl, data, old_val);
            if (tmp_idx) {
                free(tmp_idx);
            }

            _heapify_up(heap, *index);
            data = aux;
        }
        return data;
    }

    if(heap->size >= heap->capacity){
        heap->capacity *= 2;
        heap->nodes = (HeapNode *)realloc(heap->nodes, heap->capacity * sizeof(HeapNode));
    }

    heap->nodes[heap->size].data = data;
    heap->nodes[heap->size].priority = priority;

    int *new_idx = calloc(1, sizeof(int));
    *new_idx = heap->size;
    void *tmp_idx = hash_table_set(heap->hash_tbl, heap->nodes[*new_idx].data, new_idx);
    if (tmp_idx) {
        free(tmp_idx);
    }

    _heapify_up(heap, heap->size);
    heap->size++;

    return NULL;
}

bool heap_empty(Heap *heap){
    return heap->size == 0;
}

void *heap_max(Heap *heap){
    return heap->nodes[0].data;
}

double heap_min_priority(Heap *heap){
    return heap->nodes[0].priority;
}

void *heap_pop(Heap *heap){
    if(heap->size <= 0){
        printf("ERROR: trying to pop an empty heap\n");
        return NULL;
    }

    void *data = heap->nodes[0].data;

    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;

    void *aux = hash_table_pop(heap->hash_tbl, data);

    if(aux){
        free(aux);
    }

    _heapify_down(heap);

    return data;
}

void heap_destroy(Heap *heap){
    for(int i = 0; (i < heap->size) && heap->nodes[i].data; i++){
        free(heap->nodes[i].data);
    }

    free(heap->nodes);
    free(heap);
}