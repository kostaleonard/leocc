#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include <stdlib.h>
#include "include/exceptions.h"
#include "include/list.h"
#include "tests/include/test_list.h"

int compare_ints(int *num1, int *num2) {
    if (NULL == num1 || NULL == num2) {
        return 0;
    }
    return *num1 - *num2;
}

void test_list_create_returns_list() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    assert_true(NULL != list);
    list_destroy(list);
}

void test_list_create_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_create(NULL, (compare_function_t *)compare_ints);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_list_destroy_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_destroy(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_list_clear_removes_nodes() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    for (int idx = 1; idx <= 4; idx++) {
        int *x = malloc(sizeof(int));
        assert_true(NULL != x);
        *x = idx;
        list_append(list, x);
    }
    assert_true(4 == list_length(list));
    list_clear(list);
    assert_true(list_is_empty(list));
    list_destroy(list);
}

void test_list_clear_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_clear(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_list_is_empty_identifies_empty_list() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    assert_true(list_is_empty(list));
    int *x = malloc(sizeof(int));
    assert_true(NULL != x);
    *x = 1;
    list_prepend(list, x);
    assert_true(!list_is_empty(list));
    list_remove_head(list);
    assert_true(list_is_empty(list));
    list_destroy(list);
}

void test_list_is_empty_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_is_empty(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_list_length_returns_length() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    for (int idx = 1; idx <= 4; idx++) {
        int *x = malloc(sizeof(int));
        assert_true(NULL != x);
        *x = idx;
        list_prepend(list, x);
        assert_true(idx == list_length(list));
    }
    for (int idx = 1; idx <= 4; idx++) {
        list_remove_head(list);
        assert_true(4 - idx == list_length(list));
    }
    list_destroy(list);
}

void test_list_length_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_length(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_list_prepend_adds_node_to_front() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    int *x = malloc(sizeof(int));
    assert_true(NULL != x);
    *x = 1;
    list_prepend(list, x);
    assert_true(NULL != list->head);
    assert_true(1 == *(int *)list->head->data);
    assert_true(1 == *(int *)list->head->next->data);
    assert_true(1 == *(int *)list->head->prev->data);
    int *y = malloc(sizeof(int));
    assert_true(NULL != y);
    *y = 2;
    list_prepend(list, y);
    assert_true(NULL != list->head);
    assert_true(2 == *(int *)list->head->data);
    assert_true(1 == *(int *)list->head->next->data);
    assert_true(1 == *(int *)list->head->prev->data);
    assert_true(2 == *(int *)list->head->next->next->data);
    list_destroy(list);
}

void test_list_prepend_fails_on_invalid_input() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    int *x = malloc(sizeof(int));
    assert_true(NULL != x);
    *x = 1;
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_prepend(list, NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
    exception_thrown = false;
    e = SUCCESS;
    Try {
        list_prepend(NULL, x);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
    free(x);
    list_destroy(list);
}

void test_list_append_adds_node_to_back() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    int *x = malloc(sizeof(int));
    assert_true(NULL != x);
    *x = 1;
    list_append(list, x);
    assert_true(NULL != list->head);
    assert_true(1 == *(int *)list->head->data);
    assert_true(1 == *(int *)list->head->next->data);
    assert_true(1 == *(int *)list->head->prev->data);
    int *y = malloc(sizeof(int));
    assert_true(NULL != y);
    *y = 2;
    list_append(list, y);
    assert_true(NULL != list->head);
    assert_true(1 == *(int *)list->head->data);
    assert_true(2 == *(int *)list->head->next->data);
    assert_true(2 == *(int *)list->head->prev->data);
    assert_true(1 == *(int *)list->head->next->next->data);
    int *z = malloc(sizeof(int));
    assert_true(NULL != z);
    *z = 3;
    list_append(list, z);
    assert_true(NULL != list->head);
    assert_true(1 == *(int *)list->head->data);
    assert_true(2 == *(int *)list->head->next->data);
    assert_true(3 == *(int *)list->head->prev->data);
    assert_true(3 == *(int *)list->head->next->next->data);
    list_destroy(list);
}

