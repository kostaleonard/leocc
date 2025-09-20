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
    token_t *token = malloc(sizeof(token_t));
    token->kind = TOKEN_KEYWORD_INT;
    token->data = NULL;
    free_token(token);
}

void test_free_token_frees_literal_int_token() {
    token_t *token = malloc(sizeof(token_t));
    token->kind = TOKEN_LITERAL_INT;
    token->data = malloc(sizeof(literal_int_data_t));
    literal_int_data_t *int_data = (literal_int_data_t *)token->data;
    int_data->val = 2017;
    free_token(token);
}

void test_free_token_frees_identifier_token() {
    token_t *token = malloc(sizeof(token_t));
    token->kind = TOKEN_IDENTIFIER;
    token->data = malloc(sizeof(identifier_data_t));
    identifier_data_t *id_data = (identifier_data_t *)token->data;
    id_data->name = malloc(256);
    strncpy(id_data->name, "my_identifier", 256);
    free_token(token);
}
