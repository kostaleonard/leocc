#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include "include/fib.h"

void test_fib_returns_fibonacci_number() {
    assert_true(fib(0) == 0);
    assert_true(fib(1) == 1);
    assert_true(fib(2) == 1);
    assert_true(fib(3) == 2);
    assert_true(fib(4) == 3);
    assert_true(fib(5) == 5);
    assert_true(fib(15) == 610);
}

void test_fib_throws_error_on_negative_input() {
    // TODO
    assert_true(false);
}
