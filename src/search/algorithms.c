
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"
#include "../ed/heap.h"
#include "../ed/queue.h"
#include "../ed/stack.h"
#include "../ed/deque.h"
#include "../ed/hash.h"

ResultData _default_result()
{
    ResultData result;

    result.caminho = NULL;
    result.custo_caminho = 0;
    result.nos_expandidos = 0;
    result.tamanho_caminho = 0;
    result.sucesso = 0;

    return result;
}

double _cell_distance(Celula *c1, Celula *c2) {
    return sqrt(pow(c1->x - c2->x, 2) + pow(c1->y - c2->y, 2));
}

ResultData _process_path(ResultData result, Celula *c) {
    while (c) {
        result.caminho[result.tamanho_caminho] = *c;
        result.tamanho_caminho++;

        Celula *prev = c->prev;

        if (prev) {
            result.custo_caminho += _cell_distance(c, prev);
        }
        c = prev;
    }

    // Fixing path (reversing)
    for (int i = 0; i < result.tamanho_caminho / 2; i++) {
        Celula aux = result.caminho[i];
        result.caminho[i] = result.caminho[result.tamanho_caminho - i - 1];
        result.caminho[result.tamanho_caminho - i - 1] = aux;
    }

    return result;
}

Celula *celula_create(int x, int y, Celula* prev)
{
    Celula *c = malloc(sizeof(Celula));
    c->x = x;
    c->y = y;
    c->prev = prev;
    c->g = c->h = 0;
    return c;
}

void celula_destroy(Celula *c)
{
    free(c);
}

void free_fn (void *cel) {
    celula_destroy(cel);
}

int celula_hash(HashTable *h, void *key)
{
    Celula *c = (Celula *)key;
    // 83 e 97 sao primos e o operador "^" é o XOR bit a bit
    return ((c->x * 83) ^ (c->y * 97)) % hash_table_size(h);
}

int celula_cmp(void *c1, void *c2)
{
    Celula *a = (Celula *)c1;
    Celula *b = (Celula *)c2;

    if (a->x == b->x && a->y == b->y)
        return 0;
    else
        return 1;
}

int celula_hash_cmp(void *c1, void *c2) {
    HashTableItem *d1 = (HashTableItem *)c1;
    Celula *a = (Celula *)d1->key;
    Celula *b = (Celula *)c2;
    
    if (a->x == b->x && a->y == b->y) {
        return 0;
    }
    else
        return 1;
}

int directions[8][2] = {{0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}};

ResultData a_star(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData result = _default_result();
    int max_length = labirinto_n_linhas(l) * labirinto_n_colunas(l);

    result.caminho = calloc(max_length, sizeof(Celula));

    HashTable *ht = hash_table_construct(101, celula_hash, celula_hash_cmp, free, free);
    Heap *heap = heap_construct(ht);

    Celula *curr = celula_create(inicio.x, inicio.y, NULL);
    curr->g = 0;
    curr->h = _cell_distance(curr, &fim);

    heap_push(heap, curr, curr->g + curr->h);

    Deque *deque = deque_construct(free);

    while (!heap_empty(heap)) {
        curr = heap_pop(heap);
        labirinto_atribuir(l, curr->y, curr->x, EXPANDIDO);
        result.nos_expandidos++;

        deque_push_back(deque, curr);

        if (celula_cmp(curr, &fim) == 0) {
            result.sucesso = 1;
            result = _process_path(result, curr);
            break;
        }

        for (int i = 0; i < 8; i++) {
            int x = curr->x + directions[i][0];
            int y = curr->y + directions[i][1];

            if (x >= 0 && y >= 0 && x < labirinto_n_colunas(l) && y < labirinto_n_linhas(l)) {
                TipoCelula tipo = labirinto_obter(l, y, x);

                if (tipo != EXPANDIDO && tipo != OCUPADO) {
                    labirinto_atribuir(l, y, x, FRONTEIRA);
                    
                    Celula *cel = celula_create(x, y, curr);
                    cel->g = curr->g + _cell_distance(curr, cel);
                    cel->h = _cell_distance(cel, &fim);

                    cel = heap_push(heap, cel, cel->g + cel->h);

                    if (cel) {
                        celula_destroy(cel);
                    }
                }
            }
        }
    }

    if (result.sucesso == 0) {
        free(result.caminho);
        result.caminho = NULL;
    }

    heap_destroy(heap);
    hash_table_destroy(ht);
    deque_destroy(deque);

    return result;
}

