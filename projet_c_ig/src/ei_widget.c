/**
 * @file	ei_widget.h
 *
 * @brief 	API for widgets management: creation, configuration, hierarchy, redisplay.
 * 
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 */

// pour malloc et NULL
#include <stdlib.h>
#include <string.h>
#include "ei_widget.h"
#include "ei_widgettypes.h"

/**
 * @brief	Creates a new instance of a widget of some particular class, as a descendant of
 *		an existing widget.
 *
 *		The widget is not displayed on screen until it is managed by a geometry manager.
 *		The widget should be released by calling \ref ei_widget_destroy when no more needed.
 *
 * @param	class_name	The name of the class of the widget that is to be created.
 * @param	parent 		A pointer to the parent widget. Can not be NULL.
 *
 * @return			The newly created widget, or NULL if there was an error.
 */

// Quels paramètres faut-il initialiser ici ?
ei_widget_t* ei_widget_create (ei_widgetclass_name_t class_name, 
		ei_widget_t* parent){
	ei_widget_t *widget = NULL;
	ei_widgetclass_t *wclass;
	// Configuration grace au paramètres
	wclass = ei_widgetclass_from_name(class_name);
	//printf("%x wclass allocfunc\n", wclass->allocfunc);

	if (wclass)
		// après allocation, widget aura les champs communs + les champs uniques 
		widget = wclass->allocfunc();

	if (widget) {
		widget->wclass = wclass;

		// Initialisation des attributs
		wclass->setdefaultsfunc(widget);


		if (parent) {
			// Initialisation des attributs communs
			widget->parent = parent;

			if (parent->children_tail) {
				parent->children_tail->next_sibling = widget;
				parent->children_tail = widget;
			}

			if (!parent->children_head)
				parent->children_head = widget;
		}

		ei_color_t *pc = malloc(sizeof(ei_color_t));
		memset(pc, 0, sizeof(ei_color_t));
		widget->pick_color = pc;

		ei_size_t rs = {10,10};
		widget->requested_size = rs;

		ei_rect_t sl = {{0,0}, {10,10}};
		widget->screen_location = sl;
		widget->content_rect = &widget->screen_location;


		return widget;
	}
	else {
		return NULL;}
}

/**
 * @brief	Destroys a widget. Removes it from screen if it is managed by a geometry manager.
 *		Destroys all its descendants.
 *
 * @param	widget		The widget that is to be destroyed.
 */
void ei_widget_destroy (ei_widget_t* widget){
	if (widget){
                ei_widget_destroy(widget->next_sibling);
                ei_widget_destroy(widget->children_head);
                widget->wclass->releasefunc(widget);
        }
}


/**
 * @brief	Returns the widget that is at a given location on screen.
 *
 * @param	where		The location on screen, expressed in the root window coordinates.
 *
 * @return			The top-most widget at this location, or NULL if there is no widget
 *				at this location (except for the root widget).
 */
ei_widget_t* ei_widget_pick (ei_point_t* where){
        return NULL;
}




/**
 * @brief	Configures the attributes of widgets of the class "frame".
 *
 *		Parameters obey the "default" protocol: if a parameter is "NULL" and it has never
 *		been defined before, then a default value should be used (default values are
 *		specified for each parameter). If the parameter is "NULL" but was defined on a
 *		previous call, then its value must not be changed.
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The size requested for this widget. The geometry manager may
 *				override this size due to other constraints.
 *				Defaults to the "natural size" of the widget, ie. big enough to
 *				display the text or the image, or (0, 0) if the widget has no text
 *				and no image.
 * @param	color		The color of the background of the widget. Defaults to
 *				\ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border decoration of the widget. The final
 *				appearance depends on the "relief" parameter. Defaults to 0.
 * @param	relief		Appearance of the border of the widget. Defaults to
 *				\ref ei_relief_none.
 * @param	text		The text to display in the widget, or NULL. Only one of the
 *				parameter "text" and "img" should be used (i.e. non-NULL). Defaults
 *				to NULL.
 * @param	text_font	The font used to display the text. Defaults to \ref ei_default_font.
 * @param	text_color	The color used to display the text. Defaults to 
 *				\ref ei_font_default_color.
 * @param	text_anchor	The anchor of the text, i.e. where it is placed whithin the widget
 *				when the size of the widget is bigger than the size of the text.
 *				Defaults to \ref ei_anc_center.
 * @param	img		The image to display in the widget, or NULL. Any surface can be
 *				used, but usually a surface returned by \ref hw_image_load. Only one
 *				of the parameter "text" and "img" should be used (i.e. non-NULL).
 Defaults to NULL.
 * @param	img_rect	If not NULL, this rectangle defines a subpart of "img" to use as the
 *				image displayed in the widget. Defaults to NULL.
 * @param	img_anchor	The anchor of the image, i.e. where it is placed whithin the widget
 *				when the size of the widget is bigger than the size of the image.
 *				Defaults to \ref ei_anc_center.
 */
