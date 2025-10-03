#include "include/parser.h"
#include "include/exceptions.h"

static void free_declaration(declaration_t *declaration) {
    // TODO see free_token, maybe some recursive free thing or something.
}

/**
 * @brief Parses one declaration_t from the tokens.
 * 
 * @param current_node The current node in the token list. This function will
 * update the pointer to the next unconsumed node after parsing.
 * @param num_tokens_remaining The number of tokens remaining in the list. This
 * function will deduct from this the number of tokens consumed after parsing.
 * If it would fall below zero during parsing, this function will throw an
 * error.
 */
static declaration_t *parse_declaration(
    node_t **current_node,
    size_t *num_tokens_remaining) {
    
    // TODO
    return NULL;
}

ast_t *parse(list_t *tokens) {
    if (NULL == tokens) {
        Throw(FAILURE_INVALID_INPUT);
    }
    ast_t *ast = malloc(sizeof(ast_t));
    if (NULL == ast) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    ast->root = malloc(sizeof(translation_unit_t));
    if (NULL == ast->root) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    ast->root->declarations = list_create(
        (free_function_t *)free_declaration, NULL);
    node_t *current_node = tokens->head;
    size_t num_tokens_remaining = list_length(tokens);
    while (num_tokens_remaining > 0) {
        declaration_t *declaration = parse_declaration(
            &current_node, &num_tokens_remaining);
        list_append(ast->root->declarations, declaration);
    }
    return ast;
}

// TODO maybe we also have an ast_create, but likely the only thing that will create an AST is the parser. So we may not need an interface for that.
void ast_destroy(ast_t *ast) {
    if (NULL == ast) {
        Throw(FAILURE_INVALID_INPUT);
    }
    list_destroy(ast->root->declarations);
    free(ast->root);
    free(ast);
}
