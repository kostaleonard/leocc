#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include <stdlib.h>
#include "include/exceptions.h"
#include "include/list.h"
#include "include/scanner.h"
#include "tests/include/test_scanner.h"

void test_scan_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        scan(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}
