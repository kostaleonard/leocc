/**
 * @brief Contains definitions related to the abstract syntax tree (AST).
 */

#ifndef INCLUDE_AST_H_
#define INCLUDE_AST_H_
#include <stdint.h>
#include "include/types.h"
#include "include/token.h"

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

typedef enum operator_t {
    OP_NONE = 0,

    /*-----------------------------*
     *   Primary / Postfix Ops     *
     *-----------------------------*/
    OP_POST_INC,        // expr++
    OP_POST_DEC,        // expr--
    OP_FUNC_CALL,       // expr(args)
    OP_INDEX,           // expr[expr]
    OP_MEMBER,          // expr.member
    OP_PTR_MEMBER,      // expr->member

    /*-----------------------------*
     *        Unary Operators      *
     *-----------------------------*/
    OP_PRE_INC,         // ++expr
    OP_PRE_DEC,         // --expr
    OP_POSITIVE,        // +expr
    OP_NEGATIVE,        // -expr
    OP_BIT_NOT,         // ~expr
    OP_LOGICAL_NOT,     // !expr
    OP_ADDRESS_OF,      // &expr
    OP_DEREF,           // *expr
    OP_SIZEOF,          // sizeof (handled specially)
    OP_ALIGNOF,         // _Alignof / alignof (C11/C23)

    /*-----------------------------*
     *      Cast / Convert Ops     *
     *-----------------------------*/
    OP_CAST,            // (type)expr

    /*-----------------------------*
     *   Multiplicative Operators  *
     *-----------------------------*/
    OP_MUL,             // *
    OP_DIV,             // /
    OP_MOD,             // %

    /*-----------------------------*
     *    Additive Operators       *
     *-----------------------------*/
    OP_ADD,             // +
    OP_SUB,             // -

    /*-----------------------------*
     *   Bitwise Shift Operators   *
     *-----------------------------*/
    OP_SHL,             // <<
    OP_SHR,             // >>

    /*-----------------------------*
     *     Relational Operators    *
     *-----------------------------*/
    OP_LT,              // <
    OP_GT,              // >
    OP_LE,              // <=
    OP_GE,              // >=

    /*-----------------------------*
     *     Equality Operators      *
     *-----------------------------*/
    OP_EQ,              // ==
    OP_NEQ,             // !=

    /*-----------------------------*
     *    Bitwise Operators        *
     *-----------------------------*/
    OP_BIT_AND,         // &
    OP_BIT_XOR,         // ^
    OP_BIT_OR,          // |

    /*-----------------------------*
     *  Logical AND / OR Operators *
     *-----------------------------*/
    OP_LOGICAL_AND,     // &&
    OP_LOGICAL_OR,      // ||

    /*-----------------------------*
     *    Ternary Operator         *
     *-----------------------------*/
    OP_CONDITIONAL,     // ?:

    /*-----------------------------*
     *    Assignment Operators     *
     *-----------------------------*/
    OP_ASSIGN,          // =
    OP_ADD_ASSIGN,      // +=
    OP_SUB_ASSIGN,      // -=
    OP_MUL_ASSIGN,      // *=
    OP_DIV_ASSIGN,      // /=
    OP_MOD_ASSIGN,      // %=
    OP_SHL_ASSIGN,      // <<=
    OP_SHR_ASSIGN,      // >>=
    OP_AND_ASSIGN,      // &=
    OP_XOR_ASSIGN,      // ^=
    OP_OR_ASSIGN,       // |=

    /*-----------------------------*
     *  Comma Operator (lowest)    *
     *-----------------------------*/
    OP_COMMA,           // ,
} operator_t;

/**
 * @brief Represents a node in the AST.
 * 
 * @param kind The kind of AST node.
 * @param loc The location at which the AST node appears.
 * @param children An array of child AST nodes. Only some nodes have children.
 * @param child_count The length of the children array.
 * @param data The AST node's data, if it has any.
 */
typedef struct ast_node_t {
    ast_kind_t kind;
    source_loc_t loc;
    struct ast_node_t **children;
    size_t child_count;
    union {
        char *ident;
        int64_t int_value;
        char *string_value;
        operator_t operator;
        type_spec_t type_spec;
    } data;
} ast_node_t;

/**
 * @brief Creates an empty AST node of a given kind.
 */
ast_node_t *ast_node_create(ast_kind_t kind);

/**
 * @brief Frees all memory associated with the AST, including children.
 */
void ast_node_destroy(ast_node_t *node);

/**
 * @brief Adds the child node to the parent's children array.
 */
void ast_add_child(ast_node_t *parent, ast_node_t *child);

/**
 * @brief Prints the AST node and its children recursively.
 */
void ast_node_print(ast_node_t *node);

#endif  // INCLUDE_AST_H_