void test_list_append_fails_on_invalid_input() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    int *x = malloc(sizeof(int));
    assert_true(NULL != x);
    *x = 1;
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_append(list, NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
    exception_thrown = false;
    e = SUCCESS;
    Try {
        list_append(NULL, x);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
    free(x);
    list_destroy(list);
}

void test_list_remove_head_deletes_first_node() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    for (int idx = 1; idx <= 4; idx++) {
        int *x = malloc(sizeof(int));
        assert_true(NULL != x);
        *x = idx;
        list_prepend(list, x);
    }
    assert_true(4 == *(int *)list->head->data);
    assert_true(3 == *(int *)list->head->next->data);
    assert_true(1 == *(int *)list->head->prev->data);
    list_remove_head(list);
    assert_true(3 == *(int *)list->head->data);
    assert_true(2 == *(int *)list->head->next->data);
    assert_true(1 == *(int *)list->head->prev->data);
    list_remove_head(list);
    assert_true(2 == *(int *)list->head->data);
    assert_true(1 == *(int *)list->head->next->data);
    assert_true(1 == *(int *)list->head->prev->data);
    list_remove_head(list);
    assert_true(1 == *(int *)list->head->data);
    assert_true(1 == *(int *)list->head->next->data);
    assert_true(1 == *(int *)list->head->prev->data);
    list_remove_head(list);
    assert_true(NULL == list->head);
    list_destroy(list);
}

void test_list_remove_head_fails_on_empty_list() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_remove_head(list);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_EMPTY_LIST == e);
    list_destroy(list);
}

void test_list_remove_head_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_remove_head(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_list_remove_tail_deletes_last_node() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    for (int idx = 1; idx <= 4; idx++) {
        int *x = malloc(sizeof(int));
        assert_true(NULL != x);
        *x = idx;
        list_prepend(list, x);
    }
    assert_true(4 == *(int *)list->head->data);
    assert_true(3 == *(int *)list->head->next->data);
    assert_true(1 == *(int *)list->head->prev->data);
    list_remove_tail(list);
    assert_true(4 == *(int *)list->head->data);
    assert_true(3 == *(int *)list->head->next->data);
    assert_true(2 == *(int *)list->head->prev->data);
    list_remove_tail(list);
    assert_true(4 == *(int *)list->head->data);
    assert_true(3 == *(int *)list->head->next->data);
    assert_true(3 == *(int *)list->head->prev->data);
    list_remove_tail(list);
    assert_true(4 == *(int *)list->head->data);
    assert_true(4 == *(int *)list->head->next->data);
    assert_true(4 == *(int *)list->head->prev->data);
    list_remove_tail(list);
    assert_true(NULL == list->head);
    list_destroy(list);
}

void test_list_remove_tail_fails_on_empty_list() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_remove_tail(list);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_EMPTY_LIST == e);
    list_destroy(list);
}

void test_list_remove_tail_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_remove_tail(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_list_remove_node_deletes_node() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    for (int idx = 1; idx <= 4; idx++) {
        int *x = malloc(sizeof(int));
        assert_true(NULL != x);
        *x = idx;
        list_append(list, x);
    }
    list_remove_node(list, list->head->next);
    assert_true(1 == *(int *)list->head->data);
    assert_true(3 == *(int *)list->head->next->data);
    assert_true(4 == *(int *)list->head->next->next->data);
    assert_true(1 == *(int *)list->head->next->prev->data);
    list_remove_node(list, list->head);
    assert_true(3 == *(int *)list->head->data);
    assert_true(4 == *(int *)list->head->next->data);
    assert_true(4 == *(int *)list->head->prev->data);
    list_remove_node(list, list->head->prev);
    assert_true(3 == *(int *)list->head->data);
    assert_true(3 == *(int *)list->head->next->data);
    assert_true(3 == *(int *)list->head->prev->data);
    list_remove_node(list, list->head->next);
    assert_true(list_is_empty(list));
    list_destroy(list);
}

