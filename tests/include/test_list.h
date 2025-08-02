#ifndef TESTS_INCLUDE_TEST_LIST_H_
#define TESTS_INCLUDE_TEST_LIST_H_

void test_list_create_returns_list();

void test_list_create_fails_on_invalid_input();

void test_list_destroy_fails_on_invalid_input();

void test_list_prepend_adds_node();

void test_list_prepend_fails_on_invalid_input();

void test_list_remove_head_deletes_first_node();

void test_list_remove_head_fails_on_invalid_input();

#endif  // TESTS_INCLUDE_TEST_LIST_H_
