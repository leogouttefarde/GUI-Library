#ifndef EI_ENTRY_H
#define EI_ENTRY_H

#include "ei_types.h"
#include "ei_widgettypes.h"

typedef struct ei_entry_t {
			ei_widget_t widget;
			ei_bool_t top_entry;
			struct ei_entry_t* next_entry;
			char* txt;
			ei_font_t font;
			int border_width;
} ei_entry_t;

void reinit_top_entry(ei_widget_t *widget);

ei_widget_t *find_top_entry(ei_widget_t *widget);

void ei_entry_draw(ei_surface_t surface, ei_rect_t rectangle, ei_entry_t *entry, ei_rect_t* clipper);

#endif
