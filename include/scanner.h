/**
 * @brief The scanner takes one file's text as input and outputs the tokens.
 */

#ifndef INCLUDE_SCANNER_H_
#define INCLUDE_SCANNER_H_

#include <stddef.h>
#include "include/list.h"
#include "include/token.h"

// TODO docstring
// TODO use source_loc_t for standardization in scanner, preprocessor, and parser
typedef struct scanner_t {
    char *filename; // TODO may be NULL
    char *text;
    size_t idx;
    size_t line;
    size_t column;
} scanner_t;

// TODO need tests and docstrings
scanner_t *scanner_create_from_file(char *filename);

scanner_t *scanner_create_from_text(char *text);

void scanner_destroy(scanner_t *scanner);

token_t *scanner_next(scanner_t *scanner);

// TODO this function is sort of tech debt because I've got code and tests for scan, which I want to keep for testing while I reimplement scanning
list_t *scanner_all(scanner_t *scanner);

// TODO note this is only really for testing, and change program_text arg to scanner_t *
// TODO maybe change name to like scanner_all or something
/**
 * @brief Returns a list of the tokens that compose the program.
 * 
 * @param program_text The preprocessed program text.
 * @return list_t* A list of the tokens that compose the program. Each node_t's
 * void *data points to a token_t. Callers must free.
 */
//list_t *scan(char *program_text);

#endif  // INCLUDE_SCANNER_H_
