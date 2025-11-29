#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "include/exceptions.h"
#include "include/string_builder.h"
#include "tests/include/test_string_builder.h"

void test_string_builder_create_returns_empty_string_builder() {
    string_builder_t *sb = string_builder_create();
    assert_true(NULL != sb->data);
    assert_true(0 == strlen(sb->data));
    assert_true(0 == sb->length);
    assert_true(0 != sb->capacity);
    string_builder_destroy(sb);
}

void test_string_builder_append_adds_to_string() {
    string_builder_t *sb = string_builder_create();
    string_builder_append(sb, "hello");
    assert_true(5 == sb->length);
    assert_true(0 == strcmp(sb->data, "hello"));
    string_builder_append(sb, " world");
    assert_true(11 == sb->length);
    assert_true(0 == strcmp(sb->data, "hello world"));
    string_builder_destroy(sb);
}

void test_string_builder_append_expands_capacity() {
    string_builder_t *sb = string_builder_create();
    size_t original_capacity = sb->capacity;
    // The last char in the string is always \0.
    for (size_t idx = 0; idx < original_capacity - 1; idx++) {
        string_builder_append(sb, "A");
    }
    assert_true(sb->capacity == original_capacity);
    string_builder_append(sb, "A");
    assert_true(sb->capacity > original_capacity);
    string_builder_destroy(sb);
}

void test_string_builder_append_fails_on_invalid_input() {
    string_builder_t *sb = string_builder_create();
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        string_builder_append(NULL, "A");
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
    exception_thrown = false;
    Try {
        string_builder_append(sb, NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
    string_builder_destroy(sb);
}

void test_string_builder_append_int_converts_num_to_str() {
    string_builder_t *sb = string_builder_create();
    string_builder_append(sb, "hello");
    string_builder_append_int(sb, 2017);
    assert_true(9 == sb->length);
    assert_true(0 == strcmp(sb->data, "hello2017"));
    string_builder_append_int(sb, -2);
    assert_true(11 == sb->length);
    assert_true(0 == strcmp(sb->data, "hello2017-2"));
    string_builder_destroy(sb);
}

void test_string_builder_append_int_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        string_builder_append_int(NULL, 2017);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_string_builder_destroy_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        string_builder_destroy(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}
