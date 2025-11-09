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
        free_token(parser->current);
        parser->current = parser->lookahead;
        parser->lookahead = preprocessor_next(parser->pp);
    }
}

void parser_expect(parser_t *parser, token_kind_t expected) {
    if (NULL == parser) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (parser->current->kind != expected) {
        Throw(FAILURE_PARSE_ERROR);
    }
    parser_advance(parser);
}

ast_node_t *parse_declarator(parser_t *parser) {
    // Create the declarator node
    ast_node_t *decl = ast_node_create(AST_DECLARATOR);
    //decl->loc = parser->current->loc;

    
    //
    // 1. Handle any leading '*' (pointer declarators)
    //
    /*
    while (parser->current->kind == TOK_STAR) {
        ast_node_t *ptr = ast_create(parser->arena, AST_POINTER);
        ptr->loc = parser->current->loc;
        ast_add_child(decl, ptr);
        parser_advance(parser);
    }
    */

    //
    // 2. Expect an identifier (the declared name)
    //
    if (parser->current->kind == TOK_IDENTIFIER) {
        ast_node_t *id = ast_node_create(AST_IDENTIFIER);
        //id->loc = parser->current->loc;
        id->data.ident = strdup(parser->current->data.ident);
        ast_add_child(decl, id);
        parser_advance(parser);
    } else {
        Throw(FAILURE_PARSE_ERROR);
    }
    parser_expect(parser, TOK_LPAREN);
    parser_expect(parser, TOK_RPAREN);

    /*
    //
    // 3. Handle possible postfixes: function parameters or array sizes
    //
    while (parser->current->kind == TOK_LPAREN || parser->current->kind == TOK_LBRACKET) {
        if (parser->current->kind == TOK_LPAREN) {
            // Function parameter list
            parser_advance(parser); // consume '('
            ast_node_t *params = ast_create(parser->arena, AST_PARAM_LIST);

            if (parser->current->kind != TOK_RPAREN) {
                do {
                    ast_node_t *param = parse_parameter_declaration(parser);
                    ast_add_child(params, param);
                    if (parser->current->kind == TOK_COMMA)
                        parser_advance(parser);
                    else
                        break;
                } while (parser->current->kind != TOK_RPAREN);
            }

            parser_expect(parser, TOK_RPAREN);
            ast_add_child(decl, params);
        } else if (parser->current->kind == TOK_LBRACKET) {
            // Array declarator
            parser_advance(parser); // consume '['
            ast_node_t *arr = ast_create(parser->arena, AST_ARRAY_DECL);
            if (parser->current->kind != TOK_RBRACKET) {
                ast_node_t *size_expr = parse_expression(parser);
                ast_add_child(arr, size_expr);
            }
            parser_expect(parser, TOK_RBRACKET);
            ast_add_child(decl, arr);
        }
    }
    */

    return decl;
}

static ast_node_t *parse_primary_expression(parser_t *parser) {
    ast_node_t *node = NULL;

    switch (parser->current->kind) {
        /*
        case TOK_IDENTIFIER:
            node = ast_create(parser->arena, AST_IDENTIFIER);
            node->loc = parser->current->loc;
            node->data.ident = strdup(parser->current->data.ident);
            parser_advance(parser);
            break;
        */
        case TOK_INT_LITERAL:
            node = ast_node_create(AST_INT_LITERAL);
            //node->loc = parser->current->loc;
            node->data.int_value = parser->current->data.int_value;
            parser_advance(parser);
            break;
        /*
        case TOK_LPAREN:
            parser_advance(parser); // consume '('
            node = parse_expression(parser);
            parser_expect(parser, TOK_RPAREN);
            break;
        */
        default:
            Throw(FAILURE_PARSE_ERROR);
            break;
    }

    return node;
}

ast_node_t *parse_expression(parser_t *parser) {
    return parse_primary_expression(parser);
}

ast_node_t *parse_return_statement(parser_t *parser) {
    // Create the AST node for 'return'
    ast_node_t *node = ast_node_create(AST_RETURN_STMT);
    //node->loc = parser->current->loc;

    // Expect and consume 'return'
    parser_expect(parser, TOK_RETURN);

    // Optional expression before the semicolon
    if (parser->current->kind != TOK_SEMICOLON) {
        ast_node_t *expr = parse_expression(parser);
        ast_add_child(node, expr);
    }

    // Expect and consume ';'
    parser_expect(parser, TOK_SEMICOLON);

    return node;
}

