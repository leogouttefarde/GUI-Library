/**
 *  @file       ei_linkedlist.c
 *
 *  \author 
 *  Created by Léo Gouttefarde on 07.06.14
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#include "ei_linkedlist.h"
#include "ei_common.h"


void ei_linkedlist_init(ei_linkedlist_t *list)
{
        SAFE_RESET(list, sizeof(ei_linkedlist_t));
}

void ei_linkedlist_add(ei_linkedlist_t *list, ei_elem_t elem)
{
        if (elem && list) {
                ei_linked_elem_t *link = CALLOC_TYPE(ei_linked_elem_t);
                link->elem = elem;
                link->next = NULL;

                if (list->tail) {
                        link->prev = list->tail;
                        list->tail->next = link;
                        list->tail = link;
                }
                else {
                        list->head = link;
                        list->tail = link;
                }
        }
}

ei_bool_t ei_linkedlist_has(ei_linkedlist_t *list, ei_elem_t elem)
{
        ei_bool_t has = EI_FALSE;

        if (list) {
                ei_linked_elem_t *link = list->head;

                while (link && !has) {
                        if (elem == link->elem)
                                has = EI_TRUE;

                        link = link->next;
                }
        }

        return has;
}

void ei_linkedlist_add_unique(ei_linkedlist_t *list, ei_elem_t elem)
{
        if (!ei_linkedlist_has(list, elem))
                ei_linkedlist_add(list, elem);
}

typedef struct ei_linkedlist_pop_elem_t {
        ei_linkedlist_t *list;
        ei_elem_t elem;
        ei_bool_t free_elem;
} ei_linkedlist_pop_elem_t;

ei_bool_t ei_linkedlist_pop_elem_link(ei_linked_elem_t *link, void *user_param)
{
        ei_bool_t done = EI_FALSE;

        ei_linkedlist_pop_elem_t *params = (ei_linkedlist_pop_elem_t*)user_param;
        if (link->elem == params->elem) {
                ei_linkedlist_pop_link(params->list, link, params->free_elem);
                done = EI_TRUE;
        }

        return done;
}

void ei_linkedlist_pop_elem(ei_linkedlist_t *list, ei_elem_t elem, ei_bool_t free_elem)
{
        ei_linkedlist_pop_elem_t params = { list, elem, free_elem };
        ei_linkedlist_applyfunc(list, ei_linkedlist_pop_elem_link, (void*)&params);
}

void ei_linkedlist_pop_link(ei_linkedlist_t *list, ei_linked_elem_t *link, ei_bool_t free_elem)
{
        if (list && link) {
                ei_linked_elem_t *prev = link->prev;
                ei_linked_elem_t *next = link->next;


                // Random insider elem
                if (prev && next) {
                        prev->next = next;
                        next->prev = prev;
                }

                // Head
                else if (!prev && next) {

                        // Check
                        if (list->head == link) {
                                next->prev = prev;
                                list->head = next;
                        }
                }

                // Tail
                else if (prev && !next) {

                        // Check
                        if (list->tail == link) {
                                prev->next = NULL;
                                list->tail = prev;
                        }
                }

                // Both head and tail : unique elem
                else {
                        list->head = NULL;
                        list->tail = NULL;
                }

                // If unique element, remove all links
                if (list->head && (list->head == list->tail)) {
                        list->head->next = NULL;
                        list->head->prev = NULL;
                }

                if (free_elem)
                        SAFE_FREE(link->elem);

                SAFE_FREE(link);
        }
}

void ei_linkedlist_empty(ei_linkedlist_t *list, ei_bool_t free_elem)
{
        if (list) {
                ei_linked_elem_t *link = list->head, *next = NULL;

                while (link) {
                        if (free_elem)
                                SAFE_FREE(link->elem);

                        next = link->next;
                        SAFE_FREE(link);
                        link = next;
                }

                list->head = NULL;
                list->tail = NULL;
        }
}

void ei_linkedlist_applyfunc(ei_linkedlist_t *list, ei_function_t function, void *user_param)
{
        if (list) {
                ei_bool_t done = EI_FALSE;
                ei_linked_elem_t *link = list->head, *next = NULL;

                while (link && !done) {
                        next = link->next;

                        done = function(link, user_param);

                        link = next;
                }
        }
}


