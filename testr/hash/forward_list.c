
#include <stdio.h>
#include <stdlib.h>

#include "forward_list.h"

Node *node_construct(data_type value, Node *next){
    Node *node = (Node *)malloc(sizeof(Node));

    node->next = next;
    node->value = value;

    return node;
}

void node_destroy(Node *n){
    free(n);
}

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
    ForwardList *l = (ForwardList *)calloc(1, sizeof(ForwardList));

    l->head = NULL;
    l->last = NULL;
    l->size = 0;

    return l;
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
data_type forward_list_find(ForwardList *l, void *key, int (*cmp_fn)(data_type data, void *key)){
    Node *n = l->head;

    while (n != NULL){
        if (cmp_fn(n->value, key)){
            return n->value;
        }

        n = n->next;
    }

    return NULL;
}

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
    l->head = node_construct(data, l->head);
    l->size++;

    if (l->size == 1){
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

    if (l->last == NULL){
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
    Node *n = l->head;

    printf("[");

    while (n != NULL){
        print_fn(n->value);
        n = n->next;

        if (n != NULL){
            printf(", ");
        }
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
    if (i < 0 || i >= l->size){
        exit(printf("Error: forward_list_get(): index out of bounds."));
    }

    Node *n = l->head;
    for (int j = 0; j < i; j++){
        n = n->next;
    }

    return n->value;
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
    if (l->head == NULL){
        exit(printf("Error: forward_list_pop_front(): list is empty."));
    }

    Node *n = l->head;
    l->head = l->head->next;
    data_type data = n->value;
    node_destroy(n);
    l->size--;

    if (l->size <= 1){
        l->last = l->head;
    }

    return data;
}

/**
 * @brief Create a new list given by the reverse of the given list.
 * @param l
 * Pointer to the linked list.
 * @return ForwardList*
 * Pointer to the newly allocated linked list.
 */
ForwardList *forward_list_reverse(ForwardList *l){
    ForwardList *new_list = forward_list_construct();

    Node *n = l->head;
    while (n != NULL){
        forward_list_push_front(new_list, n->value);
        n = n->next;
    }

    return new_list;
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
    // we cannot use l->size in the for loop
    // because the size will change as we remove items
    int n_itens = l->size;

    for (int i = 0; i < n_itens; i++){
        forward_list_pop_front(l);
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
void forward_list_remove(ForwardList *l, data_type val){
    Node *n = l->head;
    Node *prev = NULL;
    Node *new_n = NULL;

    while (n != NULL){
        if (n->value == val){
            if (prev == NULL){
                l->head = new_n = n->next;
            }

            else{
                prev->next = new_n = n->next;
            }

            node_destroy(n);
            n = new_n;
            l->size--;
        }

        else{
            prev = n;
            n = n->next;
        }
    }
}

/**
 * @brief Removes all duplicate values from the linked list.
 * Removes all duplicate values from the linked list and frees the memory allocated for them.
 * @param l
 * Pointer to the linked list.
 * @note
 * The linked list must be sorted.
 */
void forward_list_unique(ForwardList *l){
    if (l->head == NULL){
        exit(printf("Error: forward_list_unique(): list is empty!\n"));
    }

    Node* current = l->head;

    while(current != NULL){
        Node* runner = current;

        while(runner->next != NULL){
            if(runner->next->value == current->value){
                Node* duplicate = runner->next;
                runner->next = runner->next->next;
                node_destroy(duplicate);
            } 
            
            else{
                runner = runner->next;
            }
        }

        current = current->next;
    }
}

/**
 * @brief Adds all nodes from the given list to the end of the linked list.
 * @param l
 * Pointer to the linked list.
 * @param m
 * Pointer to the linked list to be added to the end of the linked list.
 */
void forward_list_cat(ForwardList *l, ForwardList *m){
    Node *n = m->head;

    while (n != NULL){
        forward_list_push_front(l, n->value);
        n = n->next;
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
    Node *n = l->head;

    while (n != NULL){
        Node *next = n->next;
        node_destroy(n);
        n = next;
    }

    free(l);
}