/**
 * @brief Tests string_builder.c.
 */

#ifndef TESTS_INCLUDE_TEST_STRING_BUILDER_H_
#define TESTS_INCLUDE_TEST_STRING_BUILDER_H_

void test_string_builder_create_returns_empty_string_builder();

void test_string_builder_append_adds_to_string();

void test_string_builder_append_expands_capacity();

void test_string_builder_append_fails_on_invalid_input();

void test_string_builder_destroy_fails_on_invalid_input();

#endif  // TESTS_INCLUDE_TEST_STRING_BUILDER_H_
