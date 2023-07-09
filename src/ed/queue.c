
#include "queue.h"
#include "forward_list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Queue{
    ForwardList *forward_list;
    int size;
} Queue;

Queue *queue_construct(){
    Queue *queue = (Queue *)malloc(sizeof(Queue));

    queue->forward_list = forward_list_construct();
    queue->size = 0;

    return queue;
}

void queue_push(Queue *queue, void *data){
    forward_list_push_back(queue->forward_list, data);
    queue->size++;
}

bool queue_empty(Queue *queue){
    return queue->size == 0;
}

void *queue_pop(Queue *queue){
    void *val;

    val = forward_list_pop_front(queue->forward_list);
    queue->size--;

    return val;
}

void queue_destroy(Queue *queue){
    forward_list_destroy(queue->forward_list);

    free(queue);
}