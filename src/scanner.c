#include <string.h>
#include "include/scanner.h"
#include "include/token.h"
#include "include/exceptions.h"

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
    char *delim = " \t\n";
    char *saveptr;
    char *token_str = strtok_r(program_text_copy, delim, &saveptr);
    while (NULL != token_str) {
        token_t *token = calloc(1, sizeof(token_t));
        if (0 == strcmp(token_str, "int")) {
            token->kind = TOKEN_KEYWORD_INT;
        } else {
            Throw(FAILURE_NOT_IMPLEMENTED);
        }
        list_append(tokens, token);
        token_str = strtok_r(NULL, delim, &saveptr);
    }
    free(program_text_copy);
    return tokens;
}
