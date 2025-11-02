#include <stdlib.h>
#include "include/exceptions.h"
#include "include/token.h"

void free_token(token_t *token) {
    if (NULL == token) {
        return;
    }
    if (NULL != token->filename) {
        free(token->filename);
    }
    switch (token->kind) {
        case TOK_INT:
        case TOK_LPAREN:
        case TOK_RPAREN:
        case TOK_LBRACE:
        case TOK_RETURN:
        case TOK_SEMICOLON:
        case TOK_RBRACE:
        case TOK_EOF:
            free(token);
        break;
        case TOK_LITERAL_INT:
            free(token->data);
            free(token);
        break;
        case TOK_IDENTIFIER:
            identifier_data_t *id_data = (identifier_data_t *)token->data;
            free(id_data->name);
            free(token->data);
            free(token);
        break;
        default:
            fprintf(
                stderr, "Undefined free behavior for token %d\n", token->kind);
            Throw(FAILURE_INVALID_INPUT);
    }
}
