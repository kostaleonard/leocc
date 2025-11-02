#include <string.h>
#include "include/exceptions.h"
#include "include/parser.h"

parser_t *parser_create(preprocessor_t *pp) {
    if (NULL == pp) {
        Throw(FAILURE_INVALID_INPUT);
    }
    parser_t *parser = calloc(1, sizeof(parser_t));
    if (NULL == parser) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    parser->filename = strdup(pp->scanner->filename);
    parser->current = preprocessor_next(pp);
    parser->lookahead = preprocessor_next(pp);
    return parser;
}

void parser_destroy(parser_t *parser) {
    if (NULL == parser) {
        Throw(FAILURE_INVALID_INPUT);
    }
    free(parser->filename);
    free(parser);
}

void parser_advance(parser_t *parser) {
    if (parser->current->kind != TOK_EOF) {
        parser->current = parser->lookahead;
        parser->lookahead = preprocessor_next(parser->pp);
    }
}

decl_spec_t parse_decl_specifiers(parser_t *parser) {
    decl_spec_t spec = {0};
    while (is_decl_specifier(parser->current->kind)) {
        switch (parser->current->kind) {
            /*
            case TOK_STATIC:
                spec.storage_class = SC_STATIC;
                break;
            */
            case TOK_INT:
                spec.type_spec = TYPE_INT;
                break;
            /*
            case TOK_CONST:
                spec.qualifiers |= QUAL_CONST;
                break;
            */
            // ...
        }
        parser_advance(parser);
    }
    return spec;
}

ast_node_t *parse_external_declaration(parser_t *parser) {
    // TODO implement this
    // Most start with a declaration specifier (type, storage class, etc.)
    decl_spec_t spec = parse_decl_specifiers(parser);
    if (TOK_IDENTIFIER == parser->current->kind &&
        TOK_LPAREN == parser->lookahead->kind) {
        return parse_function_definition(parser, &spec);
    }
    // TODO parse declaration
    //return parse_declaration(parser, &spec);
   return NULL;
}

ast_node_t *parse_translation_unit(parser_t *parser) {
    ast_node_t *root = ast_node_create(AST_TRANSLATION_UNIT);
    while (parser->current->kind != TOK_EOF) {
        ast_node_t *decl = parse_external_declaration(parser);
        ast_add_child(root, decl);
    }
    return root;
}

