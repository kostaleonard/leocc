/**
 * @brief The parser takes tokens and produces an abstract syntax tree.
 */

#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#include "include/list.h"
#include "include/preprocessor.h"
#include "include/ast.h"

// TODO docstrings
typedef struct parser_t {
    preprocessor_t *pp;
    char *filename;
    token_t *current;
    token_t *lookahead;
} parser_t;

// TODO look into these enums and structs
typedef enum {
    STORAGE_NONE,
    STORAGE_TYPEDEF,
    STORAGE_EXTERN,
    STORAGE_STATIC,
    STORAGE_AUTO,
    STORAGE_REGISTER
} storage_class_t;

// TODO clean this up
typedef enum {
    TYPE_NONE = 0,

    // Standard integer types
    TYPE_VOID,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_SIGNED_CHAR,
    TYPE_UNSIGNED_CHAR,
    TYPE_SHORT,
    TYPE_UNSIGNED_SHORT,
    TYPE_INT,
    TYPE_UNSIGNED_INT,
    TYPE_LONG,
    TYPE_UNSIGNED_LONG,
    TYPE_LONG_LONG,
    TYPE_UNSIGNED_LONG_LONG,

    // Floating-point types
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LONG_DOUBLE,

    // Other types
    TYPE_ENUM,
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_TYPEDEF_NAME,  // user-defined typedef name

    // Extensions (optional)
    TYPE_COMPLEX,
    TYPE_IMAGINARY
} type_spec_t;

typedef enum {
    QUAL_NONE = 0,
    QUAL_CONST = 1 << 0,
    QUAL_VOLATILE = 1 << 1,
    QUAL_RESTRICT = 1 << 2,
    QUAL_ATOMIC = 1 << 3,
} type_qual_t;

typedef struct decl_spec_t {
    storage_class_t storage_class;
    type_spec_t type_spec;
    type_qual_t qualifiers;
} decl_spec_t;

parser_t *parser_create(preprocessor_t *pp);

void parser_destroy(parser_t *parser);

// TODO consume the current token and pull the next one from the preprocessor.
void parser_advance(parser_t *parser);

ast_node_t *parse_translation_unit(parser_t *parser);

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
