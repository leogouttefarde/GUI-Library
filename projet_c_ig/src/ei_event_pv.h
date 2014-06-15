/**
 *  @file       ei_event_pv.h
 *  @brief      Private event declarations
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 12.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 */

#ifndef EI_EVENT_PV_H
#define EI_EVENT_PV_H

#include "ei_event.h"
#include "ei_linkedlist.h"


/* Event binding */
typedef struct ei_binding_t {
        ei_tag_t tag;
        ei_widget_t *widget;
        ei_callback_t callback;
        void *user_param;
} ei_binding_t;


void ei_event_init();

void ei_event_process(ei_event_t *event);

void ei_unbind_link(ei_linkedlist_t *list, ei_linked_elem_t *link);

void ei_unbind_all();


#endif