void	ei_frame_configure (ei_widget_t* widget,
                ei_size_t*		requested_size,
                const ei_color_t*	color,
                int*			border_width,
                ei_relief_t*		relief,
                char**			text,
                ei_font_t*		text_font,
                ei_color_t*		text_color,
                ei_anchor_t*		text_anchor,
                ei_surface_t*		img,
                ei_rect_t**		img_rect,
                ei_anchor_t*		img_anchor){

        if (widget && widget->wclass && !strcmp(widget->wclass->name, "frame")){
                // on recaste pour passer a un type frame
                ei_frame_t *frame = (ei_frame_t*)widget;
                if (requested_size) {
                        frame->widget.requested_size = *requested_size;
                }
                if (color) {
                        frame->bg_color = *color;
                }
                if (border_width) {
                        frame->border_width = *border_width;
                }
                if (relief) {
                        frame->relief = *relief;
                }
                if (text) {
                        frame->text = *text;
                }
                if (text_font){
                        frame->text_font = *text_font;
                }
                if (text_color){
                        frame->text_color = *text_color;
                }
                if (text_anchor){
                        frame->text_anchor = *text_anchor;
                }
                if (img){
                        frame->img = *img;
                }
                if (img_rect){
                        frame->img_rect = *img_rect;
                }
                if (img_anchor){
                        frame->img_anchor = *img_anchor;
                }


        }
}




/**
 * @brief	Configures the attributes of widgets of the class "button".
 *
 * @param	widget, requested_size, color, border_width, relief,
 *		text, text_font, text_color, text_anchor,
 *		img, img_rect, img_anchor
 *				See the parameter definition of \ref ei_frame_configure.
 * @param	corner_radius	The radius (in pixels) of the rounded corners of the button.
 *				0 means straight corners. Defaults to k_default_button_corner_radius.
 * @param	callback	The callback function to call when the user clicks on the button.
 *				Defaults to NULL (no callback).
 * @param	user_param	A programmer supplied parameter that will be passed to the callback
 *				when called. Defaults to NULL.
 */
void	ei_button_configure (ei_widget_t*		widget,
                ei_size_t*		requested_size,
                const ei_color_t*	color,
                int*			border_width,
                int*			corner_radius,
                ei_relief_t*		relief,
                char**			text,
                ei_font_t*		text_font,
                ei_color_t*		text_color,
                ei_anchor_t*		text_anchor,
                ei_surface_t*		img,
                ei_rect_t**		img_rect,
                ei_anchor_t*		img_anchor,
                ei_callback_t*		callback,
                void**			user_param){

        if (widget && widget->wclass && !strcmp(widget->wclass->name, "button")){
                ei_button_t *button = (ei_button_t*)widget;
                if(requested_size){
                        button->widget.requested_size = *requested_size;
                }
                if (color) {
                        button->color = color;
                }
                if (border_width) {
                        button->border_width = *border_width;
                }
                if (corner_radius) {
                        button->corner_radius = *corner_radius;
                }
                if(relief){
                        button->relief = *relief;
                }
                if (text) {
                        button->text = *text;
                }
                if(text_font){
                        button->text_font = *text_font;
                }
                if(text_color){
                        button->text_color = *text_color;
                }
                if(text_anchor){
                        button->text_anchor = *text_anchor;
                }
                if(img) {
                        button->img = *img;
                }
                if(img_rect){
                        button->img_rect = *img_rect;
                }
                if(img_anchor) {
                        button->img_anchor = *img_anchor;
                }
                if(callback) {
                        button->callback = *callback;
                }
                if (user_param){
                        button->user_param = *user_param;
                }
        }
}

/**
 * @brief	Configures the attributes of widgets of the class "toplevel".
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The content size requested for this widget, this does not include
 *				the decorations	(border, title bar). The geometry manager may
 *				override this size due to other constraints.
 *				Defaults to (320x240).
 * @param	color		The color of the background of the content of the widget. Defaults
 *				to \ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border of the widget. Defaults to 4.
 * @param	title		The string title diplayed in the title bar. Defaults to "Toplevel".
 * @param	closable	If true, the toplevel is closable by the user, the toplevel must
 *				show a close button in its title bar. Defaults to \ref EI_TRUE.
 * @param	resizable	Defines if the widget can be resized horizontally and/or vertically
 *				by the user. Defaults to \ref ei_axis_both.
 * @param	min_size	For resizable widgets, defines the minimum size. Defaults to
 *				(160, 120).
 */
void	ei_toplevel_configure	(ei_widget_t*	widget,
                ei_size_t*	requested_size,
                ei_color_t*	color,
                int*		border_width,
                char**		title,
                ei_bool_t*	closable,
                ei_axis_set_t*	resizable,
                ei_size_t**	min_size){

        if (widget && widget->wclass &&
                        !strcmp(widget->wclass->name, "toplevel")){
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
                if (requested_size){
                        toplevel->widget.requested_size = *requested_size;
                }
                if (color){
                        toplevel->color = *color;
                }
                if (border_width) {
                        toplevel->border_width = *border_width;
                }
                if(title){
                        toplevel->title = *title;
                }
                if (closable){
                        toplevel->closable = *closable;
                }
                if(resizable){
                        toplevel->resizable = *resizable;
                }
                if(min_size){
                        toplevel->min_size = *min_size;
                }
        }
}


