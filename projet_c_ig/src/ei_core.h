/**
 *  @file	ei_core.h
 *  @brief	Manages the update rectangles
 *
 *  \author 
 *  Created by Léo GOUTTEFARDE on 05.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#ifndef EI_CORE_H
#define EI_CORE_H

#include "ei_common.h"
#include "ei_types.h"
#include "ei_widget.h"


/**
 * \brief       Initializes core variables.
 */
void ei_init();

/**
 * \brief       Resets the list of rectangles that must be updated on screen.
 */
void ei_invalidate_reset();

/**
 * \brief       Getter for the list of rectangles that must be updated on screen.
 *
 * @return      The list of rectangles that must be updated on screen
 */
ei_linked_rect_t* ei_get_update_rects();

/**
 * \brief       Adds a rectangle to the list of rectangles that must be updated on screen. The real
 *              update on the screen will be done at the right moment in the main loop.
 *
 * @param       rect            The rectangle to add, expressed in the root window coordinates.
 *                              A copy is made, so it is safe to release the rectangle on return.
 */
void ei_invalidate_rect(ei_rect_t* rect);

/**
 * \brief       Updates the list of invalid rectangles on the root surface.
 */
void ei_draw_rects();

/**
 * \brief       Draws a widget on the root surface for the given draw_rect rectangle to update.
 */
void ei_draw_widget(ei_widget_t *widget, ei_rect_t *draw_rect);

/**
 * \brief       Indicates if there is an intersection between two rectangles.
 *
 * @param       rect1   First rectangle
 * @param       rect1   Second rectangle
 *
 * @return      Intersection boolean.
 */
ei_bool_t ei_is_rect_inter(const ei_rect_t *rect1, const ei_rect_t *rect2);

/**
 * \brief       Calculates the intersection between two rectangles, if any.
 *
 * @param       rect1   First rectangle
 * @param       rect1   Second rectangle
 *
 * @return      The resulting intersection rectangle, or NULL if none.
 */
ei_rect_t* ei_rect_intersection(const ei_rect_t *rect1, const ei_rect_t *rect2);


#endif
