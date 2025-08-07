#include <stdbool.h>
#include "include/exceptions.h"
#include "include/list.h"

list_t *list_create(
    free_function_t *free_function, compare_function_t *compare_function) {
    if (NULL == free_function) {
        Throw(FAILURE_INVALID_INPUT);
    }
    list_t *list = malloc(sizeof(list_t));
    if (NULL == list) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    list->free_function = free_function;
    list->compare_function = compare_function;
    list->head = NULL;
    return list;
}

void list_destroy(list_t *list) {
    if (NULL == list) {
        Throw(FAILURE_INVALID_INPUT);
    }
    while (!list_is_empty(list)) {
        list_remove_head(list);
    }
    free(list);
}

bool list_is_empty(list_t *list) {
    if (NULL == list) {
        Throw(FAILURE_INVALID_INPUT);
    }
    return NULL == list->head;
}

size_t list_length(list_t *list) {
    if (NULL == list) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (list_is_empty(list)) {
        return 0;
    }
    size_t length = 1;
    node_t *current = list->head->next;
    while (current != list->head) {
        length++;
        current = current->next;
    }
    return length;
}

void list_prepend(list_t *list, void *data) {
    if (NULL == list || NULL == data) {
        Throw(FAILURE_INVALID_INPUT);
    }
    node_t *node = malloc(sizeof(node_t));
    if (NULL == node) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    node->data = data;
    if (list_is_empty(list)) {
        node->next = node;
        node->prev = node;
        list->head = node;
    } else {
        node->next = list->head;
        node->prev = list->head->prev;
        node->prev->next = node;
        node->next->prev = node;
        list->head = node;
    }
}

void list_append(list_t *list, void *data) {
    if (NULL == list || NULL == data) {
        Throw(FAILURE_INVALID_INPUT);
    }
    list_prepend(list, data);
    list->head = list->head->next;
}

void list_remove_head(list_t *list) {
    if (NULL == list) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (list_is_empty(list)) {
        Throw(FAILURE_EMPTY_LIST);
    }
    // Check for length 1, but don't use list_length because it is O(N).
    if (list->head == list->head->next) {
        list->free_function(list->head->data);
        free(list->head);
        list->head = NULL;
    } else {
        node_t *old_head = list->head;
        node_t *new_head = list->head->next;
        node_t *tail = list->head->prev;
        tail->next = new_head;
        new_head->prev = tail;
        list->head = new_head;
        list->free_function(old_head->data);
        free(old_head);
    }
}
