#include <stdlib.h>
#include "include/exceptions.h"
#include "include/token.h"

void free_token(token_t *token) {
    if (NULL == token) {
        return;
    }
    switch (token->kind) {
        case TOKEN_KEYWORD_INT:
        case TOKEN_LEFT_PAREN:
        case TOKEN_RIGHT_PAREN:
        case TOKEN_LEFT_BRACE:
        case TOKEN_KEYWORD_RETURN:
        case TOKEN_SEMICOLON:
        case TOKEN_RIGHT_BRACE:
            free(token);
        break;
        case TOKEN_LITERAL_INT:
            free(token->data);
            free(token);
        break;
        case TOKEN_IDENTIFIER:
            identifier_data_t *id_data = (identifier_data_t *)token->data;
            free(id_data->name);
            free(token->data);
            free(token);
        break;
        default:
            Throw(FAILURE_INVALID_INPUT);
    }
}
