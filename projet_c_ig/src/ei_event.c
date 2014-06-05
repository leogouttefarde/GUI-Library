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

/* Event link */
typedef struct ei_ev_link_t {
	ei_tag_t tag;
	ei_widget_t *widget;
	ei_callback_t callback;
	void *user_param;
	struct ei_ev_link_t *next;
} ei_ev_link_t;

/* Event links */
typedef struct ei_ev_links_t {
	struct ei_ev_link_t *head;
	struct ei_ev_link_t *tail;
} ei_ev_links_t;

/* Event table */
ei_ev_links_t ei_events[ei_ev_last] = {
	{ NULL, NULL },
	{ NULL, NULL },
	{ NULL, NULL },
	{ NULL, NULL },
	{ NULL, NULL },
	{ NULL, NULL },
	{ NULL, NULL }
};



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
		ei_ev_link_t *link = malloc(sizeof(ei_ev_link_t));

		link->widget = widget;
		link->callback = callback;
		link->user_param = user_param;
		link->next = NULL;

		if (widget == NULL)
			link->tag = tag;

		ei_ev_links_t *links = &ei_events[eventtype];

		if (!links->head) {
			links->head = link;
			links->tail = link;
		}
		else {
			links->tail->next = link;
			links->tail = link;
		}
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

	ei_bool_t bind, done = EI_FALSE;
	ei_ev_links_t links = ei_events[event->type];
	ei_ev_link_t *link = links.head;
	ei_widget_t *widget = NULL;


	if (!link)
		done = EI_TRUE;

	while (!done) {
		bind = EI_FALSE;

		if (link->callback) {
			if (link->tag && !strcmp(link->tag, "all"))
				bind = EI_TRUE;

			else {
				switch (event->type) {
				case ei_ev_app:
					bind = EI_TRUE;
					break;

				case ei_ev_keydown:
				case ei_ev_keyup:
					bind = EI_TRUE;
					break;

				case ei_ev_mouse_buttondown:
				case ei_ev_mouse_buttonup:
				case ei_ev_mouse_move:
					/*
					// Get current mouse picking
					cur_pick = get_picking(event->param.mouse.where, ei_app_picking_surface());

					if (link->widget && (link->widget->pick == cur_pick)) // If widget picking match
						bind = EI_TRUE, widget = link->widget;

					else if (link->tag) {
						// for all widgets if (!done)
						if (!strcmp(widget->wclass->name, link->tag)) {
							if (widget->pick == cur_pick) {
								bind = EI_TRUE;
							}
							if (bind)
								done = link->callback(widget, event, link->user_param);
						}
					}
					*/
					break;

				default:
					;
				}
			}

			if (bind)
				done = link->callback(widget, event, link->user_param);
		}


		link = link->next;
		if (!link)
			done = EI_TRUE;
	}
}


