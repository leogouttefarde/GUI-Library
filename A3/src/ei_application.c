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

	memset(&root_widget, 0, sizeof(root_widget));
	root_widget.wclass = NULL;
	root_widget.parent = NULL;




/**
 * \brief	Fields common to all types of widget. Every widget classes specializes this base
 *		class by adding its own fields. 
 */
typedef struct ei_widget_t {
	ei_widgetclass_t*	wclass;		///< The class of widget of this widget. Avoid the field name "class" which is a keyword in C++.
	uint32_t		pick_id;	///< Id of this widget in the picking offscreen.
	ei_color_t*		pick_color;	///< pick_id encoded as a color.

	/* Widget Hierachy Management */
	struct ei_widget_t*	parent;		///< Pointer to the parent of this widget.
	struct ei_widget_t*	children_head;	///< Pointer to the first child of this widget.	Children are chained with the "next_sibling" field.
	struct ei_widget_t*	children_tail;	///< Pointer to the last child of this widget.
	struct ei_widget_t*	next_sibling;	///< Pointer to the next child of this widget's parent widget.

	/* Geometry Management */
	struct ei_geometry_param_t*
				geom_params;	///< Pointer to the geometry management parameters for this widget. If NULL, the widget is not currently managed and thus, is not mapped on the screen.
	ei_size_t		requested_size;	///< Size requested by the widget (big enough for its label, for example), or by the programmer. This can be different than its screen size defined by the placer.
	ei_rect_t		screen_location;///< Position and size of the widget expressed in the root window reference.
	ei_rect_t*		content_rect;	///< Where to place children, when this widget is used as a container. By defaults, points to the screen_location.
} ei_widget_t;

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
ei_widget_t* ei_app_root_widget()
{
	return &root_widget;
}

/**
 * \brief	Returns the surface of the root window. Used to create surfaces with similar r, g, b
 *		channels.
 *
 * @return 			The surface of the root window.
 */
ei_surface_t ei_app_root_surface()
{
	return main_window;
}


