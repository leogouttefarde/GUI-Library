/**
 *  @file	ei_application.h
 *  @brief	Manages the main steps of a graphical application: initialization, main window,
 *		main loop, quitting, resource freeing.
 *
 *  \author 
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_common.h"
#include "ei_event.h"
#include "ei_widgettypes.h"
#include "ei_core.h"

static ei_bool_t quit_request = EI_FALSE;

// Peut-être pas dans le bon fichier
// Enfonce les boutons
ei_bool_t button_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
	if (widget){
		if (!strcmp(widget->wclass->name, "button")) {

			ei_button_t *button = (ei_button_t*)widget;
			button->relief = ei_relief_sunken;
		}
	}
	return EI_FALSE;
}

// Quand on relache la souris sur le bouton
ei_bool_t button_callback_release(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
	ei_bool_t res;

	if (widget){
		if (!strcmp(widget->wclass->name, "button")) {

			ei_button_t *button = (ei_button_t*)widget;
			button->relief = ei_relief_raised;

			// Appel du callback du bouton
			if (button->callback){
				res = button->callback((ei_widget_t*)button, NULL, button->user_param);
			}
		}
	}
	return EI_FALSE;
}

/**
 * \brief	Creates an application.
 *		<ul>
 *			<li> initializes the hardware (calls \ref hw_init), </li>
 *			<li> registers all classes of widget and all geometry managers, </li>
 *			<li> creates the root window (either in a system window, or the entire
 *				screen), </li>
 *			<li> creates the root widget to accress the root window. </li>
 *		</ul>
 *
 * @param	main_window_size	If "fullscreen is false, the size of the root window of the
 *					application.
 *					If "fullscreen" is true, the current monitor resolution is
 *					used as the size of the root window, this size is returned
 *					in this parameter.
 * @param	fullScreen		If true, the root window is the entire screen. Otherwise, it
 *					is a system window.
 */
void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen)
{
	ei_init();
	hw_init();

	ei_set_root_surface(hw_create_window(main_window_size, fullscreen));

	// Enregistrement des classes
	ei_frame_register_class();
	ei_button_register_class();
	ei_toplevel_register_class();

	// Initialisation du root_widget
	ei_set_root(ei_widget_create ("frame", NULL));

	ei_register_placer_manager();

	ei_widget_t *root_widget = ei_get_root();
	ei_place(root_widget, NULL, NULL, NULL, &main_window_size->width, &main_window_size->height
			, NULL, NULL, NULL, NULL );

	ei_set_picking_surface(hw_surface_create(ei_get_root_surface(), main_window_size, EI_TRUE));

	// Pour gérer le clic sur les boutons ils faut faire un bind sur le tag
	// "button" dans cette fonction avec les callback 1 et 2 définies dans
	// ei_widget_class
	ei_bind(ei_ev_mouse_buttondown, NULL, "button", button_callback_click, NULL);
	ei_bind(ei_ev_mouse_buttonup, NULL, "button", button_callback_release, NULL);
}

/**
 * \brief	Releases all the resources of the application, and releases the hardware
 *		(ie. calls \ref hw_quit).
 */
void ei_app_free()
{
	ei_surface_t picking = ei_get_picking_surface();

	if (picking)
		hw_surface_free(picking);
}

/**
 * \brief	Runs the application: enters the main event loop. Exits when
 *		\ref ei_app_quit_request is called.
 */
void ei_app_run()
{
	ei_event_t event;
	ei_surface_t root_surface = ei_get_root_surface();

	while (!quit_request) {

		ei_draw_widgets();

		ei_invalidate_rects();

		// Update des surfaces
		hw_surface_update_rects(root_surface, ei_get_update_rects());

		ei_invalidate_reset();


		hw_event_wait_next(&event);

		ei_event_process(&event);
	};
}

/**
 * \brief	Adds a rectangle to the list of rectangles that must be updated on screen. The real
 *		update on the screen will be done at the right moment in the main loop.
 *
 * @param	rect		The rectangle to add, expressed in the root window coordinates.
 *				A copy is made, so it is safe to release the rectangle on return.
 */
void ei_app_invalidate_rect(ei_rect_t* rect)
{
	ei_invalidate_rect(rect);
}

/**
 * \brief	Tells the application to quite. Is usually called by an event handler (for example
 *		when pressing the "Escape" key).
 */
void ei_app_quit_request()
{
	quit_request = EI_TRUE;
}


/**
 * \brief	Returns the "root widget" of the application: a "frame" widget that encapsulate the
 *		root window.
 *
 * @return 			The root widget.
 */
ei_widget_t* ei_app_root_widget(){

	return ei_get_root();
}

/**
 * \brief	Returns the surface of the root window. Used to create surfaces with similar r, g, b
 *		channels.
 *
 * @return 			The surface of the root window.
 */
ei_surface_t ei_app_root_surface(){

	return ei_get_root_surface();
}

