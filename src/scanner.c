#include <ctype.h>
#include <string.h>
#include "include/scanner.h"
#include "include/token.h"
#include "include/exceptions.h"

/**
 * @brief Returns the numeric token and updates idx to the next unconsumed char.
 */
static token_t *scan_num(char *text, size_t *idx) {
    char *endptr;
    int num = strtol(&text[*idx], &endptr, 10);
    token_t *token = calloc(1, sizeof(token_t));
    if (NULL == token) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    token->kind = TOKEN_LITERAL_INT;
    token->data = calloc(1, sizeof(literal_int_data_t));
    if (NULL == token->data) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    literal_int_data_t *int_data = (literal_int_data_t *)token->data;
    int_data->val = num;
    *idx += (endptr - (text + *idx));
    return token;
}

/**
 * @brief Returns the identifier and updates idx to the next unconsumed char.
 */
static token_t *scan_identifier(char *text, size_t *idx) {
    size_t start = *idx;
    size_t end = start;
    while (isalnum(text[end])) {
        end++;
    }
    token_t *token = calloc(1, sizeof(token_t));
    if (NULL == token) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    token->kind = TOKEN_IDENTIFIER;
    token->data = calloc(1, sizeof(identifier_data_t));
    if (NULL == token->data) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    identifier_data_t *id_data = (identifier_data_t *)token->data;
    id_data->name = calloc(end - start + 1, sizeof(char));
    if (NULL == id_data->name) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    strncpy(id_data->name, text + start, end - start);
    *idx += end - start;
    return token;
}

list_t *scan(char *program_text) {
    if (NULL == program_text) {
        Throw(FAILURE_INVALID_INPUT);
    }
    size_t copy_buffer_len = strlen(program_text) + 1;
    char *program_text_copy = calloc(copy_buffer_len, sizeof(char));
    if (NULL == program_text_copy) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    memcpy(program_text_copy, program_text, copy_buffer_len);
    list_t *tokens = list_create((free_function_t *)free_token, NULL);
    size_t idx = 0;
    while ('\0' != program_text_copy[idx]) {
        char c = program_text_copy[idx];
        if (isspace(c)) {
            idx++;
        } else if (c == '(') {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_LEFT_PAREN;
            list_append(tokens, token);
            idx++;
        } else if (c == ')') {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_RIGHT_PAREN;
            list_append(tokens, token);
            idx++;
        } else if (c == '{') {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_LEFT_BRACE;
            list_append(tokens, token);
            idx++;
        } else if (c == '}') {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_RIGHT_BRACE;
            list_append(tokens, token);
            idx++;
        } else if (c == ';') {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_SEMICOLON;
            list_append(tokens, token);
            idx++;
        } else if (0 == strncmp(
                program_text_copy + idx, "int", strlen("int"))) {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_KEYWORD_INT;
            list_append(tokens, token);
            idx += strlen("int");
        } else if (0 == strncmp(
                program_text_copy + idx, "return", strlen("return"))) {
            token_t *token = calloc(1, sizeof(token_t));
            token->kind = TOKEN_KEYWORD_RETURN;
            list_append(tokens, token);
            idx += strlen("return");
        } else if (isdigit(c)) {
            token_t *token = scan_num(program_text_copy, &idx);
            list_append(tokens, token);
        } else if (isalpha(c)) {
            token_t *token = scan_identifier(program_text_copy, &idx);
            list_append(tokens, token);
        } else {
            Throw(FAILURE_INVALID_CHARACTER);
        }
    }
    free(program_text_copy);
    return tokens;
}

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

token_t *scanner_next(scanner_t *scanner) {
    // TODO logic from scan
    Throw(FAILURE_NOT_IMPLEMENTED);
    return NULL;
}
