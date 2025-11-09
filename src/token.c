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
        case TOK_INT_LITERAL:
            free(token);
        break;
        case TOK_IDENTIFIER:
            free(token->data.ident);
            free(token);
        break;
        default:
            fprintf(
                stderr, "Undefined free behavior for token %d\n", token->kind);
            Throw(FAILURE_INVALID_INPUT);
    }
}
