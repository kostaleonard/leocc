/**
 * @brief The scanner takes a program text as input and outputs the tokens.
 */

// TODO list_t: a circular doubly linked list (just for simplicity)

/**
 * @brief Returns a list of the tokens that compose the program.
 * 
 * @param program_text The preprocessed program text.
 * @param program_text_len The number of characters in the program text.
 * @return list_t* A list of the tokens that compose the program. Each element
 * is a token_t. Callers must free.
 */
list_t *scan(char *program_text, size_t program_text_len);
