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
#include "include/token.h"
#include "tests/include/test_token.h"

void test_free_token_does_nothing_on_null_input() {
    free_token(NULL);
}

void test_free_token_frees_keyword_token() {
    token_t *token = calloc(1, sizeof(token_t));
    token->kind = TOK_INT;
    free_token(token);
}

void test_free_token_frees_literal_int_token() {
    token_t *token = calloc(1, sizeof(token_t));
    token->kind = TOK_INT_LITERAL;
    token->data.int_value = 2017;
    free_token(token);
}

void test_free_token_frees_identifier_token() {
    token_t *token = calloc(1, sizeof(token_t));
    token->kind = TOK_IDENTIFIER;
    token->data.ident = malloc(256);
    strncpy(token->data.ident, "my_identifier", 256);
    free_token(token);
}
