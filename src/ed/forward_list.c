
#include <stdio.h>
#include <stdlib.h>

#include "forward_list.h"

/**
 * @brief Construct a new Linked List:: Linked List object
 *  Allocates memory for a new linked list and returns a pointer to it.
 * @return ForwardList*
 * Pointer to the newly allocated linked list.
 * @note
 * The caller is responsible for freeing the memory allocated for the linked list using forward_list_destroy().
 *
 */
ForwardList *forward_list_construct(){
    ForwardList *fw = (ForwardList *)malloc(sizeof(ForwardList));

    fw->size = 0;
    fw->head = NULL;
    fw->last = NULL;

    return fw;
}

/**
 * @brief Returns the size of the linked list.
 *  Returns the number of nodes in the linked list.
 * @param l
 * Pointer to the linked list.
 * @return int
 * Number of nodes in the linked list.
 *
 */
int forward_list_size(ForwardList *l){
    return l->size;
}

/**
 * @brief Find an elemento in the linked list.
 * @param l
 * Pointer to the linked list.
 * @param void *key
 * Search key
 * @param int (*cmp_fn)(data_type data, void *key)
 * Pointer to the function to compare data values with the key.
 * @return data_type
 */
data_type forward_list_find(ForwardList *l, void *key, int (*cmp_fn)(data_type data, void *key));

/**
 * @brief Pushes a new node to the front of the linked list.
 *  Allocates memory for a new node and inserts it at the front of the linked list.
 * @param l
 * Pointer to the linked list.
 * @param data
 * Pointer to the data to be stored in the new node.
 *
 */
void forward_list_push_front(ForwardList *l, data_type data){
    Node *n = node_construct(data, l->head);
    l->head = n;
    l->size++;

    if(l->size == 1){
        l->last = l->head;
    }
}

/**
 * @brief Pushes a new node to the back of the linked list.
 *
 * @param l
 * Pointer to the linked list.
 * @param data
 * data to be stored in the new node.
 */
void forward_list_push_back(ForwardList *l, data_type data){
    Node *new_node = node_construct(data, NULL);

    if(l->last == NULL){
        l->head = l->last = new_node;
    }

    else{
        l->last = l->last->next = new_node;
    }

    l->size++;
}

/**
 * @brief Print the elements of the linked list.
 *  Print the elements of the linked list.
 * @param l
 * Pointer to the linked list.
 * @param print_fn
 * Pointer to the function to print data_type values.
 *
 */
void forward_list_print(ForwardList *l, void (*print_fn)(data_type)){
    Node *aux = l->head;

    printf("[");
    while(aux != NULL){
        print_fn(aux->value);
        
        if(aux->next != NULL){
            printf(", ");
        }
        aux = aux->next;
    }
    printf("]");
}

/**
 * @brief Returns the data stored in the node at the given index.
 * @param l
 * Pointer to the linked list.
 * @param i
 * Index of the node.
 * @return data_type
 * Data stored in the node at the given index.
 *
 */
data_type forward_list_get(ForwardList *l, int i){
    if(i < 0 || i > l->size){
        printf("Error: invalid index.\n");
        exit(1);
    }

    else{
        Node *aux = l->head;

        for(int idx = 0; idx < i; idx++){
            aux = aux->next;
        }

        return aux->value;
    }
}

/**
 * @brief Remove the first node of the linked list and returns its data.
 * @param l
 * Pointer to the linked list.
 * @return data_type
 * Pointer to the data stored in the first node of the linked list that was removed.
 *
 */
data_type forward_list_pop_front(ForwardList *l){
    if(l->head == NULL){
        exit(printf("Error: list is empty"));
    }

    Node *aux = l->head;
    data_type value;


    l->head = aux->next;

    value = aux->value;
    node_destroy(aux);

    l->size--;

    if(l->size <= 1){
        l->last = l->head;
    }

    return value;
}

/**
 * @brief Create a new list given by the reverse of the given list.
 * @param l
 * Pointer to the linked list.
 * @return ForwardList*
 * Pointer to the newly allocated linked list.
 */
ForwardList *forward_list_reverse(ForwardList *l){
    ForwardList *fw = forward_list_construct();
    Node *aux = l->head;

    for(int i = 0; i < l->size; i++){
        forward_list_push_front(fw, aux->value);
        aux = aux->next;
    }
    
    forward_list_destroy(l);

    return fw;
}

/**
 * @brief Removes all nodes from the linked list.
 * Removes all nodes from the linked list and frees the memory allocated for them.
 * @param l
 * Pointer to the linked list.
 * @note
 * The caller is responsible for freeing the memory allocated for the data stored in the nodes.
 */
void forward_list_clear(ForwardList *l){
    Node *current = l->head;
    Node *next;

    while(current != NULL){
        next = current->next;
        node_destroy(current);
        current = next;
        l->size--;
    }
}

/**
 * @brief Removes all nodes with the given value from the linked list.
 * Removes all nodes with the given value from the linked list and frees the memory allocated for them.
 * @param l
 * Pointer to the linked list.
 * @param val
 * Value to be removed from the linked list.
 */
void forward_list_remove(ForwardList *l, data_type val);

/**
 * @brief Removes all duplicate values from the linked list.
 * Removes all duplicate values from the linked list and frees the memory allocated for them.
 * @param l
 * Pointer to the linked list.
 * @note
 * The linked list must be sorted.
 */
void forward_list_unique(ForwardList *l);

/**
 * @brief Adds all nodes from the given list to the end of the linked list.
 * @param l
 * Pointer to the linked list.
 * @param m
 * Pointer to the linked list to be added to the end of the linked list.
 */
void forward_list_cat(ForwardList *l, ForwardList *m){
    Node *current = m->head;

    while(current != NULL){
        forward_list_push_front(l, current->value);
        current = current->next;
    }
}

/**
 * @brief Sorts the linked list.
 * Sorts the linked list.
 * @param l
 * Pointer to the linked list.
 */
void forward_list_sort(ForwardList *l);

/**
 * @brief Destroys the linked list.
 *  Frees the memory allocated for the linked list and all its nodes.
 * @param l
 * Pointer to the linked list.
 *
 */
void forward_list_destroy(ForwardList *l){
    Node *current = l->head;
    Node *next;

    while(current != NULL){
        next = current->next;
        node_destroy(current);
        current = next;
    }

    free(l);
}