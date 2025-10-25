#include <stdlib.h>
#include "include/preprocessor.h"
#include "include/scanner.h"
#include "include/exceptions.h"

preprocessor_t *preprocessor_create(scanner_t *scanner) {
    if (NULL == scanner) {
        Throw(FAILURE_INVALID_INPUT);
    }
    preprocessor_t *preprocessor = calloc(1, sizeof(preprocessor_t));
    preprocessor->scanner = scanner;
    return preprocessor;
}

void preprocessor_destroy(preprocessor_t *preprocessor) {
    if (NULL == preprocessor) {
        Throw(FAILURE_INVALID_INPUT);
    }
    scanner_destroy(preprocessor->scanner);
    free(preprocessor);
}

token_t *preprocessor_next(preprocessor_t *preprocessor) {
    if (NULL == preprocessor) {
        Throw(FAILURE_INVALID_INPUT);
    }
    // TODO right now we are not handling preprocessor directives, so this is just a pass-through to the scanner
    return scanner_next(preprocessor->scanner);
}
