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
    token_code_t kind;
    char *filename;
    size_t line;
    size_t column;
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
