/**
 *  @file       ei_event.c
 *  @brief      Allows the binding and unbinding of callbacks to events.
 *
 *  \author 
 *  Created by Léo GOUTTEFARDE on 04.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#include "ei_event_pv.h"
#include "ei_core.h"
#include "ei_utilities.h"
#include "ei_linkedlist.h"
#include "ei_tag.h"


/* Event binding table */
static ei_linkedlist_t ei_events[ei_ev_last];


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

                if ((widget == NULL) && tag)
                        make_string_copy(&binding->tag, tag);

                ei_linkedlist_t *list = &ei_events[eventtype];

                ei_linkedlist_add(list, binding);
        }
}

void ei_unbind_link(ei_linkedlist_t *list, ei_linked_elem_t *link)
{
        if (list && link) {
                ei_binding_t *binding = (ei_binding_t*)link->elem;

                if (binding && binding->tag)
                        SAFE_FREE(binding->tag);

                ei_linkedlist_pop_link(list, link, EI_TRUE);
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

                ei_linked_elem_t *link = list->head, *next = NULL;
                ei_binding_t *binding = NULL;

                /* Unbind all links */
                while (link) {
                        binding = (ei_binding_t*)link->elem;
                        next = link->next;

                        if (    binding
                                && (widget == binding->widget)
                                && ((tag == NULL) || !strcmp(tag, binding->tag))
                                && (callback == binding->callback)
                                && (user_param == binding->user_param) )
                                ei_unbind_link(list, link);

                        link = next;
                }
        }
}

void ei_event_process(ei_event_t *event)
{
        if (!event || event->type >= ei_ev_last)
                return;

        ei_bool_t call, done = EI_FALSE;
        ei_linkedlist_t list = ei_events[event->type];
        ei_linked_elem_t *link = list.head;
        ei_binding_t *binding = NULL;
        ei_widget_t *widget = NULL, *selection = NULL;
        ei_bool_t selected = EI_FALSE;

        /* If no binding, finish */
        if (!link)
                done = EI_TRUE;


        // Parcours de tous les links
        while (!done) {
                binding = link->elem;
                call = EI_FALSE;

                if (binding && binding->callback) {
                        ei_tag_t tag = binding->tag;

                        // Si le tag est "all", on appelle le callback
                        // dans tous les cas
                        if (tag && !strcmp(tag, "all"))
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
                                        // appelle le callback sur selection
                                case ei_ev_mouse_buttondown:
                                case ei_ev_mouse_buttonup:
                                case ei_ev_mouse_move:
                                        if (!selected) {
                                                selection = ei_widget_pick(&event->param.mouse.where);
                                                selected = EI_TRUE;
                                        }

                                        if (selection) {

                                                /* If linked widget is selection */
                                                if (binding->widget == selection)
                                                        call = EI_TRUE, widget = selection;

                                                /* If selection widget is tagged */
                                                else if (tag && ei_has_tag(selection, tag)) {
                                                        call = EI_TRUE, widget = selection;
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

void ei_unbind_all()
{
        ei_eventtype_t type;
        ei_linkedlist_t *list = NULL;
        ei_linked_elem_t *link = NULL, *next = NULL;

        for (type = 0; type < ei_ev_last; ++type) {
                list = &ei_events[type];
                link = list->head;

                /* Unbind all links */
                while (link) {
                        next = link->next;
                        ei_unbind_link(list, link);
                        link = next;
                }
        }
}


