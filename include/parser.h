/**
 * @brief The parser takes the tokens and produces an abstract syntax tree.
 */

#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#include "include/list.h"
#include "include/ast.h"

ast_t *parse(list_t *tokens);

#endif  // INCLUDE_PARSER_H_

// TODO probably remove
/*

[ Source code ]
    ↓
[ AST / IR ]
    ↓
[ Assembly (.s) ]
    ↓
[ Assembler → Object file (.o) ]
    ↓
[ Linker → Executable (.exe or .out) ]



global main
section .text

main:
    mov eax, 2017
    ret



void generate_translation_unit(ASTNode* tu) {
    for each declaration in tu:
        if (declaration is function) {
            emit_label(declaration->name);
            generate_statement(declaration->body);
        }
}

void generate_statement(Statement* stmt) {
    if (stmt is ReturnStatement) {
        generate_expression(stmt->return_expr);
        emit("ret");
    }
}

void generate_expression(Expression* expr) {
    if (expr is IntegerLiteral) {
        emit("mov eax, %d", expr->int_value);
    }
}
*/
