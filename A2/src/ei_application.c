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



#include "ei_application.h"

static ei_surface_t main_window;
static ei_widget_t root_widget;


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
void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen){
	hw_init();
	main_window = hw_create_window(main_window_size, fullscreen);
	root_widget.parent=NULL;
}

/**
 * \brief	Releases all the resources of the application, and releases the hardware
 *		(ie. calls \ref hw_quit).
 */
void ei_app_free(){
        ;
}

/**
 * \brief	Runs the application: enters the main event loop. Exits when
 *		\ref ei_app_quit_request is called.
 */
void ei_app_run(){
	int c;

	do {
		c=getchar();


		ei_widget_t *widget = ei_app_root_widget();

		while (widget) {
			if (widget->wclass && widget->wclass->drawfunc)
				widget->wclass->drawfunc(widget, ei_app_root_surface(), NULL, NULL);

			if (widget->next_sibling)
				widget = widget->next_sibling;

			else
				widget = widget->children_head;
		}
	} while (c != '\n');
}

/**
 * \brief	Adds a rectangle to the list of rectangles that must be updated on screen. The real
 *		update on the screen will be done at the right moment in the main loop.
 *
 * @param	rect		The rectangle to add, expressed in the root window coordinates.
 *				A copy is made, so it is safe to release the rectangle on return.
 */
void ei_app_invalidate_rect(ei_rect_t* rect){;}

/**
 * \brief	Tells the application to quite. Is usually called by an event handler (for example
 *		when pressing the "Escape" key).
 */
void ei_app_quit_request(){;}

/**
 * \brief	Returns the "root widget" of the application: a "frame" widget that encapsulate the
 *		root window.
 *
 * @return 			The root widget.
 */
ei_widget_t* ei_app_root_widget(){;}

/**
 * \brief	Returns the surface of the root window. Used to create surfaces with similar r, g, b
 *		channels.
 *
 * @return 			The surface of the root window.
 */
ei_surface_t ei_app_root_surface(){;}





