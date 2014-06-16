/**
 *  @file       ei_linkedlist.h
 *  @brief      Doubly linked list generic class.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 07.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#ifndef EI_LINKEDLIST_H
#define EI_LINKEDLIST_H

#include "ei_common.h"
#include "ei_types.h"


/**
 * @brief       The stored element's pointer.
 */
typedef void* ei_elem_t;

/**
 * @brief       An element and previous / next pointers to create a doubly linked list.
 */
typedef struct ei_linked_elem_t {
        ei_elem_t elem;
        struct ei_linked_elem_t *next;
        struct ei_linked_elem_t *prev;
} ei_linked_elem_t;

/**
 * @brief       A generic linked list type.
 */
typedef struct ei_linked_list_t {
        ei_linked_elem_t *head;
        ei_linked_elem_t *tail;
} ei_linkedlist_t;


/**
 * \brief       ei_linkedlist_t callback function.
 *              Useful to automatically call a function on each list element.
 *
 * @param       link            Linked list element
 * @param       user_param      User parameters
 *
 * @return                      A boolean telling if this callback must stop being called on other list elements.
 *                              If EI_TRUE, the callback will not be called on them, if EI_FALSE it will.
 */
typedef ei_bool_t       (*ei_function_t) (ei_linked_elem_t *link, void *user_param);

/**
 * \brief       Initializes a list.
 *
 * @param       list            List to initialize
 */
void ei_linkedlist_init(ei_linkedlist_t *list);

/**
 * \brief       Adds an element to a list.
 *
 * @param       list            The list
 * @param       elem            The element to add
 */
void ei_linkedlist_add(ei_linkedlist_t *list, ei_elem_t elem);

/**
 * \brief       Adds an element to a list, but only if it is not already in it.
 *
 * @param       list            The list
 * @param       elem            The element to add
 */
void ei_linkedlist_add_unique(ei_linkedlist_t *list, ei_elem_t elem);

/**
 * \brief       Removes an element from a list.
 *
 * @param       list            The list
 * @param       elem            The element to remove
 * @param       free_elem       Free the contained element ?
 */
void ei_linkedlist_pop_elem(ei_linkedlist_t *list, ei_elem_t elem, ei_bool_t free_elem);

/**
 * \brief       Removes a linked element from a list.
 *
 * @param       list            The list
 * @param       link            The linked element to remove
 * @param       free_elem       Free the contained element ?
 */
void ei_linkedlist_pop_link(ei_linkedlist_t *list, ei_linked_elem_t *link, ei_bool_t free_elem);

/**
 * \brief       Empties a list.
 *
 * @param       list            The list.
 * @param       free_elem       Free the contained element ?
 */
void ei_linkedlist_empty(ei_linkedlist_t *list, ei_bool_t free_elem);

/**
 * \brief       Indicates if a list contains a given element.
 *
 * @param       list            The list.
 * @param       elem            The element.
 */
ei_bool_t ei_linkedlist_has(ei_linkedlist_t *list, ei_elem_t elem);

/**
 * \brief       Applies a function to each element of a list, stops if the function returns EI_TRUE.
 *              Continues as long as it returns EI_FALSE.
 *
 * @param       list            The list.
 * @param       function        The element.
 * @param       user_param      User parameters
 */
void ei_linkedlist_applyfunc(ei_linkedlist_t *list, ei_function_t function, void *user_param);


#endif
