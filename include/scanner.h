/**
 * @brief The scanner takes a program text as input and outputs the tokens.
 */

#ifndef INCLUDE_SCANNER_H_
#define INCLUDE_SCANNER_H_

#include <stddef.h>
#include "include/list.h"

/**
 * @brief Returns a list of the tokens that compose the program.
 * 
 * @param program_text The preprocessed program text.
 * @return list_t* A list of the tokens that compose the program. Each node_t's
 * void *data points to a token_t. Callers must free.
 */
list_t *scan(char *program_text);

#endif  // INCLUDE_SCANNER_H_
