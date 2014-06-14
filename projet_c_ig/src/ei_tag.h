/**
 *  @file	ei_tag.h
 *  @brief	Tag API
 *
 *  \author 
 *  Created by Léo GOUTTEFARDE on 14.06.14
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */
#ifndef EI_TAG_H
#define EI_TAG_H

#include "ei_common.h"
#include "ei_linkedlist.h"
#include "ei_widget.h"
#include "ei_event.h"


typedef struct ei_tag_widgets_t {
        ei_tag_t name;
        ei_linkedlist_t widgets;
} ei_tag_widgets_t;

typedef struct ei_tag_params_t {
        ei_widget_t *widget;
        ei_tag_t tag_name;
        ei_bool_t result;
} ei_tag_params_t;



void ei_tag_create(ei_tag_t tag_name);

void ei_tag_destroy(ei_tag_t tag_name);


void ei_tag(ei_widget_t *widget, ei_tag_t tag_name);

void ei_untag(ei_widget_t *widget, ei_tag_t tag_name);


ei_bool_t ei_has_tag(ei_widget_t *widget, ei_tag_t tag_name);

void ei_tag_free();


#endif
