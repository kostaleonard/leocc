/**
 * @brief A circularly doubly linked list data structure.
 */

#ifndef INCLUDE_LIST_H_
#define INCLUDE_LIST_H_
#include <stdbool.h>

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
 * @brief Removes all nodes from the list, but does not delete the list.
 */
void list_clear(list_t *list);

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
 * @brief Appends a node containing the given data to the list.
 * 
 * @param data The data to place in the new node. The user should already have
 * allocated this memory.
 */
void list_append(list_t *list, void *data);

/**
 * @brief Removes the first node from the list and frees its memory.
 */
void list_remove_head(list_t *list);

/**
 * @brief Removes the last node from the list and frees its memory.
 */
void list_remove_tail(list_t *list);

/**
 * @brief Removes the node from the list in which it is a member.
 * 
 * @param node A node in a list. If node is not a member of list, the behavior
 * of this function is undefined.
 */
void list_remove_node(list_t *list, node_t *node);

/**
 * @brief Removes the node at index from the list.
 * 
 * @param idx The index of the node to remove, where index 0 is the head.
 * Negative indices are allowed. Index -1 is the tail of the list. This function
 * will throw an error if the given index exceeds the number of elements in the
 * list in either positive or negative direction.
 */
void list_remove_at(list_t *list, int idx);

/**
 * @brief Returns the first node whose data matches the given data.
 * 
 * @param data The data with which to compare each node's data.
 * @return node_t* The first node whose data matches the given data, or NULL if
 * no match exists. Matches are determined using the list's compare_function.
 */
node_t *list_find(list_t *list, void *data);

/**
 * @brief Returns the number of nodes whose data matches the given data.
 * 
 * @param data The data with which to compare each node's data.
 * @return size_t The number of nodes whose data matches the given data. Matches
 * are determined using the list's compare_function.
 */
size_t list_count(list_t *list, void *data);

/**
 * @brief Sorts the list based on its compare_function.
 * 
 * If the list is empty, this function does nothing. If the list's
 * compare_function is NULL, this function throws an error.
 */
void list_sort(list_t *list);

/**
 * @brief Applies the function to every node in the list.
 */
void list_foreach(list_t *list, void (*func)(node_t *));

#endif  // INCLUDE_LIST_H_
