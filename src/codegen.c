#include <stdlib.h>
#include <string.h>
#include "include/exceptions.h"
#include "include/codegen.h"
#include "include/string_builder.h"
#include "include/parser.h"

/**
 * @brief The code generator transforms the AST into assembly.
 * 
 * @param sb The assembly code produced during code generation.
 * @param next_label The next label to be used during code generation.
 */
typedef struct codegen_t {
    string_builder_t *sb;
    int next_label;
} codegen_t;

static codegen_t *codegen_create() {
    codegen_t *cg = calloc(1, sizeof(codegen_t));
    if (NULL == cg) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    cg->sb = string_builder_create();
    return cg;
}

static void codegen_destroy(codegen_t *cg) {
    if (NULL == cg) {
        Throw(FAILURE_INVALID_INPUT);
    }
    string_builder_destroy(cg->sb);
    free(cg);
}

static void codegen_label(codegen_t *cg, ast_node_t *function_def) {
    if (NULL == cg || NULL == function_def || AST_FUNCTION_DEF != function_def->kind) {
        Throw(FAILURE_INVALID_INPUT);
    }
    string_builder_t *sb = cg->sb;
    ast_node_t *declarator = function_def->children[1];
    ast_node_t *identifier = declarator->children[0];
    string_builder_append(sb, identifier->data.ident);
    string_builder_append(sb, ":\n");
}

static void codegen_expr(codegen_t *cg, ast_node_t *expr) {
    if (NULL == cg || NULL == expr) {
        Throw(FAILURE_INVALID_INPUT);
    }
    string_builder_t *sb = cg->sb;
    switch (expr->kind) {
        case AST_INT_LITERAL:
            string_builder_append(sb, "\tmov rax, ");
            string_builder_append_int(sb, expr->data.int_value);
            string_builder_append(sb, "\n");
            break;
        default:
            Throw(FAILURE_INVALID_INPUT);
    }
}

static void codegen_statement(codegen_t *cg, ast_node_t *statement) {
    if (NULL == cg || NULL == statement) {
        Throw(FAILURE_INVALID_INPUT);
    }
    string_builder_t *sb = cg->sb;
    switch (statement->kind) {
        case AST_COMPOUND_STMT:
            for (size_t idx = 0; idx < statement->child_count; idx++) {
                codegen_statement(cg, statement->children[idx]);
            }
            break;
        case AST_RETURN_STMT:
            codegen_expr(cg, statement->children[0]);
            string_builder_append(sb, "\tret\n\n");
            break;
        default:
            Throw(FAILURE_NOT_IMPLEMENTED);
    }
}

char *codegen_translation_unit(ast_node_t *ast) {
    if (NULL == ast) {
        Throw(FAILURE_INVALID_INPUT);
    }
    codegen_t *cg = codegen_create();
    string_builder_t *sb = cg->sb;
    string_builder_append(sb, "default rel\n");
    for (size_t idx = 0; idx < ast->child_count; idx++) {
        ast_node_t *top_level_child = ast->children[idx];
        if (AST_FUNCTION_DEF == top_level_child->kind) {
            // TODO when we implement storage classes in the AST, static functions are not made global in the assembly
            ast_node_t *declarator = top_level_child->children[1];
            ast_node_t *identifier = declarator->children[0];
            string_builder_append(sb, "global ");
            string_builder_append(sb, identifier->data.ident);
            string_builder_append(sb, "\n");
        }
    }
    string_builder_append(sb, "\nsection .text\n\n");
    for (size_t idx = 0; idx < ast->child_count; idx++) {
        ast_node_t *top_level_child = ast->children[idx];
        if (AST_FUNCTION_DEF == top_level_child->kind) {
            ast_node_t *function_def = top_level_child;
            codegen_label(cg, function_def);
            ast_node_t *body = function_def->children[2];
            codegen_statement(cg, body);
        }
    }
    char *prog = strdup(sb->data);
    codegen_destroy(cg);
    return prog;
}
