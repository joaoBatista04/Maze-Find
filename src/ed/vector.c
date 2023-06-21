#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

struct Vector{
    data_type *data;
    int size;
    int allocated;
};

// Aloca espaço para um elemento do tipo vector e inicializa os seus atributos.
Vector *vector_construct(){
    Vector *vector = (Vector *)calloc(10, sizeof(Vector));

    vector->size = 0;
    vector->allocated = 10;

    vector->data = (data_type *)calloc(10, sizeof(data_type));

    return vector;
}

// Libera o espaço reservado para o vector.
void vector_destroy(Vector *v){
    free(v->data);
    free(v);
}

// Adiciona o item dado ao final do vector e aumenta o tamanho. Aumenta a capacidade alocada se necessário.
void vector_push_back(Vector *v, data_type val){
    if(v->allocated == v->size){
        v->allocated *= 2;
        v->data = (data_type *)realloc(v->data, v->allocated * sizeof(data_type));
    }

    v->data[v->size] = val;
    v->size++;
}

// Retorna o número de elementos atualmente no vector.
int vector_size(Vector *v){
    return v->size;
}

// Retorna o i-ésimo elemento do vector.
data_type vector_get(Vector *v, int i){
    if(i >= 0 && i <= v->allocated){
        return v->data[i];
    }

    else{
        printf("Error: index not found\n");
        exit(1);
    }
}

// Atualiza o i-ésimo elemento do vector  para que ele passe a ter o valor val. 
void vector_set(Vector *v, int i, data_type val){
    if(i >= 0 && i <= v->allocated){
        v->data[i] = val;
    }

    else{
        printf("Error: index not found\n");
        exit(1);
    }
}

// Retorna o índice da primeira ocorrência do item no Vector ou -1 se o elemento não for encontrado.
int vector_find(Vector *v, data_type val){
    for(int i = 0; i < v->size; i++){
        if(v->data[i] == val){
            return i;
        }
    }

    return -1;
}

data_type vector_remove(Vector *v, int i){
    if(i >= v->size || i < 0){
        printf("Error in vector_remove: invalid index %d for vector\n", i);
        exit(0);
    }

    data_type value = v->data[i];

    for(int j = i; j < v->size - 1; j++){
        v->data[j] = v->data[j + 1];
    }

    v->size--;

    return value;
}

// Remove o primeiro elemento
data_type vector_pop_front(Vector *v){
    return vector_remove(v, 0);
}

// Remove o ultimo elemento
data_type vector_pop_back(Vector *v){
    return vector_remove(v, v->size - 1);
}

// Insere o elemento na i-esima posicao
void vector_insert(Vector *v, int i, data_type val){
    if(i < 0 || i > v->size){
        printf("Error in vector_insert: %d index not allowed", i);
        exit(1);
    }
    
    if(v->size + 1 >= v->allocated){
        v->allocated *= 2;
        v->data = (data_type *)realloc(v->data, v->allocated * sizeof(data_type));
    }

    for(int j = v->size; j > i; j--){
        v->data[j] = v->data[j - 1];
    }

    v->data[i] = val;
    v->size++;
}

// Troca os elementos das posições i e j (i vira j e j vira i)
void vector_swap(Vector *v, int i, int j){
    data_type aux = v->data[i];

    v->data[i] = v->data[j];
    v->data[j] = aux;
}

// Inverte o vetor in-place (sem criar um novo vetor)
void vector_reverse(Vector *v){
    for(int i = 0, j = v->size - 1; i < v->size / 2; i++, j--){
        vector_swap(v, j, i);
    }
}

// Ordena o vetor in-place (sem criar um novo vetor)
void vector_sort(Vector *v){
    data_type aux;

    for(int i = 0; i < v->size - 1; i++) {
      for(int j = 0; j < v->size - 1; j++) {
         if(v->data[j] > v->data[j+1]) {
            aux = v->data[j];
            v->data[j] = v->data[j+1];
            v->data[j+1] = aux;
         }
      }
   }
}

// Retorna o indice de val usando busca binaria. Retorna -1 se nao encontrado.
int vector_binary_search(Vector *v, data_type val){
    int begin = 0;
    int end = v->size - 1;

    while (begin <= end) {
        int middle = (begin + end) / 2;

        if (v->data[middle] == val) {
            return middle;
        } else if (v->data[middle] < val) {
            begin = middle + 1;
        } else {
            end = middle  - 1;
        }
    }

    return -1;
}