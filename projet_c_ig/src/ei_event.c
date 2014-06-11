/**
 *  @file       ei_event.c
 *  @brief      Allows the binding and unbinding of callbacks to events.
 *
 *  \author 
 *  Created by Léo Gouttefarde on 04.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#include <stdlib.h>
#include <string.h>

#include "ei_event.h"
#include "ei_core.h"
#include "ei_linkedlist.h"


/* Event binding */
typedef struct ei_binding_t {
        ei_tag_t tag;
        ei_widget_t *widget;
        ei_callback_t callback;
        void *user_param;
} ei_binding_t;

/* Event binding table */
ei_linkedlist_t ei_events[ei_ev_last];


void ei_event_init()
{
        for (uint32_t i = 0; i < ei_ev_last; ++i)
                ei_linkedlist_init(&ei_events[i]);
}

/**
 * \brief       Binds a callback to an event type and a widget or a tag.
 *
 * @param       eventtype       The type of the event.
 * @param       widget          The callback is only called if the event is related to this widget.
 *                              This parameter must be NULL if the "tag" parameter is not NULL.
 * @param       tag             The callback is only called if the event is related to a widget that
 *                              has this tag. A tag can be a widget class name, or the tag "all".
 *                              This parameter must be NULL is the "widget" parameter is not NULL.
 * @param       callback        The callback (i.e. the function to call).
 * @param       user_param      A user parameter that will be passed to the callback when it is
 *                              called.
 */
void ei_bind(ei_eventtype_t eventtype,
                ei_widget_t *widget,
                ei_tag_t tag,
                ei_callback_t callback,
                void *user_param)
{
        if (eventtype < ei_ev_last) {
                ei_binding_t *binding = CALLOC_TYPE(ei_binding_t);
                assert(binding);

                binding->widget = widget;
                binding->callback = callback;
                binding->user_param = user_param;

                if ((widget == NULL) && tag) {
                        const uint32_t size = strlen(tag) + 1;
                        char *tag_copy = NULL;
                        tag_copy = malloc(size);
                        strncpy(tag_copy, tag, size);

                        binding->tag = tag_copy;
                }

                ei_linkedlist_t *list = &ei_events[eventtype];

                ei_linkedlist_add(list, binding);
        }
}


/**
 * \brief       Unbinds a callback from an event type and widget or tag.
 *
 * @param       eventtype, widget, tag, callback, user_param
 *                              All parameters must have the same value as when \ref ei_bind was
 *                              called to create the binding.
 */
void ei_unbind(ei_eventtype_t eventtype,
                ei_widget_t *widget,
                ei_tag_t tag,
                ei_callback_t callback,
                void *user_param)
{
        if (eventtype < ei_ev_last) {
                ei_linkedlist_t *list = &ei_events[eventtype];

                ei_linked_elem_t *link = list->head;
                ei_binding_t *binding = NULL;

                /* Unbind all links */
                while (link) {
                        binding = (ei_binding_t*)link->elem;

                        if (binding) {
                                if (    (widget == binding->widget)
                                        && ((tag == NULL) || !strcmp(tag, binding->tag))
                                        && (callback == binding->callback)
                                        && (user_param == binding->user_param)
                                        ) {

                                        //printf("Unbind link %x !\n", link);



                                        if (binding->tag)
                                                SAFE_FREE(binding->tag);

                                        ei_linkedlist_pop_link(list, link, true);
                                }
                        }

                        link = link->next;
                }
        }

        //printf("ei_unbind END\n");
}

void ei_event_process(ei_event_t *event)
{
        if (!event || event->type >= ei_ev_last)
                return;

        ei_bool_t call, done = EI_FALSE;
        ei_linkedlist_t list = ei_events[event->type];
        ei_linked_elem_t *link = list.head;
        ei_binding_t *binding = NULL;
        ei_widget_t *widget = NULL, *selected = NULL;


        if (!link)
                done = EI_TRUE;

        // Si l'event est lié aux boutons de la souris on selectionne le widget
        // TODO : Optimisation : ne faire que si un binding est lié à cet événement
        if (event->type == ei_ev_mouse_buttondown
                || event->type == ei_ev_mouse_buttonup
                || event->type == ei_ev_mouse_move) {
                selected = ei_widget_pick(&event->param.mouse.where);
        /*printf("selected %08lX", selected);

        if (selected && selected->wclass)
                printf("   %s", selected->wclass->name);


        printf("\n");*/
        }

        // Parcours de tous les links
        while (!done) {
                binding = link->elem;
                call = EI_FALSE;

                if (binding && binding->callback) {
                        // Si le tag est "all", on appelle le callback
                        // dans tous les cas
                        if (binding->tag && !strcmp(binding->tag, "all"))
                                call = EI_TRUE;

                        else {
                                switch (event->type) {
                                case ei_ev_app:
                                        call = EI_TRUE;
                                        break;

                                case ei_ev_keydown:
                                case ei_ev_keyup:
                                        call = EI_TRUE;
                                        break;

                                        // Pour les boutons de la souris, on
                                        // appelle le callback sur selected
                                case ei_ev_mouse_buttondown:
                                case ei_ev_mouse_buttonup:
                                case ei_ev_mouse_move:

                                        if (selected) {

                                                /* If linked widget is selected */
                                                if (binding->widget == selected)
                                                        call = EI_TRUE, widget = selected;

                                                /* If selected widget is tagged */
                                                else if (binding->tag && selected->wclass) {
                                                        if (!strcmp(selected->wclass->name, binding->tag)) {
                                                                call = EI_TRUE, widget = selected;
                                                        }
                                                }
                                        }

                                        break;

                                default:
                                        ;
                                }
                        }

                        if (call) {
                                done = binding->callback(widget, event, binding->user_param);
                        }
                }


                if (!done) {
                        link = link->next;
                        if (!link)
                                done = EI_TRUE;
                }
        }
}


