/**
 * @brief Contains definitions and functions for tokens in the C language.
 */

#ifndef INCLUDE_TOKEN_H_
#define INCLUDE_TOKEN_H_
#include <stddef.h>
#include <stdint.h>

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

typedef struct source_loc_t {
    char *filename;
    size_t line;
    size_t column;
} source_loc_t;

typedef union token_data_t {
    int64_t int_value;
    double float_value;
    char *string_value;
    char *ident;
} token_data_t;

/**
 * @brief Represents a token in the C language.
 * 
 * @param kind A code indicating the kind of token. This code tells the user how
 * to interpret (i.e., cast) the data pointer.
 * @param loc The token's file and position within that file.
 * @param data A union containing data describing the token. For example, if the
 * token was a literal int, the data could be read as an int to retrieve the
 * literal int value. Some token kinds do not have data, such as keyword tokens.
 * In this case, the data field is unused and can be set to 0 (or any other
 * value, since it will not be used).
 */
typedef struct token_t {
    token_kind_t kind;
    source_loc_t loc;
    token_data_t data;
} token_t;

/**
 * @brief Frees token and all memory associated with it.
 * 
 * If token is NULL, the function does nothing.
 */
void token_destroy(token_t *token);

/**
 * @brief Copies the source_loc_t, duplicating string pointers.
 */
source_loc_t source_loc_dup(source_loc_t loc);

#endif  // INCLUDE_TOKEN_H_
