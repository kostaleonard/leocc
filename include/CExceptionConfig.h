/**
 * @brief Configures CException.
 */

#ifndef INCLUDE_CEXCEPTIONCONFIG_H_
#define INCLUDE_CEXCEPTIONCONFIG_H_
#include <stdio.h>
#include <stdlib.h>

#define CEXCEPTION_NO_CATCH_HANDLER(id) \
    printf("Uncaught Throw! ID: %u at %s:%d\n", (id), __FILE__, __LINE__); \
    exit((int)(id))

#endif
