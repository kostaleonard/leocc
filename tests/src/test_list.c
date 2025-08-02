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

void test_list_prepend_adds_node() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    assert_true(NULL != list);
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
    assert_true(NULL != list);
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
    list_destroy(list);
}

void test_list_remove_head_deletes_first_node() {
    list_t *list = list_create(free, (compare_function_t *)compare_ints);
    assert_true(NULL != list);
    for (int i = 1; i <= 4; i++) {
        int *x = malloc(sizeof(int));
        assert_true(NULL != x);
        *x = i;
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
