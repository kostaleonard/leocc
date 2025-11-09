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

static void print_token_node(node_t *node) {
    if (NULL == node) {
        return;
    }
    printf("%d ", ((token_t *)node->data)->kind);
}

void token_list_print(list_t *list) {
    if (NULL == list) {
        return;
    }
    printf("[");
    list_foreach(list, print_token_node);
    printf("]\n");
}

void test_scanner_all_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        scanner_all(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_scanner_all_tokenizes_one_keyword() {
    scanner_t *scanner = scanner_create_from_text("int");
    list_t *tokens = scanner_all(scanner);
    assert_true(NULL != tokens);
    assert_true(2 == list_length(tokens));
    token_t *token = (token_t *)tokens->head->data;
    assert_true(TOK_INT == token->kind);
    assert_true(NULL == token->data);
    token = (token_t *)tokens->head->next->data;
    assert_true(TOK_EOF == token->kind);
    assert_true(NULL == token->data);
    list_destroy(tokens);
    scanner_destroy(scanner);
}

void test_scanner_all_tokenizes_several_keywords() {
    scanner_t *scanner = scanner_create_from_text("int int int");
    list_t *tokens = scanner_all(scanner);
    assert_true(NULL != tokens);
    assert_true(4 == list_length(tokens));
    size_t idx = 0;
    for (node_t *node = tokens->head; idx < 3; node = node->next, idx++) {
        token_t *token = (token_t *)node->data;
        assert_true(TOK_INT == token->kind);
        assert_true(NULL == token->data);
        node = node->next;
    }
    list_destroy(tokens);
    scanner_destroy(scanner);
}

void test_scanner_all_skips_whitespace() {
    scanner_t *scanner = scanner_create_from_text(
        "int      int\t int\n\nint\n\t\rint\n\t\t");
    list_t *tokens = scanner_all(scanner);
    assert_true(NULL != tokens);
    assert_true(6 == list_length(tokens));
    size_t idx = 0;
    for (node_t *node = tokens->head; idx < 5; node = node->next, idx++) {
        token_t *token = (token_t *)node->data;
        assert_true(TOK_INT == token->kind);
        assert_true(NULL == token->data);
        node = node->next;
    }
    list_destroy(tokens);
    scanner_destroy(scanner);
}

void test_scanner_all_tokenizes_simple_program() {
    scanner_t *scanner = scanner_create_from_text(
        "int main() {\n\treturn 2017;\n}\n");
    list_t *tokens = scanner_all(scanner);
    assert_true(NULL != tokens);
    assert_true(10 == list_length(tokens));
    node_t *node = tokens->head;
    // int
    token_t *token = (token_t *)node->data;
    assert_true(TOK_INT == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // main
    token = (token_t *)node->data;
    assert_true(TOK_IDENTIFIER == token->kind);
    assert_true(NULL != token->data);
    identifier_data_t *id_data = (identifier_data_t *)token->data;
    assert_true(NULL != id_data->name);
    assert_true(0 == strcmp(id_data->name, "main"));
    node = node->next;
    // (
    token = (token_t *)node->data;
    assert_true(TOK_LPAREN == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // )
    token = (token_t *)node->data;
    assert_true(TOK_RPAREN == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // {
    token = (token_t *)node->data;
    assert_true(TOK_LBRACE == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // return
    token = (token_t *)node->data;
    assert_true(TOK_RETURN == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // 2017
    token = (token_t *)node->data;
    assert_true(TOK_INT_LITERAL == token->kind);
    assert_true(NULL != token->data);
    literal_int_data_t *int_data = (literal_int_data_t *)token->data;
    assert_true(2017 == int_data->val);
    node = node->next;
    // ;
    token = (token_t *)node->data;
    assert_true(TOK_SEMICOLON == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // }
    token = (token_t *)node->data;
    assert_true(TOK_RBRACE == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    // EOF
    token = (token_t *)node->data;
    assert_true(TOK_EOF == token->kind);
    assert_true(NULL == token->data);
    node = node->next;
    assert_true(node == tokens->head);
    list_destroy(tokens);
    scanner_destroy(scanner);
}
