/**
 * @brief A dynamic string buffer designed for repeated append operations.
 */

#ifndef INCLUDE_STRING_BUILDER_H_
#define INCLUDE_STRING_BUILDER_H_
#include <stddef.h>

/**
 * @brief A dynamic string buffer.
 * 
 * @param data The string being built.
 * @param length The current length of the string. Does not include the null
 * terminator.
 * @param capacity The number of bytes allocated for the string.
 */
typedef struct string_builder_t {
    char *data;
    size_t length;
    size_t capacity;
} string_builder_t;

/**
 * @brief Returns a new string builder.
 * 
 * The string builder will have a small initial capacity and an empty string as
 * data.
 */
string_builder_t *string_builder_create();

/**
 * @brief Appends the input to the builder's string.
 * 
 * If this operation would cause length to exceed capacity, capacity is doubled
 * and data is realloc'd.
 * 
 * @param s The string to append.
 */
void string_builder_append(char *s);

/**
 * @brief Frees all memory associated with the string builder.
 */
void string_builder_destroy(string_builder_t *sb);

#endif  // INCLUDE_STRING_BUILDER_H_
