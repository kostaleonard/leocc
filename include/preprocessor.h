/**
 * @brief The preprocessor handles directives and outputs tokens from files.
 */

#ifndef INCLUDE_PREPROCESSOR_H_
#define INCLUDE_PREPROCESSOR_H_

#include "include/scanner.h"

// TODO tests
// TODO docstrings
// TODO this will have to be a stack (array) of scanners later--one for each file.
typedef struct preprocessor_t {
    scanner_t *scanner;
} preprocessor_t;

preprocessor_t *preprocessor_create(scanner_t *scanner);

void preprocessor_destroy(preprocessor_t *preprocessor);

token_t *preprocessor_next(preprocessor_t *preprocessor);

#endif  // INCLUDE_PREPROCESSOR_H_
