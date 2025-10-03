/**
 * @brief Contains definitions related to the abstract syntax tree (AST).
 */

#ifndef INCLUDE_AST_H_
#define INCLUDE_AST_H_

#include "include/list.h"

// TODO remove
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

typedef struct translation_unit_t {
    list_t* declarations;
} translation_unit_t;

typedef struct ast_t {
    translation_unit_t *root;
} ast_t;

typedef enum declaration_code_t {
    FUNCTION_DECLARATION,
} declaration_code_t;

// TODO not sure if need to rename this to top_level_declaration_t--can you declare anything anywhere?
typedef struct declaration_t {
    declaration_code_t kind;
    void *data;
} declaration_t;

typedef enum type_t {
    TYPE_INT,
} type_t;

typedef struct function_declaration_data_t {
    char *name;
    type_t return_type;
    list_t *body;
} function_declaration_data_t;

typedef enum statement_code_t {
    STATEMENT_RETURN,
} statement_code_t;

typedef struct statement_t {
    statement_code_t kind;
    void *data;
} statement_t;

typedef enum expr_code_t {
    EXPR_INT_LITERAL,
} expr_code_t;

typedef struct expr_t {
    expr_code_t kind;
    void *data;
} expr_t;

typedef struct expr_int_literal_data_t {
    int value;
} expr_int_literal_data_t;

typedef struct return_statement_data_t {
    expr_t *return_value;
} return_statement_data_t;

/**
 * @brief Frees all memory associated with the AST.
 */
void ast_destroy(ast_t *ast);

#endif  // INCLUDE_AST_H_