void test_list_remove_node_fails_on_invalid_input() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    int *x = malloc(sizeof(int));
    assert_true(NULL != x);
    *x = 1;
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_remove_node(list, NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
    list_append(list, x);
    exception_thrown = false;
    e = SUCCESS;
    Try {
        list_remove_node(NULL, list->head);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
    list_destroy(list);
}

void test_list_remove_at_deletes_node() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    for (int idx = 1; idx <= 10; idx++) {
        int *x = malloc(sizeof(int));
        assert_true(NULL != x);
        *x = idx;
        list_append(list, x);
    }
    list_remove_at(list, 1);
    assert_true(1 == *(int *)list->head->data);
    assert_true(3 == *(int *)list->head->next->data);
    assert_true(4 == *(int *)list->head->next->next->data);
    assert_true(1 == *(int *)list->head->next->prev->data);
    list_remove_at(list, 0);
    assert_true(3 == *(int *)list->head->data);
    assert_true(4 == *(int *)list->head->next->data);
    assert_true(10 == *(int *)list->head->prev->data);
    list_remove_at(list, 2);
    assert_true(3 == *(int *)list->head->data);
    assert_true(4 == *(int *)list->head->next->data);
    assert_true(6 == *(int *)list->head->next->next->data);
    assert_true(4 == *(int *)list->head->next->next->prev->data);
    list_remove_at(list, 6);
    assert_true(3 == *(int *)list->head->data);
    assert_true(9 == *(int *)list->head->prev->data);
    assert_true(3 == *(int *)list->head->prev->next->data);
    list_remove_at(list, -1);
    assert_true(3 == *(int *)list->head->data);
    assert_true(8 == *(int *)list->head->prev->data);
    assert_true(3 == *(int *)list->head->prev->next->data);
    list_remove_at(list, -4);
    assert_true(3 == *(int *)list->head->data);
    assert_true(6 == *(int *)list->head->next->data);
    assert_true(3 == *(int *)list->head->next->prev->data);
    list_remove_at(list, -4);
    assert_true(6 == *(int *)list->head->data);
    assert_true(7 == *(int *)list->head->next->data);
    assert_true(6 == *(int *)list->head->prev->next->data);
    list_remove_at(list, -1);
    list_remove_at(list, -1);
    list_remove_at(list, -1);
    assert_true(list_is_empty(list));
    list_destroy(list);
}

void test_list_remove_at_fails_on_index_out_of_bounds() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_remove_at(list, 0);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INDEX_OUT_OF_BOUNDS == e);
    for (int idx = 1; idx <= 4; idx++) {
        int *x = malloc(sizeof(int));
        assert_true(NULL != x);
        *x = idx;
        list_append(list, x);
    }
    exception_thrown = false;
    e = SUCCESS;
    Try {
        list_remove_at(list, 4);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INDEX_OUT_OF_BOUNDS == e);
    exception_thrown = false;
    e = SUCCESS;
    Try {
        list_remove_at(list, -5);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INDEX_OUT_OF_BOUNDS == e);
    list_clear(list);
    for (int idx = 1; idx <= 1; idx++) {
        int *x = malloc(sizeof(int));
        assert_true(NULL != x);
        *x = idx;
        list_append(list, x);
    }
    exception_thrown = false;
    e = SUCCESS;
    Try {
        list_remove_at(list, 1);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INDEX_OUT_OF_BOUNDS == e);
    list_destroy(list);
}

void test_list_remove_at_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_remove_at(NULL, 0);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_list_find_returns_matching_node() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    for (int idx = 1; idx <= 10; idx++) {
        int *x = malloc(sizeof(int));
        assert_true(NULL != x);
        *x = idx;
        list_append(list, x);
    }
    int *y = malloc(sizeof(int));
    *y = 1;
    node_t *found = list_find(list, y);
    assert_true(found == list->head);
    *y = 9;
    found = list_find(list, y);
    assert_true(found == list->head->prev->prev);
    free(y);
    list_destroy(list);
}

void test_list_find_returns_null_on_no_match() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    for (int idx = 1; idx <= 10; idx++) {
        int *x = malloc(sizeof(int));
        assert_true(NULL != x);
        *x = idx;
        list_append(list, x);
    }
    int *y = malloc(sizeof(int));
    *y = 11;
    node_t *found = list_find(list, y);
    assert_true(NULL == found);
    *y = 1;
    list_clear(list);
    found = list_find(list, y);
    assert_true(NULL == found);
    free(y);
    list_destroy(list);
}

void test_list_find_fails_on_invalid_input() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    int *y = malloc(sizeof(int));
    *y = 1;
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        list_find(NULL, y);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
    exception_thrown = false;
    e = SUCCESS;
    Try {
        list_find(list, NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
    free(y);
    list_destroy(list);
}
