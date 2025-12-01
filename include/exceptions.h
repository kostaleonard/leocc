/**
 * @brief Provides exception handling.
 */

#ifndef INCLUDE_EXCEPTIONS_H_
#define INCLUDE_EXCEPTIONS_H_
#include "submodules/CException/CException/lib/CException.h"

typedef enum error_code_t {
    SUCCESS,
    FAILURE_COULD_NOT_MALLOC,
    FAILURE_INVALID_INPUT,
    FAILURE_NOT_IMPLEMENTED,
    FAILURE_EMPTY_LIST,
    FAILURE_INDEX_OUT_OF_BOUNDS,
    FAILURE_INVALID_CHARACTER,
    FAILURE_PARSER_ERROR,
    FAILURE_INVALID_COMMAND_LINE_ARGS,
    FAILURE_FILE_IO,
    FAILURE_SCANNER_ERROR,
} error_code_t;

#endif  // INCLUDE_EXCEPTIONS_H_