ast_node_t *parse_statement(parser_t *parser) {
    /*
    switch (p->current->kind) {
        case TOK_LBRACE:
            return parse_compound_statement(p);

        case TOK_IF:
            return parse_if_statement(p);

        case TOK_WHILE:
            return parse_while_statement(p);

        case TOK_FOR:
            return parse_for_statement(p);

        case TOK_DO:
            return parse_do_while_statement(p);

        case TOK_RETURN:
            return parse_return_statement(p);

        case TOK_BREAK:
            return parse_break_statement(p);

        case TOK_CONTINUE:
            return parse_continue_statement(p);

        case TOK_SWITCH:
            return parse_switch_statement(p);

        case TOK_GOTO:
            return parse_goto_statement(p);

        case TOK_SEMICOLON:
            // Empty statement ';'
            {
                ast_node_t *empty = ast_create(p->arena, AST_EMPTY_STMT);
                empty->loc = p->current->loc;
                parser_advance(p);
                return empty;
            }

        default:
            // Expression statement (fallthrough)
            return parse_expression_statement(p);
    }
    */
   return parse_return_statement(parser);
}

bool is_decl_specifier(token_kind_t kind) {
    switch (kind) {
        case TOK_INT:
        /*
        case TOK_CHAR:
        case TOK_FLOAT:
        case TOK_DOUBLE:
        case TOK_VOID:
        case TOK_STRUCT:
        case TOK_UNION:
        case TOK_ENUM:
        case TOK_TYPEDEF:
        case TOK_STATIC:
        case TOK_EXTERN:
        case TOK_CONST:
        case TOK_VOLATILE:
        case TOK_RESTRICT:
        case TOK_ATOMIC:
        */
            return true;
        default:
            return false;
    }
}

ast_node_t *parse_compound_statement(parser_t *parser) {
    // Create node
    ast_node_t *block = ast_node_create(AST_COMPOUND_STMT);
    //block->loc = parser->current->loc;

    // Expect '{'
    parser_expect(parser, TOK_LBRACE);

    // Parse block-item-list
    while (parser->current->kind != TOK_RBRACE &&
           parser->current->kind != TOK_EOF) {
        /*
        // Distinguish declaration vs statement
        if (is_decl_specifier(parser->current->kind)) {
            // Declaration
            decl_spec_t spec = parse_decl_specifiers(parser);
            ast_node_t *decl = parse_declaration(parser, &spec);
            ast_add_child(block, decl);
        } else {
            // Statement
            ast_node_t *stmt = parse_statement(parser);
            ast_add_child(block, stmt);
        }
        */
        ast_node_t *stmt = parse_statement(parser);
        ast_add_child(block, stmt);
    }

    // Expect '}'
    parser_expect(parser, TOK_RBRACE);

    return block;
}

ast_node_t *parse_function_definition(parser_t *parser, decl_spec_t *spec) {
    // TODO clean this up
    // 1. Create a new AST node for the function definition
    ast_node_t *fn = ast_node_create(AST_FUNCTION_DEF);
    //fn->loc = parser->current->loc; // anchor location at start of declarator

    // 2. Attach the declaration specifiers (return type, storage class, etc.)
    ast_node_t *specs_node = ast_node_create(AST_DECL_SPECIFIERS);
    //specs_node->loc = parser->current->loc;

    // Convert decl_spec_t → AST children (optional simplification)
    // TODO omit for now

    /*
    if (spec->storage_class != STORAGE_NONE) {
        ast_node_t *sc = ast_node_create(AST_STORAGE_CLASS);
        // TODO storage class goes into the data union I think
        sc->data.storage_class = spec->storage_class;
        ast_add_child(specs_node, sc);
    }
    */
    if (spec->type_spec != TYPE_NONE) {
        ast_node_t *type = ast_node_create(AST_TYPE_SPECIFIER);
        type->data.type_spec = spec->type_spec;
        ast_add_child(specs_node, type);
    }

    ast_add_child(fn, specs_node);

    // 3. Parse the function declarator (identifier + params)
    ast_node_t *declarator = parse_declarator(parser);
    ast_add_child(fn, declarator);

    // 4. (Optional) Parse declaration list (old-style parameter decls)
    // Skipped in modern C, but would go here.

    // 5. Parse the function body (compound statement)
    ast_node_t *body = parse_compound_statement(parser);
    ast_add_child(fn, body);

    return fn;
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
            default:
                Throw(FAILURE_PARSE_ERROR);
                break;
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
