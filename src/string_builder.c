#include <stdlib.h>
#include <string.h>
#include "include/string_builder.h"
#include "include/exceptions.h"

#define INITIAL_CAPACITY 64

string_builder_t *string_builder_create() {
    string_builder_t *sb = calloc(1, sizeof(string_builder_t));
    if (NULL == sb) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    sb->capacity = INITIAL_CAPACITY;
    sb->data = calloc(INITIAL_CAPACITY, sizeof(char));
    if (NULL == sb->data) {
        Throw(FAILURE_COULD_NOT_MALLOC);
    }
    return sb;
}

void string_builder_append(string_builder_t *sb, char *s) {
    if (NULL == sb || NULL == s) {
        Throw(FAILURE_INVALID_INPUT);
    }
    size_t s_len = strlen(s);
    size_t old_capacity = sb->capacity;
    while (sb->length + s_len >= sb->capacity) {
        sb->capacity *= 2;
    }
    if (sb->capacity > old_capacity) {
        sb->data = realloc(sb->data, sb->capacity);
        if (NULL == sb->data) {
            Throw(FAILURE_COULD_NOT_MALLOC);
        }
    }
    strncpy(sb->data + sb->length, s, s_len);
    sb->length += s_len;
    sb->data[sb->length] = '\0';
}

void string_builder_destroy(string_builder_t *sb) {
    if (NULL == sb) {
        Throw(FAILURE_INVALID_INPUT);
    }
    free(sb->data);
    free(sb);
}
