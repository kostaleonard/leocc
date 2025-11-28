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
    parser->pp = pp;
    if (NULL != pp->scanner->loc.filename) {
        parser->filename = strdup(pp->scanner->loc.filename);
        if (NULL == parser->filename) {
            Throw(FAILURE_COULD_NOT_MALLOC);
        }
    }
    parser->current = preprocessor_next(parser->pp);
    parser->lookahead = preprocessor_next(parser->pp);
    return parser;
}

void parser_destroy(parser_t *parser) {
    if (NULL == parser) {
        Throw(FAILURE_INVALID_INPUT);
    }
    free(parser->filename);
    free(parser);
}

/**
 * @brief Consumes the current token and pulls the next from the preprocessor. 
 */
static void parser_advance(parser_t *parser) {
    if (NULL == parser) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (parser->current->kind != TOK_EOF) {
        token_destroy(parser->current);
        parser->current = parser->lookahead;
        parser->lookahead = preprocessor_next(parser->pp);
    }
}

/**
 * @brief Requires a match on the current token, then consumes that token.
 */
static void parser_expect(parser_t *parser, token_kind_t expected) {
    if (NULL == parser) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (parser->current->kind != expected) {
        Throw(FAILURE_PARSE_ERROR);
    }
    parser_advance(parser);
}

static ast_node_t *parse_declarator(parser_t *parser) {
    ast_node_t *decl = ast_node_create(AST_DECLARATOR);
    decl->loc = source_loc_dup(parser->current->loc);
    if (parser->current->kind == TOK_IDENTIFIER) {
        ast_node_t *id = ast_node_create(AST_IDENTIFIER);
        id->loc = source_loc_dup(parser->current->loc);
        id->data.ident = strdup(parser->current->data.ident);
        if (NULL == id->data.ident) {
            Throw(FAILURE_COULD_NOT_MALLOC);
        }
        ast_add_child(decl, id);
        parser_advance(parser);
    } else {
        Throw(FAILURE_PARSE_ERROR);
    }
    parser_expect(parser, TOK_LPAREN);
    parser_expect(parser, TOK_RPAREN);
    return decl;
}

static ast_node_t *parse_primary_expression(parser_t *parser) {
    ast_node_t *node = NULL;
    switch (parser->current->kind) {
        case TOK_INT_LITERAL:
            node = ast_node_create(AST_INT_LITERAL);
            node->loc = source_loc_dup(parser->current->loc);
            node->data.int_value = parser->current->data.int_value;
            parser_advance(parser);
            break;
        default:
            Throw(FAILURE_PARSE_ERROR);
            break;
    }
    return node;
}

static ast_node_t *parse_expression(parser_t *parser) {
    return parse_primary_expression(parser);
}

static ast_node_t *parse_return_statement(parser_t *parser) {
    ast_node_t *node = ast_node_create(AST_RETURN_STMT);
    node->loc = source_loc_dup(parser->current->loc);
    parser_expect(parser, TOK_RETURN);
    if (parser->current->kind != TOK_SEMICOLON) {
        ast_node_t *expr = parse_expression(parser);
        ast_add_child(node, expr);
    }
    parser_expect(parser, TOK_SEMICOLON);
    return node;
}

static ast_node_t *parse_statement(parser_t *parser) {
    return parse_return_statement(parser);
}

static ast_node_t *parse_compound_statement(parser_t *parser) {
    ast_node_t *block = ast_node_create(AST_COMPOUND_STMT);
    block->loc = source_loc_dup(parser->current->loc);
    parser_expect(parser, TOK_LBRACE);
    while (parser->current->kind != TOK_RBRACE &&
           parser->current->kind != TOK_EOF) {
        ast_node_t *stmt = parse_statement(parser);
        ast_add_child(block, stmt);
    }
    parser_expect(parser, TOK_RBRACE);
    return block;
}

static ast_node_t *parse_function_definition(
    parser_t *parser, decl_spec_t *spec) {
    ast_node_t *fn = ast_node_create(AST_FUNCTION_DEF);
    fn->loc = source_loc_dup(parser->current->loc);
    ast_node_t *specs_node = ast_node_create(AST_DECL_SPECIFIERS);
    specs_node->loc = source_loc_dup(parser->current->loc);
    if (spec->type_spec != TYPE_NONE) {
        ast_node_t *type = ast_node_create(AST_TYPE_SPECIFIER);
        type->data.type_spec = spec->type_spec;
        ast_add_child(specs_node, type);
    }
    ast_add_child(fn, specs_node);
    ast_node_t *declarator = parse_declarator(parser);
    ast_add_child(fn, declarator);
    ast_node_t *body = parse_compound_statement(parser);
    ast_add_child(fn, body);
    return fn;
}

static bool is_decl_specifier(token_kind_t kind) {
    switch (kind) {
        case TOK_INT:
            return true;
        default:
            return false;
    }
}

static decl_spec_t parse_decl_specifiers(parser_t *parser) {
    decl_spec_t spec = {0};
    while (is_decl_specifier(parser->current->kind)) {
        switch (parser->current->kind) {
            case TOK_INT:
                spec.type_spec = TYPE_INT;
                break;
            default:
                Throw(FAILURE_PARSE_ERROR);
                break;
        }
        parser_advance(parser);
    }
    return spec;
}

static ast_node_t *parse_external_declaration(parser_t *parser) {
    decl_spec_t spec = parse_decl_specifiers(parser);
    if (TOK_IDENTIFIER == parser->current->kind &&
        TOK_LPAREN == parser->lookahead->kind) {
        return parse_function_definition(parser, &spec);
    }
    return NULL;
}

ast_node_t *parse_translation_unit(parser_t *parser) {
    if (NULL == parser) {
        Throw(FAILURE_INVALID_INPUT);
    }
    ast_node_t *root = ast_node_create(AST_TRANSLATION_UNIT);
    while (parser->current->kind != TOK_EOF) {
        ast_node_t *decl = parse_external_declaration(parser);
        ast_add_child(root, decl);
    }
    return root;
}
