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
        cmocka_unit_test(test_list_clear_removes_nodes),
        cmocka_unit_test(test_list_clear_fails_on_invalid_input),
        cmocka_unit_test(test_list_is_empty_identifies_empty_list),
        cmocka_unit_test(test_list_is_empty_fails_on_invalid_input),
        cmocka_unit_test(test_list_length_returns_length),
        cmocka_unit_test(test_list_length_fails_on_invalid_input),
        cmocka_unit_test(test_list_prepend_adds_node_to_front),
        cmocka_unit_test(test_list_prepend_fails_on_invalid_input),
        cmocka_unit_test(test_list_append_adds_node_to_back),
        cmocka_unit_test(test_list_append_fails_on_invalid_input),
        cmocka_unit_test(test_list_remove_head_deletes_first_node),
        cmocka_unit_test(test_list_remove_head_fails_on_empty_list),
        cmocka_unit_test(test_list_remove_head_fails_on_invalid_input),
        cmocka_unit_test(test_list_remove_tail_deletes_last_node),
        cmocka_unit_test(test_list_remove_tail_fails_on_empty_list),
        cmocka_unit_test(test_list_remove_tail_fails_on_invalid_input),
        cmocka_unit_test(test_list_remove_node_deletes_node),
        cmocka_unit_test(test_list_remove_node_fails_on_invalid_input),
        cmocka_unit_test(test_list_remove_at_deletes_node),
        cmocka_unit_test(test_list_remove_at_fails_on_index_out_of_bounds),
        cmocka_unit_test(test_list_remove_at_fails_on_invalid_input),
        cmocka_unit_test(test_list_find_returns_matching_node),
        cmocka_unit_test(test_list_find_returns_null_on_no_match),
        cmocka_unit_test(test_list_find_fails_on_invalid_input),
        cmocka_unit_test(test_list_count_returns_num_matches),
        cmocka_unit_test(test_list_count_returns_zero_on_no_match),
        cmocka_unit_test(test_list_count_fails_on_invalid_input),
        cmocka_unit_test(test_list_sort_arranges_data_low_to_high),
        cmocka_unit_test(test_list_sort_sorts_randomly_generated_values),
        cmocka_unit_test(test_list_sort_does_nothing_on_empty_list),
        cmocka_unit_test(test_list_sort_fails_if_compare_function_is_null),
        cmocka_unit_test(test_list_sort_fails_on_invalid_input),
        cmocka_unit_test(test_list_foreach_applies_function),
        cmocka_unit_test(test_list_foreach_fails_on_invalid_input),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
