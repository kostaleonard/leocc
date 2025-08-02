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
