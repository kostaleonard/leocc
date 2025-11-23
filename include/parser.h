/**
 * @brief The parser takes tokens and produces an abstract syntax tree.
 */

#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#include "include/list.h"
#include "include/preprocessor.h"
#include "include/types.h"
#include "include/ast.h"

// TODO docstrings
typedef struct parser_t {
    preprocessor_t *pp;
    char *filename;
    token_t *current;
    token_t *lookahead;
} parser_t;

// TODO look into these enums and structs
typedef enum {
    STORAGE_NONE,
    STORAGE_TYPEDEF,
    STORAGE_EXTERN,
    STORAGE_STATIC,
    STORAGE_AUTO,
    STORAGE_REGISTER
} storage_class_t;

typedef struct decl_spec_t {
    storage_class_t storage_class;
    type_spec_t type_spec;
    type_qual_t qualifiers;
} decl_spec_t;

parser_t *parser_create(preprocessor_t *pp);

void parser_destroy(parser_t *parser);

ast_node_t *parse_translation_unit(parser_t *parser);

#endif  // INCLUDE_PARSER_H_
