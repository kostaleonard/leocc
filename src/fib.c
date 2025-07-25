#include <stdio.h>
#include "include/exceptions.h"
#include "include/fib.h"

int fib(int n) {
    if (n < 0) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}
