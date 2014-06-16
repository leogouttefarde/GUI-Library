#ifndef EI_ENTRY_H
#define EI_ENTRY_H

#include "ei_types.h"
#include "ei_widgettypes.h"

void reinit_top_entry(ei_widget_t *widget);

ei_widget_t *find_top_entry(ei_widget_t *widget);

void ei_entry_draw(ei_surface_t surface, ei_rect_t rectangle, ei_entry_t *entry, ei_rect_t* clipper);
#endif