ResultData breadth_first_search(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData result = _default_result();
    int max_length = labirinto_n_linhas(l) * labirinto_n_colunas(l);

    result.caminho = calloc(max_length, sizeof(Celula));

    Queue *queue = queue_construct(free_fn);
    queue_push(queue, celula_create(inicio.x, inicio.y, NULL));

    Deque *deque = deque_construct(free_fn);

    while (!queue_empty(queue)) {
        Celula *curr = queue_pop(queue);
        labirinto_atribuir(l, curr->y, curr->x, EXPANDIDO);
        result.nos_expandidos++;

        deque_push_back(deque, curr);

        if (celula_cmp(curr, &fim) == 0) {
            result.sucesso = 1;
            result = _process_path(result, curr);
            break;
        }

        for (int i = 0; i < 8; i++) {
            int x = curr->x + directions[i][0];
            int y = curr->y + directions[i][1];

            if (x >= 0 && y >= 0 && x < labirinto_n_colunas(l) && y < labirinto_n_linhas(l)) {
                TipoCelula cel = labirinto_obter(l, y, x);

                if (cel == LIVRE || cel == FIM) {
                    queue_push(queue, celula_create(x, y, curr));
                    labirinto_atribuir(l, y, x, FRONTEIRA);
                }
            }
        }
    }

    if (result.sucesso == 0) {
        free(result.caminho);
        result.caminho = NULL;
    }

    queue_destroy(queue);
    deque_destroy(deque);

    return result;
}

ResultData depth_first_search(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData result = _default_result();
    int max_length = labirinto_n_linhas(l) * labirinto_n_colunas(l);

    result.caminho = calloc(max_length, sizeof(Celula));

    Stack *stack = stack_construct(free_fn);
    stack_push(stack, celula_create(inicio.x, inicio.y, NULL));

    Deque *deque = deque_construct(free_fn);

    while (!stack_empty(stack)) {
        Celula *curr = stack_pop(stack);
        labirinto_atribuir(l, curr->y, curr->x, EXPANDIDO);
        result.nos_expandidos++;

        deque_push_back(deque, curr);

        if (celula_cmp(curr, &fim) == 0) {
            result.sucesso = 1;
            result = _process_path(result, curr);
            break;
        }

        for (int i = 0; i < 8; i++) {
            int x = curr->x + directions[i][0];
            int y = curr->y + directions[i][1];

            if (x >= 0 && y >= 0 && x < labirinto_n_colunas(l) && y < labirinto_n_linhas(l)) {
                TipoCelula cel = labirinto_obter(l, y, x);

                if (cel == LIVRE || cel == FIM) {
                    stack_push(stack, celula_create(x, y, curr));
                    labirinto_atribuir(l, y, x, FRONTEIRA);
                }
            }
        }
    }

    if (result.sucesso == 0) {
        free(result.caminho);
        result.caminho = NULL;
    }

    stack_destroy(stack);
    deque_destroy(deque);

    return result;
}

ResultData dummy_search(Labirinto *l, Celula inicio, Celula fim)
{
    int max_path_length = 0;
    float dx, dy;

    ResultData result = _default_result();

    max_path_length = abs(fim.x - inicio.x) + abs(fim.y - inicio.y);
    result.caminho = (Celula *)malloc(sizeof(Celula) * max_path_length);
    result.sucesso = 1;

    Celula atual = inicio;
    result.caminho[result.tamanho_caminho++] = atual;
    result.nos_expandidos++;

    while ((atual.x != fim.x) || (atual.y != fim.y))
    {
        dx = fim.x - atual.x;
        dy = fim.y - atual.y;

        if (dx != 0)
            dx /= fabs(dx);

        if (dy != 0)
            dy /= fabs(dy);

        atual.x += (int)dx;
        atual.y += (int)dy;

        if (labirinto_obter(l, atual.y, atual.x) == OCUPADO || (atual.x > labirinto_n_colunas(l) - 1) || (atual.y > labirinto_n_linhas(l) - 1) || (atual.x < 0) || (atual.y < 0))
        {
            result.sucesso = 0;
            free(result.caminho);
            result.caminho = NULL;
            return result;
        }

        result.caminho[result.tamanho_caminho++] = atual;
        result.nos_expandidos++;
        result.custo_caminho += sqrt(pow(dx, 2) + pow(dy, 2));
    }

    return result;
}