#include <stdlib.h>
#include <string.h>
#include "include/exceptions.h"
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

char *codegen_translation_unit(ast_node_t *ast) {
    if (NULL == ast) {
        Throw(FAILURE_INVALID_INPUT);
    }
    codegen_t *cg = codegen_create();
    char *prog = strdup(cg->sb->data);
    // TODO
    codegen_destroy(cg);
    return prog;
}
