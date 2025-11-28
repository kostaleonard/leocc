#include <stdlib.h>
#include "include/preprocessor.h"
#include "include/scanner.h"
#include "include/exceptions.h"

preprocessor_t *preprocessor_create(scanner_t *scanner) {
    if (NULL == scanner) {
        Throw(FAILURE_INVALID_INPUT);
    }
    preprocessor_t *pp = calloc(1, sizeof(preprocessor_t));
    if (NULL == pp) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    pp->scanner = scanner;
    return pp;
}

void preprocessor_destroy(preprocessor_t *pp) {
    if (NULL == pp) {
        Throw(FAILURE_INVALID_INPUT);
    }
    scanner_destroy(pp->scanner);
    free(pp);
}

token_t *preprocessor_next(preprocessor_t *pp) {
    if (NULL == pp) {
        Throw(FAILURE_INVALID_INPUT);
    }
    return scanner_next(pp->scanner);
}
