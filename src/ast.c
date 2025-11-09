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

void ast_node_print(ast_node_t *node) {
    if (NULL == node) {
        Throw(FAILURE_INVALID_INPUT);
    }
    printf("TODO print ast\n");
}
