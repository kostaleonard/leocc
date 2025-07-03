#include <stdio.h>
#include "exceptions.h"
#include "fib.h"

int fib(int n) {
    if (n < 0) {
        // TODO need to set CEXCEPTION_NO_CATCH_HANDLER macro to kill the program with helpful debugging message when a throw occurs outside of a try-catch.
        // TODO remove debugging
        printf("Throwing\n");
        Throw(FAILURE_INVALID_INPUT);
    }
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}
