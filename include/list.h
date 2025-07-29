/**
 * @brief A circularly doubly linked list data structure.
 * 
 */

#ifndef INCLUDE_LIST_H_
#define INCLUDE_LIST_H_

typedef struct node_t {
    void *data;
    struct node_t *next;
} node_t;

typedef struct list_t {
    node_t *head;
} list_t;

#endif  // INCLUDE_LIST_H_
