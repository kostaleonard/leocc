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
    long size = ftell(fp);  // NOLINT(runtime/int)
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
    scanner->loc.line = 1;
    return scanner;
}

scanner_t *scanner_create_from_file(char *filename) {
    if (NULL == filename) {
        Throw(FAILURE_INVALID_INPUT);
    }
    char *text = read_program_text(filename);
    scanner_t *scanner = scanner_create_from_text(text);
    free(text);
    scanner->loc.filename = strdup(filename);
    if (NULL == scanner->loc.filename) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    return scanner;
}

void scanner_destroy(scanner_t *scanner) {
    if (NULL == scanner) {
        Throw(FAILURE_INVALID_INPUT);
    }
    if (NULL != scanner->loc.filename) {
        free(scanner->loc.filename);
    }
    free(scanner->text);
    free(scanner);
}

/**
 * @brief Advances the scanner to the next non-whitespace, non-comment char.
 */
static void scanner_skip_whitespace_and_comments(scanner_t *scanner) {
    if (NULL == scanner) {
        Throw(FAILURE_INVALID_INPUT);
    }
    while (true) {
        // Skip one space character.
        if (isspace(scanner->text[scanner->idx])) {
            if (scanner->text[scanner->idx] == '\n') {
                scanner->loc.line++;
                scanner->loc.column = 0;
            } else {
                scanner->loc.column++;
            }
            scanner->idx++;
            continue;
        }
        // Skip single-line comment.
        if (0 == strncmp(&scanner->text[scanner->idx], "//", 2)) {
            while (scanner->text[scanner->idx] != '\0' &&
                   scanner->text[scanner->idx] != '\n') {
                scanner->loc.column++;
                scanner->idx++;
            }
            continue;
        }
        // Skip block comment.
        if (0 == strncmp(&scanner->text[scanner->idx], "/*", 2)) {
            while (scanner->text[scanner->idx] != '\0' &&
                   0 != strncmp(&scanner->text[scanner->idx], "*/", 2)) {
                scanner->loc.column++;
                scanner->idx++;
            }
            if ('\0' == scanner->text[scanner->idx]) {
                fprintf(
                    stderr,
                    "%s:%lld:%lld: error: Reached EOF before */\n",
                    scanner->loc.filename,
                    scanner->loc.line,
                    scanner->loc.column);
                Throw(FAILURE_SCANNER_ERROR);
            }
            // Skip */
            scanner->loc.column += 2;
            scanner->idx += 2;
            continue;
        }
        // We have encountered a non-whitespace, non-comment char.
        break;
    }
}

/**
 * @brief Fills the numeric token and advances scanner.
 */
static void scan_num(scanner_t *scanner, token_t *token) {
    if (NULL == scanner || NULL == token) {
        Throw(FAILURE_INVALID_INPUT);
    }
    char *endptr;
    int num = strtol(scanner->text + scanner->idx, &endptr, 10);
    token->kind = TOK_INT_LITERAL;
    token->data.int_value = num;
    size_t idx_diff = endptr - (scanner->text + scanner->idx);
    scanner->idx += idx_diff;
    scanner->loc.column += idx_diff;
}

/**
 * @brief Fills the keyword or identifier token and advances scanner.
 */
static void scan_keyword_or_identifier(scanner_t *scanner, token_t *token) {
    if (NULL == scanner ||
        NULL == token ||
        isdigit(scanner->text[scanner->idx])) {
        Throw(FAILURE_INVALID_INPUT);
    }
    size_t start = scanner->idx;
    size_t end = start;
    while (isalnum(scanner->text[end]) || '_' == scanner->text[end]) {
        end++;
    }
    size_t len = end - start;
    // Check if we have a keyword.
    if (3 == len && 0 == strncmp(scanner->text + start, "int", len)) {
        token->kind = TOK_INT;
        scanner->loc.column += len;
        scanner->idx += len;
    } else if (6 == len && 0 == strncmp(scanner->text + start, "return", len)) {
        token->kind = TOK_RETURN;
        scanner->loc.column += len;
        scanner->idx += len;
    } else {
        // If it's not a keyword, then it's an identifier.
        token->kind = TOK_IDENTIFIER;
        token->data.ident = calloc(len + 1, sizeof(char));
        if (NULL == token->data.ident) {
            Throw(FAILURE_COULD_NOT_MALLOC);
        }
        strncpy(token->data.ident, scanner->text + start, len);
        scanner->idx += len;
        scanner->loc.column += len;
    }
}

token_t *scanner_next(scanner_t *scanner) {
    if (NULL == scanner) {
        Throw(FAILURE_INVALID_INPUT);
    }
    scanner_skip_whitespace_and_comments(scanner);
    token_t *token = calloc(1, sizeof(token_t));
    if (NULL == token) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    token->loc = source_loc_dup(scanner->loc);
    char c = scanner->text[scanner->idx];
    if ('\0' == c) {
        token->kind = TOK_EOF;
    } else if ('(' == c) {
        token->kind = TOK_LPAREN;
        scanner->loc.column++;
        scanner->idx++;
    } else if (')' == c) {
        token->kind = TOK_RPAREN;
        scanner->loc.column++;
        scanner->idx++;
    } else if ('{' == c) {
        token->kind = TOK_LBRACE;
        scanner->loc.column++;
        scanner->idx++;
    } else if ('}' == c) {
        token->kind = TOK_RBRACE;
        scanner->loc.column++;
        scanner->idx++;
    } else if (';' == c) {
        token->kind = TOK_SEMICOLON;
        scanner->loc.column++;
        scanner->idx++;
    } else if ('=' == c) {
        token->kind = TOK_EQ;
        scanner->loc.column++;
        scanner->idx++;
    } else if (isdigit(c)) {
        scan_num(scanner, token);
    } else if (isalpha(c) || '_' == c) {
        scan_keyword_or_identifier(scanner, token);
    } else {
        fprintf(
            stderr,
            "%s:%lld:%lld: error: Unexpected scan char: %c\n",
            scanner->loc.filename,
            scanner->loc.line,
            scanner->loc.column,
            c);
        Throw(FAILURE_INVALID_CHARACTER);
    }
    return token;
}

list_t *scanner_all(scanner_t *scanner) {
    if (NULL == scanner) {
        Throw(FAILURE_INVALID_INPUT);
    }
    list_t *tokens = list_create((free_function_t *)token_destroy, NULL);
    token_t *token = NULL;
    do {
        token = scanner_next(scanner);
        list_append(tokens, token);
    } while (token->kind != TOK_EOF);
    return tokens;
}
