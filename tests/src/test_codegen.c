#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/exceptions.h"
#include "include/codegen.h"
#include "include/scanner.h"
#include "include/preprocessor.h"
#include "include/parser.h"
#include "include/ast.h"
#include "tests/include/test_codegen.h"

void test_codegen_translation_unit_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        codegen_translation_unit(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_codegen_translation_unit_simple_program() {
    scanner_t *scanner = scanner_create_from_text(
        "int main() {\n\treturn 2017;\n}\n");
    preprocessor_t *pp = preprocessor_create(scanner);
    parser_t *parser = parser_create(pp);
    ast_node_t *ast = parse_translation_unit(parser);
    char *assembly_prog = codegen_translation_unit(ast);
    char *expected_strings[] = {
        "default rel",
        "global main",
        "section .text",
        "main:",
        "mov",
        "rax, 2017",
        "ret"
    };
    size_t n = sizeof(expected_strings) / sizeof(expected_strings[0]);
    for (size_t idx = 0; idx < n; idx++) {
        assert_true(NULL != strstr(assembly_prog, expected_strings[idx]));
    }
    free(assembly_prog);
    ast_node_destroy(ast);
    parser_destroy(parser);
}
