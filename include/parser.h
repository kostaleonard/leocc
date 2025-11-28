/**
 * @brief The parser takes tokens and produces an abstract syntax tree.
 */

#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#include "include/list.h"
#include "include/preprocessor.h"
#include "include/types.h"
#include "include/ast.h"

/**
 * @brief The parser takes tokens and produces an abstract syntax tree.
 * 
 * @param pp The preprocessor.
 * @param filename The name of the translation unit being parsed.
 * @param current The current token, not yet consumed. Supplied by the
 * preprocessor.
 * @param lookahead The token after current.
 */
typedef struct parser_t {
    preprocessor_t *pp;
    char *filename;
    token_t *current;
    token_t *lookahead;
} parser_t;

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

/**
 * @brief Returns a new parser on the preprocessor's current file. 
 */
parser_t *parser_create(preprocessor_t *pp);

/**
 * @brief Frees all memory associated with the parser.
 */
void parser_destroy(parser_t *parser);

/**
 * @brief Parses the current program and returns the abstract syntax tree.
 */
ast_node_t *parse_translation_unit(parser_t *parser);

#endif  // INCLUDE_PARSER_H_
