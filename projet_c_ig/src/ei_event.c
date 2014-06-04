/**
 *  @file	ei_event.c
 *  @brief	Allows the binding and unbinding of callbacks to events.
 *
 *  \author 
 *  Created by Léo Gouttefarde on 04.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#include "ei_event.h"



typedef struct ei_binding_t {
	ei_linked_tag_t ltag;
	ei_widget_t *widget;
	ei_event_t event;
	ei_callback_t callback;
} ei_binding_t;


ei_binding_t *first = NULL;
ei_binding_t *last = NULL;


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
	ei_binding_t *binding = malloc(sizeof(ei_binding_t));
	memset(binding, 0, sizeof(ei_binding_t));

	binding->event.type = eventtype;
	binding->callback = callback;

	if (eventtype == ei_ev_app)
		binding->event.param.application.user_param = user_param;

	if (widget)
		binding->widget = widget;
	else
		binding->ltag.tag = tag;

	if (!first) {
		first = binding;
		last = first;
	}
	else {
		last->ltag.next = binding;
		last = binding;
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
	ei_binding_t *binding = first;
	ei_bool_t ev_match;
	while (binding) {
		ev_match = EI_FALSE;

		if (binding->callback && (event->type == binding->event.type)) {
			if (!strcmp(binding->ltag.tag, "all"))
				ev_match = EI_TRUE;

			else {
				switch (event->type) {
					case ei_ev_app:
						break;

					case ei_ev_keydown:
					case ei_ev_keyup:
						break;

					case ei_ev_mouse_move:
						// if (event->param.mouse.where.x)
						// 	ei_app_picking_surface();
						// if (pick == binding->widget->pick)
						// 	ev_match = EI_TRUE;

					case ei_ev_mouse_buttondown:
					case ei_ev_mouse_buttonup:
						//if (event->param.mouse.button)
						break;

				}
			}

			if (ev_match) {
				ei_bool_t result;
				result = binding->callback(NULL, event, NULL);
			}
		}


		binding = binding->ltag.next;
	}
}


