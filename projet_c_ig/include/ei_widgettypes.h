
#ifndef EI_WIDGETTYPES_H
#define EI_WIDGETTYPES_H

#include "ei_widget.h"


//Definition du type frame
typedef struct ei_frame_t {
        ei_widget_t widget;
        ei_color_t bg_color;
        int border_width; 
        ei_relief_t relief;
        char* text;
        ei_font_t text_font;
        ei_color_t text_color;
        ei_anchor_t text_anchor;
        ei_surface_t img;
        ei_rect_t* img_rect;
        ei_anchor_t img_anchor;
        /*struct {bool is_txt; 
          union{ char* txt;
          uint32_t* img;
          } type;
          } foreground;*/

        // POSITIONNEMENT
        // SOUS RECTANGLE page 19
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
typedef struct ei_top_btn_t {
	ei_widget_t widget;
} ei_top_btn_t;

typedef struct ei_toplevel_t {
        ei_widget_t widget;
        ei_color_t	color;
        int		border_width;
        char*		title;
        ei_bool_t	closable;
        ei_axis_set_t	resizable;
        ei_size_t*	min_size;
		  ei_top_btn_t btn_close;
		  ei_top_btn_t btn_resize;
} ei_toplevel_t;



#endif

