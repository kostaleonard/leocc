#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include "exceptions.h"
#include "fib.h"

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
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        fib(-1);
    } Catch(e) {
        exception_thrown = true;
        assert_true(FAILURE_INVALID_INPUT == e);
    }
    assert_true(exception_thrown);
}
