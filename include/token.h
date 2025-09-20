/**
 * @brief Contains definitions and functions for tokens in the C language.
 */

#ifndef INCLUDE_TOKEN_H_
#define INCLUDE_TOKEN_H_

typedef enum token_code_t {
    TOKEN_KEYWORD_INT,
    TOKEN_IDENTIFIER,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_KEYWORD_RETURN,
    TOKEN_LITERAL_INT,
    TOKEN_SEMICOLON,
    TOKEN_RIGHT_BRACE,
} token_code_t;

typedef struct token_t {
    token_code_t kind;
    void *data;
} token_t;

typedef struct identifier_data_t {
    char *name;
} identifier_data_t;

typedef struct literal_int_data_t {
    int val;
} literal_int_data_t;

/**
 * @brief Frees token and all memory associated with it.
 * 
 * If token is NULL, the function does nothing.
 */
void free_token(token_t *token);

#endif  // INCLUDE_TOKEN_H_
