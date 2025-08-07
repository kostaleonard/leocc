#ifndef TESTS_INCLUDE_TEST_LIST_H_
#define TESTS_INCLUDE_TEST_LIST_H_

void test_list_create_returns_list();

void test_list_create_fails_on_invalid_input();

void test_list_destroy_fails_on_invalid_input();

void test_list_is_empty_identifies_empty_list();

void test_list_is_empty_fails_on_invalid_input();

void test_list_length_returns_length();

void test_list_length_fails_on_invalid_input();

void test_list_prepend_adds_node_to_front();

void test_list_prepend_fails_on_invalid_input();

void test_list_append_adds_node_to_back();

void test_list_append_fails_on_invalid_input();

void test_list_remove_head_deletes_first_node();

void test_list_remove_head_fails_on_empty_list();

void test_list_remove_head_fails_on_invalid_input();

void test_list_remove_tail_deletes_last_node();

void test_list_remove_tail_fails_on_empty_list();

void test_list_remove_tail_fails_on_invalid_input();

void test_list_remove_node_deletes_node();

void test_list_remove_node_fails_on_invalid_input();

#endif  // TESTS_INCLUDE_TEST_LIST_H_
