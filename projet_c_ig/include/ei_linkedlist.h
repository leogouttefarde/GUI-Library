/**
 *  @file	ei_linkedlist.h
 *  @brief	Linked list generic class.
 *
 *  \author 
 *  Created by Léo Gouttefarde on 07.07.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */
#ifndef EI_LINKEDLIST_H
#define EI_LINKEDLIST_H

#include "ei_common.h"
#include "ei_types.h"


/**
 * @brief	The stored element's pointer.
 */
typedef void* ei_elem_t;

/**
 * @brief	An element and a pointer to create a linked list.
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


void ei_linkedlist_init(ei_linkedlist_t *list);

void ei_linkedlist_add(ei_linkedlist_t *list, ei_elem_t elem);

// void ei_linkedlist_pop_elem(ei_linkedlist_t *list, ei_elem_t elem);

void ei_linkedlist_pop_link(ei_linkedlist_t *list, ei_linked_elem_t *link, ei_bool_t free_elem);

// ei_elem_t ei_linkedlist_pop(ei_linkedlist_t *list);

void ei_linkedlist_empty(ei_linkedlist_t *list, ei_bool_t free_elem);

ei_bool_t ei_linkedlist_has(ei_linkedlist_t *list, ei_elem_t elem);

void ei_linkedlist_add_unique(ei_linkedlist_t *list, ei_elem_t elem);


#endif
