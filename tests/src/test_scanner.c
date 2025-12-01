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
    token = (token_t *)tokens->head->next->data;
    assert_true(TOK_EOF == token->kind);
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
    node = node->next;
    // main
    token = (token_t *)node->data;
    assert_true(TOK_IDENTIFIER == token->kind);
    assert_true(NULL != token->data.ident);
    assert_true(0 == strcmp(token->data.ident, "main"));
    node = node->next;
    // (
    token = (token_t *)node->data;
    assert_true(TOK_LPAREN == token->kind);
    node = node->next;
    // )
    token = (token_t *)node->data;
    assert_true(TOK_RPAREN == token->kind);
    node = node->next;
    // {
    token = (token_t *)node->data;
    assert_true(TOK_LBRACE == token->kind);
    node = node->next;
    // return
    token = (token_t *)node->data;
    assert_true(TOK_RETURN == token->kind);
    node = node->next;
    // 2017
    token = (token_t *)node->data;
    assert_true(TOK_INT_LITERAL == token->kind);
    assert_true(2017 == token->data.int_value);
    node = node->next;
    // ;
    token = (token_t *)node->data;
    assert_true(TOK_SEMICOLON == token->kind);
    node = node->next;
    // }
    token = (token_t *)node->data;
    assert_true(TOK_RBRACE == token->kind);
    node = node->next;
    // EOF
    token = (token_t *)node->data;
    assert_true(TOK_EOF == token->kind);
    node = node->next;
    assert_true(node == tokens->head);
    list_destroy(tokens);
    scanner_destroy(scanner);
}

void test_scanner_next_skips_comment() {
    scanner_t *scanner = scanner_create_from_text(
        "int main() {\n"
        "\t//Ignore this comment\n"
        "\treturn 2017; // Ignore this comment\n"
        "}//Ignore this comment"
    );
    token_t *token = scanner_next(scanner); // int
    token_destroy(token);
    token = scanner_next(scanner); // main
    token_destroy(token);
    token = scanner_next(scanner); // (
    token_destroy(token);
    token = scanner_next(scanner); // )
    token_destroy(token);
    token = scanner_next(scanner); // {
    token_destroy(token);
    token = scanner_next(scanner); // return
    assert_true(TOK_RETURN == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // 2017
    token_destroy(token);
    token = scanner_next(scanner); // ;
    token_destroy(token);
    token = scanner_next(scanner); // }
    assert_true(TOK_RBRACE == token->kind);
    token_destroy(token);
    // Keep asking for tokens--should always get EOF.
    for (size_t idx = 0; idx < 5; idx++) {
        token = scanner_next(scanner); // EOF
        assert_true(TOK_EOF == token->kind);
        token_destroy(token);
    }
    scanner_destroy(scanner);
}

void test_scanner_next_skips_block_comment() {
    scanner_t *scanner = scanner_create_from_text(
        "/**Ignore this comment.*/\n"
        "int main/*Ignore this comment*/ /*and this one*/() {\n"
        "\treturn/*Ignore this comment*/2017;\n"
        "}/*Ignore this comment*/"
    );
    token_t *token = scanner_next(scanner); // int
    assert_true(TOK_INT == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // main
    token_destroy(token);
    token = scanner_next(scanner); // (
    assert_true(TOK_LPAREN == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // )
    token_destroy(token);
    token = scanner_next(scanner); // {
    token_destroy(token);
    token = scanner_next(scanner); // return
    token_destroy(token);
    token = scanner_next(scanner); // 2017
    assert_true(TOK_INT_LITERAL == token->kind);
    assert_true(2017 == token->data.int_value);
    token_destroy(token);
    token = scanner_next(scanner); // ;
    token_destroy(token);
    token = scanner_next(scanner); // }
    assert_true(TOK_RBRACE == token->kind);
    token_destroy(token);
    // Keep asking for tokens--should always get EOF.
    for (size_t idx = 0; idx < 5; idx++) {
        token = scanner_next(scanner); // EOF
        assert_true(TOK_EOF == token->kind);
        token_destroy(token);
    }
    scanner_destroy(scanner);
}

void test_scanner_next_tokenizes_assignment() {
    scanner_t *scanner = scanner_create_from_text(
        "int main() {\n"
        "    int x = 2017;\n"
        "    return x;\n"
        "}\n"
    );
    token_t *token = scanner_next(scanner); // int
    token_destroy(token);
    token = scanner_next(scanner); // main
    token_destroy(token);
    token = scanner_next(scanner); // (
    token_destroy(token);
    token = scanner_next(scanner); // )
    token_destroy(token);
    token = scanner_next(scanner); // {
    token_destroy(token);
    token = scanner_next(scanner); // int
    assert_true(TOK_INT == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // x
    assert_true(TOK_IDENTIFIER == token->kind);
    assert_true(0 == strcmp(token->data.ident, "x"));
    token_destroy(token);
    token = scanner_next(scanner); // =
    assert_true(TOK_EQ == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // 2017
    assert_true(TOK_INT_LITERAL == token->kind);
    assert_true(2017 == token->data.int_value);
    token_destroy(token);
    token = scanner_next(scanner); // ;
    token_destroy(token);
    scanner_destroy(scanner);
}

void test_scanner_next_tokenizes_keywords() {
    scanner_t *scanner = scanner_create_from_text(
        "int main() {\n"
        "    int intx = 2017;\n"
        "    int int_y;\n"
        "    int;\n"
        "    return returnx;\n"
        "}\n"
    );
    token_t *token = scanner_next(scanner); // int
    token_destroy(token);
    token = scanner_next(scanner); // main
    token_destroy(token);
    token = scanner_next(scanner); // (
    token_destroy(token);
    token = scanner_next(scanner); // )
    token_destroy(token);
    token = scanner_next(scanner); // {
    token_destroy(token);
    token = scanner_next(scanner); // int
    assert_true(TOK_INT == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // intx
    assert_true(TOK_IDENTIFIER == token->kind);
    assert_true(0 == strcmp(token->data.ident, "intx"));
    token_destroy(token);
    token = scanner_next(scanner); // =
    assert_true(TOK_EQ == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // 2017
    assert_true(TOK_INT_LITERAL == token->kind);
    assert_true(2017 == token->data.int_value);
    token_destroy(token);
    token = scanner_next(scanner); // ;
    token_destroy(token);
    token = scanner_next(scanner); // int
    assert_true(TOK_INT == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // int_y
    assert_true(TOK_IDENTIFIER == token->kind);
    assert_true(0 == strcmp(token->data.ident, "intx"));
    token_destroy(token);
    token = scanner_next(scanner); // ;
    assert_true(TOK_SEMICOLON == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // int
    assert_true(TOK_INT == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // ;
    assert_true(TOK_SEMICOLON == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // return
    assert_true(TOK_RETURN == token->kind);
    token_destroy(token);
    token = scanner_next(scanner); // returnx
    assert_true(TOK_IDENTIFIER == token->kind);
    assert_true(0 == strcmp(token->data.ident, "returnx"));
    token_destroy(token);
    token = scanner_next(scanner); // ;
    assert_true(TOK_SEMICOLON == token->kind);
    token_destroy(token);
    scanner_destroy(scanner);
}
