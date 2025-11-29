/**
 * @brief The code generator transforms the AST into assembly.
 */

#ifndef INCLUDE_CODEGEN_H_
#define INCLUDE_CODEGEN_H_
#include <stddef.h>
#include "include/ast.h"

/**
 * @brief Returns the assembly code generated from the AST.
 * 
 * @param ast The root of translation unit's AST.
 */
char *codegen_translation_unit(ast_node_t *ast);

#endif  // INCLUDE_CODEGEN_H_
