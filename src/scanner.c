#include <ctype.h>
#include <string.h>
#include "include/scanner.h"
#include "include/token.h"
#include "include/exceptions.h"

static char *read_program_text(char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (NULL == fp) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (0 != fseek(fp, 0, SEEK_END)) {
        fclose(fp);
        Throw(FAILURE_FILE_IO);
    }
    long size = ftell(fp);
    if (size < 0) {
        fclose(fp);
        Throw(FAILURE_FILE_IO);
    }
    rewind(fp);
    char *buffer = malloc(size + 1);
    if (NULL == buffer) {
        fclose(fp);
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    size_t bytes_read = fread(buffer, 1, size, fp);
    fclose(fp);
    if (bytes_read != (size_t)size) {
        free(buffer);
        Throw(FAILURE_FILE_IO);
    }
    buffer[size] = '\0';
    return buffer;
}

scanner_t *scanner_create_from_text(char *text) {
    if (NULL == text) {
        Throw(FAILURE_INVALID_INPUT);
    }
    scanner_t *scanner = calloc(1, sizeof(scanner_t));
    if (NULL == scanner) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    scanner->text = strdup(text);
    if (scanner->text == NULL) {
        free(scanner);
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    scanner->line = 1;
    return scanner;
}

scanner_t *scanner_create_from_file(char *filename) {
    if (NULL == filename) {
        Throw(FAILURE_INVALID_INPUT);
    }
    char *text = read_program_text(filename);
    scanner_t *scanner = scanner_create_from_text(text);
    free(text);
    scanner->filename = strdup(filename);
    if (NULL == scanner->filename) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    return scanner;
}

void scanner_destroy(scanner_t *scanner) {
    if (NULL == scanner) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (NULL != scanner->filename) {
        free(scanner->filename);
    }
    free(scanner->text);
    free(scanner);
}

/**
 * @brief Advances the scanner to the next non-whitespace character.
 */
static void scanner_skip_whitespace(scanner_t *scanner) {
    if (NULL == scanner) {
        Throw(FAILURE_INVALID_INPUT);
    }
    while (isspace(scanner->text[scanner->idx])) {
        if (scanner->text[scanner->idx] == '\n') {
            scanner->line++;
            scanner->column = 0;
        } else {
            scanner->column++;
        }
        scanner->idx++;
    }
}

/**
 * @brief Fills the numeric token and advances scanner.
 */
static void scan_num(scanner_t *scanner, token_t *token) {
    char *endptr;
    int num = strtol(scanner->text + scanner->idx, &endptr, 10);
    token->kind = TOK_LITERAL_INT;
    token->data = calloc(1, sizeof(literal_int_data_t));
    if (NULL == token->data) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    literal_int_data_t *int_data = (literal_int_data_t *)token->data;
    int_data->val = num;
    size_t idx_diff = endptr - (scanner->text + scanner->idx);
    scanner->idx += idx_diff;
    scanner->column += idx_diff;
}

/**
 * @brief Fills the identifier token and advances scanner.
 */
static void scan_identifier(scanner_t *scanner, token_t *token) {
    size_t start = scanner->idx;
    size_t end = start;
    while (isalnum(scanner->text[end])) {
        end++;
    }
    token->kind = TOK_IDENTIFIER;
    token->data = calloc(1, sizeof(identifier_data_t));
    if (NULL == token->data) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    identifier_data_t *id_data = (identifier_data_t *)token->data;
    id_data->name = calloc(end - start + 1, sizeof(char));
    if (NULL == id_data->name) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    strncpy(id_data->name, scanner->text + start, end - start);
    size_t idx_diff = end - start;
    scanner->idx += idx_diff;
    scanner->column += idx_diff;
}

token_t *scanner_next(scanner_t *scanner) {
    if (NULL == scanner) {
        Throw(FAILURE_INVALID_INPUT);
    }
    token_t *token = calloc(1, sizeof(token_t));
    if (NULL == token) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    if (NULL != scanner->filename) {
        token->filename = strdup(scanner->filename);
        if (NULL == token->filename) {
            Throw(FAILURE_COULD_NOT_MALLOC);
        }
    }
    scanner_skip_whitespace(scanner);
    token->line = scanner->line;
    token->column = scanner->column;
    char c = scanner->text[scanner->idx];
    if (c == '\0') {
        token->kind = TOK_EOF;
    } else if (c == '(') {
        token->kind = TOK_LPAREN;
        scanner->column++;
        scanner->idx++;
    } else if (c == ')') {
        token->kind = TOK_RPAREN;
        scanner->column++;
        scanner->idx++;
    } else if (c == '{') {
        token->kind = TOK_LBRACE;
        scanner->column++;
        scanner->idx++;
    } else if (c == '}') {
        token->kind = TOK_RBRACE;
        scanner->column++;
        scanner->idx++;
    } else if (c == ';') {
        token->kind = TOK_SEMICOLON;
        scanner->column++;
        scanner->idx++;
    } else if (0 == strncmp(
            scanner->text + scanner->idx, "int", strlen("int"))) {
        token->kind = TOK_INT;
        scanner->column += strlen("int");
        scanner->idx += strlen("int");
    } else if (0 == strncmp(
            scanner->text + scanner->idx, "return", strlen("return"))) {
        token->kind = TOK_RETURN;
        scanner->column += strlen("return");
        scanner->idx += strlen("return");
    } else if (isdigit(c)) {
        scan_num(scanner, token);
    } else if (isalpha(c)) {
        scan_identifier(scanner, token);
    } else {
        Throw(FAILURE_INVALID_CHARACTER);
    }
    return token;
}

list_t *scanner_all(scanner_t *scanner) {
    if (NULL == scanner) {
        Throw(FAILURE_INVALID_INPUT);
    }
    list_t *tokens = list_create((free_function_t *)free_token, NULL);
    token_t *token = NULL;
    do {
        token = scanner_next(scanner);
        list_append(tokens, token);
    } while (token->kind != TOK_EOF);
    return tokens;
}
