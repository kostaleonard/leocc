#ifndef TESTS_INCLUDE_TEST_TOKEN_H_
#define TESTS_INCLUDE_TEST_TOKEN_H_

void test_free_token_does_nothing_on_null_input();

void test_free_token_frees_keyword_token();

void test_free_token_frees_literal_int_token();

void test_free_token_frees_identifier_token();

#endif  // TESTS_INCLUDE_TEST_TOKEN_H_
