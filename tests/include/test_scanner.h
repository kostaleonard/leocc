#ifndef TESTS_INCLUDE_TEST_SCANNER_H_
#define TESTS_INCLUDE_TEST_SCANNER_H_

void test_scanner_all_fails_on_invalid_input();

void test_scanner_all_tokenizes_one_keyword();

void test_scanner_all_tokenizes_several_keywords();

void test_scanner_all_skips_whitespace();

void test_scanner_all_tokenizes_simple_program();

void test_scanner_next_skips_comment();

void test_scanner_next_skips_block_comment();

#endif  // TESTS_INCLUDE_TEST_SCANNER_H_
