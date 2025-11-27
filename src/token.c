#include <stdlib.h>
#include "include/exceptions.h"
#include "include/token.h"

token_t *token_create(token_kind_t kind, source_loc_t loc, token_data_t data) {
    token_t *token = calloc(1, sizeof(token_t));
    if (NULL == token) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    token->kind = kind;
    token->loc = loc;
    token->data = data;
    return token;
}

void token_destroy(token_t *token) {
    if (NULL == token) {
        return;
    }
    if (NULL != token->loc.filename) {
        free(token->loc.filename);
    }
    switch (token->kind) {
        case TOK_IDENTIFIER:
            free(token->data.ident);
            break;
        default:
            break;
    }
    free(token);
}
