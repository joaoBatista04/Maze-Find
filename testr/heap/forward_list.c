
#include <stdio.h>
#include <stdlib.h>

#include "forward_list.h"

Node *node_construct(data_type value, Node *next) {
    Node *node = calloc(1, sizeof(Node));
    node->value = value;
    node->next = next;

    return node;
}

void node_destroy(Node *n) {

    free(n);
}

ForwardList *forward_list_construct() {
    ForwardList *fl = calloc(1, sizeof(ForwardList));
    fl->head = fl->last = NULL;
    fl->size = 0;

    return fl;
}

int forward_list_size(ForwardList *l) {
    return l->size;
}

data_type forward_list_find(ForwardList *l, void *key, int (*cmp_fn)(data_type data, void *key)) {
    Node *node = l->head;

    while (node) {
        if (cmp_fn(node->value, key) == 0){
            //printf("Found\n");
            return node->value;
        }
        node = node->next;
    }

    return NULL;
}

void forward_list_push_front(ForwardList *l, data_type data) {
    l->head = node_construct(data, l->head);
    l->size++;
}

void forward_list_push_back(ForwardList *l, data_type data) {
    if (!l->head) {
        l->head = l->last = node_construct(data, NULL);
    }
    else {
        l->last = l->last->next = node_construct(data, NULL);
    }
    l->size++;
}

void forward_list_print(ForwardList *l, void (*print_fn)(data_type)) {
    Node* node = l->head;

    while(node) {
        print_fn(node->value);
        node = node->next;
    }
    printf("\n");
}

data_type forward_list_get(ForwardList *l, int i) {
    Node *node = l->head;
    for (int j = 0; j < i && node; j++) {
        node = node->next;
    }
    if (node) return node->value;
    return NULL;
}

data_type forward_list_pop_front(ForwardList *l) {
    Node *node = l->head;
    if (!node) {
        printf("Forward list is empty!");
        exit(1);
    }
    data_type aux = node->value;
    l->head = node->next;
    l->size--;
    node_destroy(node);

    return aux;
}

ForwardList *forward_list_reverse(ForwardList *l);

void forward_list_clear(ForwardList *l);

void forward_list_remove(ForwardList *l, data_type val) {
    Node *prev = NULL;
    Node *curr = l->head;

    while (curr != NULL) {
        if (curr->value == val) {
            if (prev == NULL) {
                l->head = curr->next;
            }
            else {
                prev->next = curr->next;
            }
            node_destroy(curr);
            l->size--;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
}

void forward_list_unique(ForwardList *l);

void forward_list_cat(ForwardList *l, ForwardList *m);

void forward_list_sort(ForwardList *l);

void forward_list_destroy(ForwardList *l) {
    while (l->size) {
        node_destroy(forward_list_pop_front(l));
    }
    free(l);
}