/**
 *  @file	ei_global.h
 *  @brief	Contains the root widget address, the root surface and the
 *              picking surface as global variables.
 *              Strangely looks like a Java class.
 *
 *  \author 
 *  Created by Eric BUREL on 05.06.14
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */
#ifndef EI_GLOBAL_H
#define EI_GLOBAL_H

#include "ei_common.h"
#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"


void ei_init();

void ei_set_root(ei_widget_t *root_set);
void ei_set_root_surface(ei_surface_t root_surface_set);
void ei_set_picking_surface(ei_surface_t picking_surface_set);

ei_widget_t* ei_get_root();
ei_surface_t ei_get_root_surface();
ei_surface_t ei_get_picking_surface();

ei_surface_t* ei_get_root_surface_pointer();
ei_surface_t* ei_get_picking_surface_pointer();


void ei_invalidate_reset();
ei_linked_rect_t* ei_get_update_rects();
void ei_invalidate_rect(ei_rect_t* rect);
void ei_draw_rects();
void ei_draw_widget(ei_widget_t *widget);
ei_bool_t ei_is_rect_inter(const ei_rect_t *rect1, const ei_rect_t *rect2);
ei_rect_t* ei_rect_intersection(const ei_rect_t *rect1, const ei_rect_t *rect2);

#endif
