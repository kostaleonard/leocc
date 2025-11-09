/**
 * @brief Contains definitions related to the abstract syntax tree (AST).
 */

#ifndef INCLUDE_AST_H_
#define INCLUDE_AST_H_
#include "include/types.h"

// TODO remove brainstorming
/*
AST:
TranslationUnit
└── FunctionDeclaration
    ├── returnType: int
    ├── name: main
    ├── parameters: []
    └── body:
        └── ReturnStatement
            └── IntegerLiteral: 2017
*/

// TODO docstrings
// TODO cull unused ones until needed so we don't have them hanging around
typedef enum {
    AST_TRANSLATION_UNIT,
    AST_FUNCTION_DEF,
    AST_DECLARATION,
    AST_PARAM,
    AST_COMPOUND_STMT,
    AST_RETURN_STMT,
    AST_IF_STMT,
    AST_BINARY_EXPR,
    AST_UNARY_EXPR,
    AST_CALL_EXPR,
    AST_IDENTIFIER,
    AST_INT_LITERAL,
    AST_STRING_LITERAL,
    AST_DECL_SPECIFIERS,
    AST_STORAGE_CLASS,
    AST_TYPE_SPECIFIER,
    AST_DECLARATOR,
} ast_kind_t;

// TODO replace filename/line/col with source_loc_t 
typedef struct ast_node_t {
    ast_kind_t kind;
    char *filename;
    size_t line;
    size_t column;
    struct ast_node_t **children;
    size_t child_count;
    // TODO not sure how each of these will all fit into the scheme.
    union {
        char *ident;              // for identifiers
        long int_value;           // for integer literals
        char *string_value;       // for string literals
        char *operator;           // for operators like "+"
        type_spec_t type_spec;
    } data;
} ast_node_t;

ast_node_t *ast_node_create(ast_kind_t kind);

/**
 * @brief Frees all memory associated with the AST.
 */
void ast_node_destroy(ast_node_t *node);

void ast_add_child(ast_node_t *parent, ast_node_t *child);

void ast_node_print(ast_node_t *node);

#endif  // INCLUDE_AST_H_
