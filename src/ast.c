#include <stdlib.h>
#include "include/exceptions.h"
#include "include/ast.h"

ast_node_t *ast_node_create(ast_kind_t kind) {
    ast_node_t *node = calloc(1, sizeof(ast_node_t));
    if (NULL == node) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    node->kind = kind;
    return node;
}

void ast_node_destroy(ast_node_t *node) {
    // TODO
    free(node);
}

void ast_add_child(ast_node_t *parent, ast_node_t *child) {
    if (NULL == parent || NULL == child) {
        Throw(FAILURE_INVALID_INPUT);
    }
    parent->children = realloc(parent->children, (1 + parent->child_count) * sizeof(ast_node_t *));
    if (NULL == parent->children) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    parent->children[parent->child_count] = child;
    parent->child_count++;
}

static void ast_node_print_tree(ast_node_t *node, size_t indent_level) {
    if (NULL == node) {
        Throw(FAILURE_INVALID_INPUT);
    }
    for (size_t idx = 0; idx < indent_level; idx++) {
        printf(" ");
    }
    switch (node->kind) {
        case AST_TRANSLATION_UNIT:
            printf("AST_TRANSLATION_UNIT:\n");
            break;
        case AST_FUNCTION_DEF:
            printf("AST_FUNCTION_DEF:\n");
            break;
        case AST_DECLARATION:
            printf("AST_DECLARATION:\n");
            break;
        case AST_PARAM:
            printf("AST_PARAM:\n");
            break;
        case AST_COMPOUND_STMT:
            printf("AST_COMPOUND_STMT:\n");
            break;
        case AST_RETURN_STMT:
            printf("AST_RETURN_STMT:\n");
            break;
        case AST_IF_STMT:
            printf("AST_IF_STMT:\n");
            break;
        case AST_BINARY_EXPR:
            printf("AST_BINARY_EXPR:\n");
            break;
        case AST_UNARY_EXPR:
            printf("AST_UNARY_EXPR:\n");
            break;
        case AST_CALL_EXPR:
            printf("AST_CALL_EXPR:\n");
            break;
        case AST_IDENTIFIER:
            printf("AST_IDENTIFIER: %s\n", node->data.ident);
            break;
        case AST_INT_LITERAL:
            printf("AST_INT_LITERAL: %ld\n", node->data.int_value);
            break;
        case AST_STRING_LITERAL:
            printf("AST_STRING_LITERAL:\n");
            break;
        case AST_DECL_SPECIFIERS:
            printf("AST_DECL_SPECIFIERS:\n");
            break;
        case AST_STORAGE_CLASS:
            printf("AST_STORAGE_CLASS:\n");
            break;
        case AST_TYPE_SPECIFIER:
            printf("AST_TYPE_SPECIFIER:\n");
            break;
        case AST_DECLARATOR:
            printf("AST_DECLARATOR:\n");
            break;
        default:
            Throw(FAILURE_INVALID_INPUT);
    }
    for (size_t idx = 0; idx < node->child_count; idx++) {
        ast_node_print_tree(node->children[idx], indent_level + 2);
    }
}

void ast_node_print(ast_node_t *node) {
    if (NULL == node) {
        Throw(FAILURE_INVALID_INPUT);
    }
    ast_node_print_tree(node, 0);
}
