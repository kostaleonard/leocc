/**
 * @brief Contains definitions and functions for tokens in the C language.
 */

#ifndef INCLUDE_TOKEN_H_
#define INCLUDE_TOKEN_H_

// TODO make issue--add sample buggy programs to test compiler error output.

// TODO eventually we should sort these tokens to be in a more logical order. I have them in the order they appear in our sample programs.
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

// TODO how do we think about token_create function(s)?
typedef struct token_t {
    token_code_t kind;
    // TODO add these fields for error output?
    //char *filename;
    //size_t line;
    //size_t column;
    void *data;
} token_t;

typedef struct identifier_data_t {
    char *name;
} identifier_data_t;

typedef struct literal_int_data_t {
    int val;
} literal_int_data_t;

#endif  // INCLUDE_TOKEN_H_
