#include <stdlib.h>
#include "include/exceptions.h"
#include "include/token.h"

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
