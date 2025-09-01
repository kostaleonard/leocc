#include "include/scanner.h"
#include "include/token.h"
#include "include/exceptions.h"

list_t *scan(char *program_text) {
    if (NULL == program_text) {
        Throw(FAILURE_INVALID_INPUT);
    }
    // TODO free function and compare function defined in token.h
    // TODO do we need a compare function? What value do we get from being able to say that token A is "equal" to token B?
    //list_t *tokens = list_create()
    return NULL;
}
