#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "heap.h"

typedef struct
{
    int x, y;
    float g, h;
} Celula;

Celula *celula_create(int x, int y)
{
    Celula *c = malloc(sizeof(Celula));
    c->x = x;
    c->y = y;
    return c;
}

void celula_destroy(Celula *c)
{
    free(c);
}

void hash_key_destroy(void *a) {
    Celula *c = (Celula *)a;
    celula_destroy(c);
}

int celula_hash(HashTable *h, void *key)
{
    Celula *c = (Celula *)key;
    // 83 e 97 sao primos e o operador "^" é o XOR bit a bit
    return ((c->x * 83) ^ (c->y * 97)) % hash_table_size(h);
}

int celula_cmp(void *c1, void *c2)
{
    HashTableItem *d1 = (HashTableItem *)c1;
    Celula *a = (Celula *)d1->key;
    Celula *b = (Celula *)c2;
    
    if (a->x == b->x && a->y == b->y) {
        return 0;
    }
    else
        return 1;
}

int main()
{
    int i, n, x, y, priority;
    char cmd[10];

    HashTable *h = hash_table_construct(19, celula_hash, celula_cmp, free, free);
    Heap *heap = heap_construct(h);

    scanf("%d", &n);

    for (i = 0; i < n; i++)
    {
        scanf("\n%s", cmd);

        if (!strcmp(cmd, "PUSH"))
        {
            scanf("%d %d %d", &x, &y, &priority);
            Celula *cel = celula_create(x, y);
            cel = heap_push(heap, cel, priority);

            // se a celula ja existia, lembre-se liberar a memoria alocada para a nova celula
            if (cel)
                celula_destroy(cel);
        }
        else if (!strcmp(cmd, "POP"))
        {
            int priority = heap_min_priority(heap);
            Celula *cel = heap_pop(heap);
            printf("%d %d %d\n", cel->x, cel->y, priority);
            celula_destroy(cel);
        }
        //printf("%d\n", i);
    }

    hash_table_destroy(h);
    heap_destroy(heap);

    return 0;
}