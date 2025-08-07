/**
 * @brief A circularly doubly linked list data structure.
 */

#ifndef INCLUDE_LIST_H_
#define INCLUDE_LIST_H_

/**
 * @brief A doubly linked list node.
 * 
 * @param data A generic pointer to data the user has stored in the node.
 * @param next A pointer to the next link in the node.
 * @param prev A pointer to the previous link in the node.
 */
typedef struct node_t {
    void *data;
    struct node_t *next;
    struct node_t *prev;
} node_t;

/**
 * @brief A user-defined function that will free the data in a node.
 */
typedef void (free_function_t(void *data));

/**
 * @brief A user-defined function that will compare two elements of data.
 * 
 * The compare function should return a negative number if data1 < data2, a
 * positive number if data1 > data2, and zero if data1 and data2 are equal. If
 * data1 and data2 point to integers, one valid compare function would return
 * *data1 minus *data2.
 */
typedef int (compare_function_t(void *data1, void *data2));

/**
 * @brief A circular doubly linked list.
 * 
 * @param free_function A user-defined function that will free node data.
 * @param compare_function A user-defined function that will compare node data.
 * May be NULL.
 * @param head The first node in the linked list. If the list is empty, head
 * will be NULL.
 */
typedef struct list_t {
    free_function_t *free_function;
    compare_function_t *compare_function;
    node_t *head;
} list_t;

/**
 * @brief Returns a pointer to the newly allocated list.
 * 
 * @param free_function A user-defined function that will free node data.
 * @param compare_function A user-defined function that will compare node data.
 * May be NULL.
 */
list_t *list_create(
    free_function_t *free_function, compare_function_t *compare_function);

/**
 * @brief Frees all memory associated with the list.
 */
void list_destroy(list_t *list);

/**
 * @brief Returns true if the list is empty, false otherwise.
 */
bool list_is_empty(list_t *list);

/**
 * @brief Returns the number of nodes in the list.
 */
size_t list_length(list_t *list);

/**
 * @brief Prepends a node containing the given data to the list.
 * 
 * @param data The data to place in the new node. The user should already have
 * allocated this memory.
 */
void list_prepend(list_t *list, void *data);

/**
 * @brief Removes the first node from the list and frees its memory.
 */
void list_remove_head(list_t *list);

#endif  // INCLUDE_LIST_H_
