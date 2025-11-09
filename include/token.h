/**
 * @brief Contains definitions and functions for tokens in the C language.
 */

#ifndef INCLUDE_TOKEN_H_
#define INCLUDE_TOKEN_H_
#include <stddef.h>

typedef enum token_kind_t {
    TOK_INT,
    TOK_IDENTIFIER,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RETURN,
    TOK_INT_LITERAL,
    TOK_SEMICOLON,
    TOK_RBRACE,
    TOK_EOF,
} token_kind_t;

/**
 * @brief Represents a token in the C language.
 * 
 * @param kind A code indicating the kind of token. This code tells the user how
 * to interpret (i.e., cast) the data pointer.
 * @param filename The name of the file in which the token appears.
 * @param line The line number on which the token appears in its file.
 * @param column The column on which the first character of the token appears in
 * its file.
 * @param data A pointer to other data describing the token. For example, if the
 * token was a literal int, the pointer would point to a data structure
 * containing this value (and potentially other relevant information).
 */
typedef struct token_t {
    token_kind_t kind;
    char *filename;
    size_t line;
    size_t column;
    union {
        long int_value;       // for integer constants
        double float_value;   // for floating-point constants
        char *string_value;   // for string or char literals (malloc’d)
        char *ident;          // for identifiers
    } data;
} token_t;

/**
 * @brief Frees token and all memory associated with it.
 * 
 * If token is NULL, the function does nothing.
 */
void free_token(token_t *token);
// TODO rename to token_destroy for consistency

#endif  // INCLUDE_TOKEN_H_
