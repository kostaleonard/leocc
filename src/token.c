#include <stdlib.h>
#include <string.h>
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

source_loc_t source_loc_dup(source_loc_t loc) {
    source_loc_t copy = loc;
    if (NULL != loc.filename) {
        copy.filename = strdup(loc.filename);
        if (NULL == copy.filename) {
            Throw(FAILURE_COULD_NOT_MALLOC);
        }
    }
    return copy;
}
