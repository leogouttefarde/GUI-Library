/**
 *  @file	ei_linkedlist.c
 *  @brief	Linked list generic class.
 *
 *  \author 
 *  Created by Léo Gouttefarde on 07.07.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#include "ei_linkedlist.h"
#include "ei_application.h"
#include "ei_common.h"


void ei_linkedlist_init(ei_linkedlist_t *list)
{
	SAFE_RESET(list, sizeof(ei_linkedlist_t));
}

void ei_linkedlist_add(ei_linkedlist_t *list, ei_elem_t elem)
{
	if (elem && list) {
		ei_linked_elem_t *link = malloc(sizeof(ei_linked_elem_t));
		link->elem = elem;
		link->next = NULL;

		if (list->tail) {
			list->tail->next = link;
			list->tail = link;
		}
		else {
			list->head = link;
			list->tail = link;
		}
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

