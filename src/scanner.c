#include <ctype.h>
#include <string.h>
#include "include/scanner.h"
#include "include/token.h"
#include "include/exceptions.h"

/**
 * @brief Returns the numeric token and updates idx to the next unconsumed char.
 */
static token_t *scan_num(char *text, size_t *idx) {
    // TODO ignoring floats for now.
    char *endptr;
    int num = strtol(&text[*idx], &endptr, 10);
    token_t *token = calloc(1, sizeof(token_t));
    if (NULL == token) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    token->kind = TOKEN_LITERAL_INT;
    token->data = calloc(1, sizeof(literal_int_data_t));
    if (NULL == token->data) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    literal_int_data_t *int_data = (literal_int_data_t *)token->data;
    int_data->val = num;
    *idx += (endptr - (text + *idx));
    return token;
}

/**
 * @brief Returns the identifier token and updates idx to the next unconsumed
 * char.
 */
static token_t *scan_identifier(char *text, size_t *idx) {
    // TODO ignoring underscores and other valid identifier chars for now.
    size_t start = *idx;
    size_t end = start;
    while (isalnum(text[end])) {
        end++;
    }
    token_t *token = calloc(1, sizeof(token_t));
    if (NULL == token) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    token->kind = TOKEN_IDENTIFIER;
    token->data = calloc(1, sizeof(identifier_data_t));
    if (NULL == token->data) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    identifier_data_t *id_data = (identifier_data_t *)token->data;
    id_data->name = calloc(end - start + 1, sizeof(char));
    if (NULL == id_data->name) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    strncpy(id_data->name, text + start, end - start);
    *idx += end - start;
    return token;
}

list_t *scan(char *program_text) {
    if (NULL == program_text) {
        Throw(FAILURE_INVALID_INPUT);
    }
    size_t copy_buffer_len = strlen(program_text) + 1;
    char *program_text_copy = malloc(copy_buffer_len);
    if (NULL == program_text_copy) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    memcpy(program_text_copy, program_text, copy_buffer_len);
    list_t *tokens = list_create((free_function_t *)free_token, NULL);
    size_t idx = 0;
    while ('\0' != program_text_copy[idx]) {
        char c = program_text_copy[idx];
        if (isspace(c)) {
            idx++;
        } else if (c == '(') {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_LEFT_PAREN;
            list_append(tokens, token);
            idx++;
        } else if (c == ')') {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_RIGHT_PAREN;
            list_append(tokens, token);
            idx++;
        } else if (c == '{') {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_LEFT_BRACE;
            list_append(tokens, token);
            idx++;
        } else if (c == '}') {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_RIGHT_BRACE;
            list_append(tokens, token);
            idx++;
        } else if (c == ';') {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_SEMICOLON;
            list_append(tokens, token);
            idx++;
        } else if (0 == strncmp(program_text_copy + idx, "int", strlen("int"))) {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_KEYWORD_INT;
            list_append(tokens, token);
            idx += strlen("int");
        } else if (0 == strncmp(program_text_copy + idx, "return", strlen("return"))) {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_KEYWORD_RETURN;
            list_append(tokens, token);
            idx += strlen("return");
        } else if (isdigit(c)) {
            token_t *token = scan_num(program_text_copy, &idx);
            list_append(tokens, token);
        } else if (isalpha(c)) {
            token_t *token = scan_identifier(program_text_copy, &idx);
            list_append(tokens, token);
        } else {
            Throw(FAILURE_INVALID_CHARACTER);
        }
    }
    free(program_text_copy);
    return tokens;
}
