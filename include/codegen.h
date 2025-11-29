/**
 * @brief The code generator transforms the AST into assembly.
 */

#ifndef INCLUDE_CODEGEN_H_
#define INCLUDE_CODEGEN_H_
#include <stddef.h>
#include "include/ast.h"

// TODO this data structure is an implementation detail that should live in codegen.c
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

char *codegen_generate_translation_unit(codegen_t *cg, ast_node_t *ast);

// TODO these will be static functions in codegen.c
codegen_t *codegen_create();

void codegen_destroy(codegen_t *cg);

#endif  // INCLUDE_CODEGEN_H_
