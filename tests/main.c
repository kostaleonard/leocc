/**
 * @brief Runs the unit test suite.
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include "tests/include/test_fib.h"
#include "tests/include/test_list.h"

int main(int argc, char **argv) {
    const struct CMUnitTest tests[] = {
        // test_fib.h
        cmocka_unit_test(test_fib_returns_fibonacci_number),
        cmocka_unit_test(test_fib_throws_error_on_negative_input),
        // test_list.c
        cmocka_unit_test(test_list_create_returns_list),
        cmocka_unit_test(test_list_create_fails_on_invalid_input),
        cmocka_unit_test(test_list_destroy_fails_on_invalid_input),
        cmocka_unit_test(test_list_is_empty_identifies_empty_list),
        cmocka_unit_test(test_list_is_empty_fails_on_invalid_input),
        cmocka_unit_test(test_list_length_returns_length),
        cmocka_unit_test(test_list_length_fails_on_invalid_input),
        cmocka_unit_test(test_list_prepend_adds_node_to_front),
        cmocka_unit_test(test_list_prepend_fails_on_invalid_input),
        cmocka_unit_test(test_list_remove_head_deletes_first_node),
        cmocka_unit_test(test_list_remove_head_fails_on_invalid_input),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
