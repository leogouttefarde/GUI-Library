/**
 *  @file	ei_core.h
 *  @brief	Manages the update rectangles
 *
 *  \author 
 *  Created by Leo GOUTTEFARDE on 05.06.14
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */
#ifndef EI_CORE_H
#define EI_CORE_H

#include "ei_common.h"
#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_root.h"

void ei_init();



void ei_invalidate_reset();
ei_linked_rect_t* ei_get_update_rects();
void ei_invalidate_rect(ei_rect_t* rect);
void ei_draw_rects();
void ei_draw_widget(ei_widget_t *widget, ei_rect_t *draw_rect);
ei_bool_t ei_is_rect_inter(const ei_rect_t *rect1, const ei_rect_t *rect2);
ei_rect_t* ei_rect_intersection(const ei_rect_t *rect1, const ei_rect_t *rect2);

#endif
