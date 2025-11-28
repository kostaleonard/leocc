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
        parse_translation_unit(NULL);
    } Catch(e) {
        exception_thrown = true;
    }
    assert_true(exception_thrown);
    assert_true(FAILURE_INVALID_INPUT == e);
}

void test_parse_single_function_declaration() {
    scanner_t *scanner = scanner_create_from_text(
        "int main() {\n\treturn 2017;\n}\n");
    preprocessor_t *pp = preprocessor_create(scanner);
    parser_t *parser = parser_create(pp);
    ast_node_t *ast = parse_translation_unit(parser);
    assert_true(AST_TRANSLATION_UNIT == ast->kind);
    assert_true(1 == ast->child_count);
    ast_node_t *function_def = ast->children[0];
    assert_true(AST_FUNCTION_DEF == function_def->kind);
    assert_true(3 == function_def->child_count);
    ast_node_t *decl_specifiers = function_def->children[0];
    assert_true(AST_DECL_SPECIFIERS == decl_specifiers->kind);
    assert_true(1 == decl_specifiers->child_count);
    ast_node_t *type_specifier = decl_specifiers->children[0];
    assert_true(AST_TYPE_SPECIFIER == type_specifier->kind);
    assert_true(TYPE_INT == type_specifier->data.type_spec);
    assert_true(0 == type_specifier->child_count);
    ast_node_t *declarator = function_def->children[1];
    assert_true(AST_DECLARATOR == declarator->kind);
    assert_true(1 == declarator->child_count);
    ast_node_t *identifier = declarator->children[0];
    assert_true(AST_IDENTIFIER == identifier->kind);
    assert_true(0 == strcmp(identifier->data.ident, "main"));
    assert_true(0 == identifier->child_count);
    ast_node_t *compound_stmt = function_def->children[2];
    assert_true(AST_COMPOUND_STMT == compound_stmt->kind);
    assert_true(1 == compound_stmt->child_count);
    ast_node_t *return_stmt = compound_stmt->children[0];
    assert_true(AST_RETURN_STMT == return_stmt->kind);
    assert_true(1 == return_stmt->child_count);
    ast_node_t *int_literal = return_stmt->children[0];
    assert_true(AST_INT_LITERAL == int_literal->kind);
    assert_true(2017 == int_literal->data.int_value);
    assert_true(0 == int_literal->child_count);
    ast_node_destroy(ast);
    parser_destroy(parser);
}
