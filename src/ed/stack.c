#include "queue.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Stack{
    Vector *vector;
    int size;
} Stack;

Stack *stack_construct(){
    Stack *stack = (Stack *)malloc(sizeof(Stack));

    stack->size = 0;
    stack->vector = vector_construct();

    return stack;
}

void stack_push(Stack *stack, void *data){
    vector_push_back(stack->vector, data);
    stack->size++;
}

bool stack_empty(Stack *stack){
    return stack->size == 0;
}

void *stack_pop(Stack *stack){
    data_type val;
    val = vector_pop_back(stack->vector);
    stack->size--;

    return val;
}

void stack_destroy(Stack *stack){
    vector_destroy(stack->vector);

    free(stack);
}