/**
 *  @file	ei_geometrymanager.h
 *  @brief	Manages the positionning and sizing of widgets on the screen.
 *
 *  \author
 *  Created by François Bérard on 18.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */


#include "ei_geometrymanager.h"
#include "ei_common.h"


static ei_geometrymanager_t *first = NULL;

/**
 * \brief	Registers a geometry manager to the program so that it can be called to manager
 *		widgets. This must be done only once in the application.
 *
 * @param	geometrymanager		The structure describing the geometry manager.
 */
void ei_geometrymanager_register(ei_geometrymanager_t* geometrymanager)
{
	if (geometrymanager) {
		ei_geometrymanager_t *current, *next;

		if (first) {
			current = first;
			next = current;

			do {
				current = next;
				next = current->next;
			} while (next);

			current->next = geometrymanager;
		}
		else
			first = geometrymanager;

		/* Make sure it is the last geometry manager */
		geometrymanager->next = NULL;
	}
}



/**
 * \brief	Returns a geometry manager structure from its name.
 *
 * @param	name		The name of the geometry manager.
 *
 * @return			The structure describing the geometry manager.
 */
ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name)
{
	ei_geometrymanager_t *current, *next;
	ei_geometrymanager_t *geometrymanager = NULL;
	ei_bool_t fail = EI_TRUE;

	current = first;
	next = current;
	while (current && (fail = strncmp(current, name, sizeof(ei_geometrymanager_name_t))) && next) {
		current = next;
		next = current->next;
	}

	if (!fail)
		geometrymanager = current;

	return geometrymanager;
}



/**
 * \brief	Tell the geometry manager in charge of a widget to forget it. This removes the
 *		widget from the screen. If the widget is not currently managed, this function
 *		returns silently.
 *		Side effects:
 *		<ul>
 *			<li> the \ref ei_geometrymanager_releasefunc_t of the geometry manager in
 *				charge of this widget is called, </li>
 *			<li> the geom_param field of the widget is freed, </li>
 *			<li> the current screen_location of the widget is invalided (which will
 *				trigger a redraw), </li>
 *			<li> the screen_location of the widget is reset to 0. </li>
 *		</ul>
 *
 * @param	widget		The widget to unmap from the screen.
 */
void ei_geometrymanager_unmap(ei_widget_t* widget)
{

}


void ei_place_runfunc(struct ei_widget_t*	widget)
{
	widget->wclass->drawfunc(widget, ei_app_root_surface(), NULL, widget->parent ? &widget->screen_location : NULL);
}

void ei_place_releasefunc(struct ei_widget_t*	widget)
{
	SAFE_FREE(widget->geom_params);
}

/**
 * \brief	Registers the "placer" geometry manager in the program. This must be called only
 *		once before the \ref ei_place function can be called.
 */
void  ei_register_placer_manager()
{
	ei_geometrymanager_t *placer = malloc(sizeof(ei_geometrymanager_t));
	memset(placer, 0, sizeof(ei_geometrymanager_t));
	strcpy(placer->name, "placer");

	placer->runfunc = ei_place_runfunc;
	placer->releasefunc = ei_place_releasefunc;
	placer->next = NULL;

	ei_geometrymanager_register(placer);
}



/**
 * \brief	Configures the geometry of a widget using the "placer" geometry manager.
 * 		If the widget was already managed by another geometry manager, then it is first
 *		removed from the previous geometry manager.
 * 		If the widget was already managed by the "placer", then this calls simply updates
 *		the placer parameters: arguments that are not NULL replace previous values.
 * 		When the arguments are passed as NULL, the placer uses default values (detailed in
 *		the argument descriptions below). If no size is provided (either absolute or
 *		relative), then the requested size of the widget is used, i.e. the minimal size
 *		required to display its content.
 *
 * @param	widget		The widget to place.
 * @param	anchor		How to anchor the widget to the position defined by the placer
 *				(defaults to ei_anc_northwest).
 * @param	x		The absolute x position of the widget (defaults to 0).
 * @param	y		The absolute y position of the widget (defaults to 0).
 * @param	width		The absolute width for the widget (defaults to the requested width
 *				of the widget).
 * @param	height		The absolute height for the widget (defaults to the requested height
 *				of the widget).
 * @param	rel_x		The relative x position of the widget: 0.0 corresponds to the left
 *				side of the master, 1.0 to the right side (defaults to 0.0).
 * @param	rel_y		The relative y position of the widget: 0.0 corresponds to the top
 *				side of the master, 1.0 to the bottom side (defaults to 0.0).
 * @param	rel_width	The relative width of the widget: 0.0 corresponds to a width of 0,
 *				1.0 to the width of the master (defaults to 0.0).
 * @param	rel_height	The relative height of the widget: 0.0 corresponds to a height of 0,
 *				1.0 to the height of the master (defaults to 0.0).
 */
void ei_place(ei_widget_t *widget,
		ei_anchor_t	*anchor,
		int *x,
		int *y,
		int *width,
		int *height,
		float *rel_x,
		float *rel_y,
		float *rel_width,
		float *rel_height)
{
	ei_geometrymanager_t *placer = ei_geometrymanager_from_name("placer");

	if (placer) {
		if (widget && (!widget->geom_params || (widget->geom_params->manager != placer))) {
			if (widget->geom_params && widget->geom_params->manager)
				widget->geom_params->manager->releasefunc(widget);

			widget->geom_params = malloc(sizeof(ei_placer_param_t));
			widget->geom_params->manager = placer;
		}

		if (x)
			widget->screen_location.top_left.x = *x;
		else
			widget->screen_location.top_left.x = 0;

		if (y)
			widget->screen_location.top_left.y = *y;
		else
			widget->screen_location.top_left.y = 0;


		widget->screen_location.size = widget->requested_size;

		if (width)
			widget->screen_location.size.width = *width;

		if (height)
			widget->screen_location.size.height = *height;
	}
}


