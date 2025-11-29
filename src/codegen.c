#include <stdlib.h>
#include <string.h>
#include "include/codegen.h"
#include "include/string_builder.h"

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
    // TODO
    return NULL;
}

static void codegen_destroy(codegen_t *cg) {
    // TODO
}

char *codegen_translation_unit(ast_node_t *ast) {
    // TODO
    codegen_t *cg = codegen_create();
    codegen_destroy(cg);
    return strdup(
"default rel\n"
"global main\n\n"
"section .text\n\n"
"main:\n"
"\tmov     rax, 2017\n"
"\tret\n");
}
