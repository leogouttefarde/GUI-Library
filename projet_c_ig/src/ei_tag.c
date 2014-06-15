/**
 *  @file	ei_tag.c
 *  @brief	Tag API
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 14.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#include "ei_tag.h"
#include "ei_utilities.h"



ei_linkedlist_t tags = { NULL, NULL };



void ei_tag_create(ei_tag_t tag_name)
{
        ei_tag_widgets_t *tag = CALLOC_TYPE(ei_tag_widgets_t);

        ei_linkedlist_init(&tag->widgets);
        make_string_copy(&tag->name, tag_name);

        ei_linkedlist_add(&tags, tag);
}

// Requires link != NULL
ei_bool_t ei_tag_destroy_link(ei_linked_elem_t *link, void *user_param)
{
        ei_bool_t done = EI_FALSE;
        ei_tag_t tag_name = (ei_tag_t)user_param;
        ei_tag_widgets_t *tag = (ei_tag_widgets_t*)link->elem;

        if (    (tag_name && tag && tag->name && !strcmp(tag->name, tag_name))
                || (tag_name == NULL)) {
                ei_linkedlist_empty(&tag->widgets, EI_FALSE);
                SAFE_FREE(tag->name);

                ei_linkedlist_pop_link(&tags, link, EI_TRUE);

                if (tag_name)
                        done = EI_TRUE;
        }

        return done;
}

void ei_tag_destroy(ei_tag_t tag_name)
{
        ei_linkedlist_applyfunc(&tags, ei_tag_destroy_link, (void*)tag_name);
}

void ei_tag_free()
{
        ei_linkedlist_applyfunc(&tags, ei_tag_destroy_link, NULL);
}

ei_bool_t ei_tag_link(ei_linked_elem_t *link, void *user_param)
{
        ei_bool_t done = EI_FALSE;

        if (user_param) {
                ei_tag_params_t *params = (ei_tag_params_t*)user_param;
                ei_widget_t *widget = params->widget;
                ei_tag_widgets_t *tag = (ei_tag_widgets_t*)link->elem;

                if (widget && tag && !strcmp(tag->name, params->tag_name))
                        ei_linkedlist_add(&tag->widgets, widget), done = EI_TRUE;
        }

        return done;
}

void ei_tag(ei_widget_t *widget, ei_tag_t tag_name)
{
        ei_tag_params_t params = { widget, tag_name, EI_FALSE };
        ei_linkedlist_applyfunc(&tags, ei_tag_link, (void*)&params);
}

ei_bool_t ei_untag_link(ei_linked_elem_t *link, void *user_param)
{
        ei_bool_t done = EI_FALSE;

        if (user_param) {
                ei_tag_params_t *params = (ei_tag_params_t*)user_param;
                ei_widget_t *widget = params->widget;
                ei_tag_widgets_t *tag = (ei_tag_widgets_t*)link->elem;

                if (widget && tag && !strcmp(tag->name, params->tag_name))
                        ei_linkedlist_pop_elem(&tag->widgets, (ei_elem_t)widget, EI_FALSE), done = EI_TRUE;
        }

        return done;
}

void ei_untag(ei_widget_t *widget, ei_tag_t tag_name)
{
        ei_tag_params_t params = { widget, tag_name, EI_FALSE };
        ei_linkedlist_applyfunc(&tags, ei_untag_link, (void*)&params);
}

ei_bool_t ei_has_tag_link(ei_linked_elem_t *link, void *user_param)
{
        ei_bool_t done = EI_FALSE;

        ei_tag_params_t *params = (ei_tag_params_t*)user_param;
        ei_tag_widgets_t *tag = (ei_tag_widgets_t*)link->elem;

        if (tag && params && !strcmp(tag->name, params->tag_name)) {
                done = EI_TRUE;

                if (ei_linkedlist_has(&tag->widgets, (ei_elem_t)params->widget))
                        params->result = EI_TRUE;
        }

        return done;
}

ei_bool_t ei_has_tag(ei_widget_t *widget, ei_tag_t tag_name)
{
        ei_tag_params_t params = { widget, tag_name, EI_FALSE };

        ei_linkedlist_applyfunc(&tags, ei_has_tag_link, (void*)&params);

        return params.result;
}


