/**
 * @brief Runs the unit test suite.
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include "tests/include/test_fib.h"

int main(int argc, char **argv) {
    const struct CMUnitTest tests[] = {
        // test_fib.h
        cmocka_unit_test(test_fib_returns_fibonacci_number),
        cmocka_unit_test(test_fib_throws_error_on_negative_input),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
