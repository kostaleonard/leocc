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
    // TODO list_remove_head until empty
    free(list);
}
