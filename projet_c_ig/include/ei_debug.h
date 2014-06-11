/**
 *  @file	ei_debug.h
 *  @brief	Debug
 *
 *  \author 
 *  Created by Léo Gouttefarde on 11.06.14
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */
#ifndef EI_DEBUG_H
#define EI_DEBUG_H

#include "ei_types.h"


void ei_print_color(const ei_color_t *color);
void ei_print_rect(const ei_rect_t *rect);
void ei_print_rects(ei_linked_rect_t *lrect);

void debug_display_picking_surface();
void debug_display_root_surface();


#endif
