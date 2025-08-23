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
    list_clear(list);
    free(list);
}

void list_clear(list_t *list) {
    if (NULL == list) {
        Throw(FAILURE_INVALID_INPUT);
    }
    while (!list_is_empty(list)) {
        list_remove_head(list);
    }
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

void list_remove_tail(list_t *list) {
    if (NULL == list) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (list_is_empty(list)) {
        Throw(FAILURE_EMPTY_LIST);
    }
    list->head = list->head->prev;
    list_remove_head(list);
}

void list_remove_node(list_t *list, node_t *node) {
    if (NULL == list || NULL == node) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (node == list->head) {
        list_remove_head(list);
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        list->free_function(node->data);
        free(node);
    }
}

void list_remove_at(list_t *list, int idx) {
    if (NULL == list) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (list_is_empty(list)) {
        Throw(FAILURE_INDEX_OUT_OF_BOUNDS);
    }
    if (0 == idx) {
        list_remove_head(list);
    } else if (-1 == idx) {
        list_remove_tail(list);
    } else if (list->head == list->head->next) {
        // If list is length 1, any other index is out of bounds.
        Throw(FAILURE_INDEX_OUT_OF_BOUNDS);
    } else if (idx > 0) {
        // idx is 1 or higher and list is length >= 2.
        node_t *node = list->head->next;
        for (int i = 1; i < idx; i++) {
            node = node->next;
            if (node == list->head) {
                Throw(FAILURE_INDEX_OUT_OF_BOUNDS);
            }
        }
        list_remove_node(list, node);
    } else {
        // idx is -2 or lower and list is length >= 2.
        node_t *node = list->head->prev->prev;
        for (int i = -2; i > idx; i--) {
            node = node->prev;
            if (node == list->head->prev) {
                Throw(FAILURE_INDEX_OUT_OF_BOUNDS);
            }
        }
        list_remove_node(list, node);
    }
}

node_t *list_find(list_t *list, void *data) {
    if (NULL == list || NULL == data) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (list_is_empty(list)) {
        return NULL;
    }
    bool explored_head = false;
    node_t *current = list->head;
    while (!explored_head || current != list->head) {
        if (0 == list->compare_function(data, current->data)) {
            return current;
        }
        explored_head = true;
        current = current->next;
    }
    return NULL;
}

size_t list_count(list_t *list, void *data) {
    if (NULL == list || NULL == data) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (list_is_empty(list)) {
        return 0;
    }
    size_t matches = 0;
    bool explored_head = false;
    node_t *current = list->head;
    while (!explored_head || current != list->head) {
        if (0 == list->compare_function(data, current->data)) {
            matches++;
        }
        explored_head = true;
        current = current->next;
    }
    return matches;
}

static int sort_compare_function(void *list, const void *pa, const void *pb) {
    void *a = *(void **)pa;
    void *b = *(void **)pb;
    return ((list_t *)list)->compare_function(a, b);
}

void list_sort(list_t *list) {
    if (NULL == list || NULL == list->compare_function) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (list_is_empty(list)) {
        return;
    }
    size_t length = list_length(list);
    void **arr = malloc(length * sizeof(void *));
    if (NULL == arr) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    node_t *current = list->head;
    for (size_t idx = 0; idx < length; idx++) {
        arr[idx] = current->data;
        current = current->next;
    }
    qsort_s(arr, length, sizeof(void *), sort_compare_function, list);
    current = list->head;
    for (size_t idx = 0; idx < length; idx++) {
        current->data = arr[idx];
        current = current->next;
    }
    free(arr);
}

void list_foreach(list_t *list, void (*func)(node_t *)) {
    if (NULL == list || NULL == func) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (list_is_empty(list)) {
        return;
    }
    bool explored_head = false;
    node_t *current = list->head;
    while (!explored_head || current != list->head) {
        func(current);
        explored_head = true;
        current = current->next;
    }
}
