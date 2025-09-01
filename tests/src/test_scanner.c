#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "include/exceptions.h"
#include "include/list.h"
#include "include/scanner.h"
#include "include/token.h"
#include "tests/include/test_scanner.h"

void test_scan_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        scan(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_scan_tokenizes_one_keyword() {
    list_t *tokens = scan("int");
    assert_true(NULL != tokens);
    assert_true(1 == list_length(tokens));
    token_t *token = (token_t *)tokens->head->data;
    assert_true(TOKEN_KEYWORD_INT == token->kind);
    assert_true(NULL == token->data);
    list_destroy(tokens);
}

void test_scan_tokenizes_several_keywords() {
    list_t *tokens = scan("int int int");
    assert_true(NULL != tokens);
    assert_true(3 == list_length(tokens));
    node_t *node = tokens->head;
    do {
        token_t *token = (token_t *)node->data;
        assert_true(TOKEN_KEYWORD_INT == token->kind);
        assert_true(NULL == token->data);
        node = node->next;
    } while (node != tokens->head);
    list_destroy(tokens);
}

void test_scan_tokenizes_skips_whitespace() {
    list_t *tokens = scan("int      int\t int\n\nint\n\tint\n\t\t");
    assert_true(NULL != tokens);
    assert_true(5 == list_length(tokens));
    node_t *node = tokens->head;
    do {
        token_t *token = (token_t *)node->data;
        assert_true(TOKEN_KEYWORD_INT == token->kind);
        assert_true(NULL == token->data);
        node = node->next;
    } while (node != tokens->head);
    list_destroy(tokens);
}

void test_scan_tokenizes_simple_program() {
    list_t *tokens = scan("int main() {\n\treturn 2017;\n}\n");
    assert_true(NULL != tokens);
    assert_true(9 == list_length(tokens));
    node_t *node = tokens->head;
    // int
    token_t *token = (token_t *)node->data;
    assert_true(TOKEN_KEYWORD_INT == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // main
    token = (token_t *)node->data;
    assert_true(TOKEN_IDENTIFIER == token->kind);
    assert_true(NULL != token->data);
    identifier_data_t *id_data = (identifier_data_t *)token->data;
    assert_true(NULL != id_data->name);
    assert_true(0 == strcmp(id_data->name, "main"));
    node = node->next;
    // (
    token = (token_t *)node->data;
    assert_true(TOKEN_LEFT_PAREN == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // )
    token = (token_t *)node->data;
    assert_true(TOKEN_RIGHT_PAREN == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // {
    token = (token_t *)node->data;
    assert_true(TOKEN_LEFT_BRACE == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // return
    token = (token_t *)node->data;
    assert_true(TOKEN_KEYWORD_RETURN == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // 2017
    token = (token_t *)node->data;
    assert_true(TOKEN_LITERAL_INT == token->kind);
    assert_true(NULL != token->data);
    literal_int_data_t *int_data = (literal_int_data_t *)token->data;
    assert_true(2017 == int_data->val);
    node = node->next;
    // ;
    token = (token_t *)node->data;
    assert_true(TOKEN_SEMICOLON == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // }
    token = (token_t *)node->data;
    assert_true(TOKEN_RIGHT_BRACE == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    assert_true(node == tokens->head);
    list_destroy(tokens);
}
