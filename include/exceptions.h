/**
 * @brief Provides exception handling.
 */

#ifndef INCLUDE_EXCEPTIONS_H_
#define INCLUDE_EXCEPTIONS_H_
#include "CException.h"

typedef enum error_code_t {
    SUCCESS,
    FAILURE_COULD_NOT_MALLOC,
    FAILURE_INVALID_INPUT,
} error_code_t;

#endif  // INCLUDE_EXCEPTIONS_H_
