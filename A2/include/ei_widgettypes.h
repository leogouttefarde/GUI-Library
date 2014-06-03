#ifndef EI_WIDGETTYPES_H
#define EI_WIDGETTYPES_H
// pour avoir le type ei_widget
#include "ei_widget.h"
//Definition du type frame
typedef struct ei_frame_t {
        ei_widget_t widget;
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


#endif
