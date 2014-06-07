/**
 *  @file	ei_event.c
 *  @brief	Allows the binding and unbinding of callbacks to events.
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
 * \brief	Binds a callback to an event type and a widget or a tag.
 *
 * @param	eventtype	The type of the event.
 * @param	widget		The callback is only called if the event is related to this widget.
 *				This parameter must be NULL if the "tag" parameter is not NULL.
 * @param	tag		The callback is only called if the event is related to a widget that
 *				has this tag. A tag can be a widget class name, or the tag "all".
 *				This parameter must be NULL is the "widget" parameter is not NULL.
 * @param	callback	The callback (i.e. the function to call).
 * @param	user_param	A user parameter that will be passed to the callback when it is
 *				called.
 */
void ei_bind(ei_eventtype_t eventtype,
		ei_widget_t *widget,
		ei_tag_t tag,
		ei_callback_t callback,
		void *user_param)
{
	if (eventtype < ei_ev_last) {
		ei_binding_t *bind = malloc(sizeof(ei_binding_t));
		memset(bind, 0, sizeof(ei_binding_t));

		bind->widget = widget;
		bind->callback = callback;
		bind->user_param = user_param;

		if (widget == NULL)
			bind->tag = tag;

		ei_linkedlist_t *list = &ei_events[eventtype];

		ei_linkedlist_add(list, bind);
	}
}


/**
 * \brief	Unbinds a callback from an event type and widget or tag.
 *
 * @param	eventtype, widget, tag, callback, user_param
 *				All parameters must have the same value as when \ref ei_bind was
 *				called to create the binding.
 */
void ei_unbind(ei_eventtype_t eventtype,
		ei_widget_t *widget,
		ei_tag_t tag,
		ei_callback_t callback,
		void *user_param)
{
	;
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
	if (event->type == ei_ev_mouse_buttondown
		|| event->type == ei_ev_mouse_buttonup
		|| event->type == ei_ev_mouse_move) {
		selected = ei_widget_pick(&event->param.mouse.where);
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
					/* If linked widget is selected */
					if (binding->widget == selected)
						call = EI_TRUE, widget = selected;

					/* If selected widget is tagged */
					else if (binding->tag) {
						if (!strcmp(selected->wclass->name, binding->tag))
							call = EI_TRUE, widget = selected;
					}
					break;

				default:
					;
				}
			}

			if (call) {
				done = binding->callback(widget, event, binding->user_param);
				ei_invalidate_widget(widget);
			}
		}


		link = link->next;
		if (!link)
			done = EI_TRUE;
	}
}


