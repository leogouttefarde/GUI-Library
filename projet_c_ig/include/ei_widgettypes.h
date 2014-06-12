
#ifndef EI_WIDGETTYPES_H
#define EI_WIDGETTYPES_H

#include "ei_geometrymanager.h"
#include "ei_widget.h"
#include <stdbool.h>


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
        ei_color_t	*color;
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
        // Pour la gestion du release
        bool clic;
} ei_button_t;

typedef struct ei_toplevel_t {
        ei_widget_t widget;
        ei_color_t	color;
        //Barre de titre
		  int bar_height;
        int		border_width;
		  ei_color_t bar_color;
		  //titre
        char*		title;
        ei_font_t	title_font;
		  ei_color_t title_color;
		//bouton close
        ei_bool_t	closable;
		  ei_relief_t rel_btn_close;
		  //bouton resize
        ei_axis_set_t	resizable;
        int             resize_size;
		  //
        ei_size_t*	min_size;
        // Pour le calcul du deplacement
        ei_point_t move_pos;
        // Pour detecter une demande de fermeture
        ei_bool_t   close;
        // Booleans pour detecter les différents release << Useless
        // bool            move;
        // bool             resize;
} ei_toplevel_t;

/**
 * \brief       Placer geometry manager
 */
typedef struct ei_placer_param_t {
        ei_geometry_param_t geom;       /* ei_geometry_param_t child */
        ei_anchor_t* anc;
        int* x;
        int* y;
        float* rel_x;
        float* rel_y;
        int* w;
        int* h;
        float* rel_w;
        float* rel_h;
} ei_placer_param_t;

typedef struct ei_gridder_param_t {
        ei_geometry_param_t geom;

} ei_gridder_param_t;

#endif

