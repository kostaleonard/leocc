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
#include "include/list.h"
#include "include/scanner.h"
#include "include/parser.h"
#include "tests/include/test_parser.h"

void test_parse_fails_on_invalid_input() {
    bool exception_thrown = false;
    volatile CEXCEPTION_T e;
    Try {
        parse(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_parse_single_function_declaration() {
    /*
    list_t *tokens = scan("int main() {\n\treturn 2017;\n}\n");
    ast_t *ast = parse(tokens);
    assert_true(NULL != ast);
    assert_true(1 == list_length(ast->root->declarations));
    declaration_t *declaration =
        (declaration_t *)ast->root->declarations->head->data;
    assert_true(FUNCTION_DECLARATION == declaration->kind);
    function_declaration_data_t *fdd =
        (function_declaration_data_t *)declaration->data;
    assert_true(0 == strncmp(fdd->name, "main", strlen("main")));
    assert_true(TYPE_INT == fdd->return_type);
    assert_true(1 == list_length(fdd->body));
    statement_t *stmt = (statement_t *)fdd->body->head->data;
    assert_true(STATEMENT_RETURN == stmt->kind);
    return_statement_data_t *rsd = (return_statement_data_t *)stmt->data;
    assert_true(EXPR_INT_LITERAL == rsd->return_value->kind);
    expr_int_literal_data_t *eild =
        (expr_int_literal_data_t *)rsd->return_value->data;
    assert_true(2017 == eild->value);
    list_destroy(tokens);
    ast_destroy(ast);
    */
   // TODO
   assert_true(false);
}
