#ifndef EI_WIDGETTYPES_H
#define EI_WIDGETTYPES_H
// pour avoir le type ei_widget
#include "ei_widget.h"
//Definition du type frame
typedef struct ei_frame_t {
        ei_widget_t widget;
        const ei_color_t *bg_color;
        int border_width;
        ei_relief_t relief;
        char* text;
        ei_font_t text_font;
        ei_color_t text_color;
        ei_anchor_t text_anchor;
        ei_surface_t img;
        ei_rect_t* img_rect;
        ei_anchor_t img_anchor;
} ei_frame_t;

//Definition du type button
typedef struct ei_button_t {
        ei_widget_t widget;
        const ei_color_t	*color;
        int		border_width;
        int		corner_radius;
        ei_relief_t	relief;
        char*		text;
        ei_font_t	text_font;
        ei_color_t	text_color;
        ei_anchor_t	text_anchor;
        ei_surface_t	img;
        ei_rect_t*	img_rect;
        ei_anchor_t	img_anchor;
        ei_callback_t	callback;
        void *	user_param;
} ei_button_t;

//Definition du type toplevel
typedef struct ei_toplevel_t {
        ei_widget_t widget;
        ei_color_t	color;
        int		border_width;
        char*		title;
        ei_bool_t	closable;
        ei_axis_set_t	resizable;
        ei_size_t*	min_size;
} ei_toplevel_t;
#endif
