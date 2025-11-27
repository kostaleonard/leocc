/**
 * @brief The preprocessor handles directives and outputs tokens from files.
 */

#ifndef INCLUDE_PREPROCESSOR_H_
#define INCLUDE_PREPROCESSOR_H_

#include "include/scanner.h"

/**
 * @brief The preprocessor handles directives and outputs tokens from files.
 * 
 * @param scanner The scanner for the current file.
 */
typedef struct preprocessor_t {
    scanner_t *scanner;
} preprocessor_t;

/**
 * @brief Returns a new preprocessor with the scanner's file as the start file.
 */
preprocessor_t *preprocessor_create(scanner_t *scanner);

/**
 * @brief Frees all memory associated with the preprocessor.
 */
void preprocessor_destroy(preprocessor_t *pp);

/**
 * @brief Returns the next token in the text after handling directives.
 */
token_t *preprocessor_next(preprocessor_t *pp);

#endif  // INCLUDE_PREPROCESSOR_H_
