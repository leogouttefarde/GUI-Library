/**
 *  @file       ei_core.h
 *  @brief      Manages the update rectangles
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 05.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 */

#ifndef EI_CORE_H
#define EI_CORE_H

#include "ei_common.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_linkedlist.h"


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
void ei_invalidate_rect(ei_rect_t* invalid_rect);

/**
 * \brief       Updates the list of invalid rectangles on the root surface.
 */
void ei_draw_rects();

/**
 * \brief       Draws a rectangle using ei_fill inside the given clipper.
 *
 * @param       surface         The surface to be filled. The surface must be *locked* by
 *                              \ref hw_surface_lock.
 * @param       color           The color used to fill the surface. If NULL, it means that the
 *                              caller want it painted black (opaque).
 * @param       rect            The rectangle to draw.
 * @param       clipper         If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_rectangle(ei_surface_t surface, ei_color_t *color, ei_rect_t *rect, ei_rect_t *clipper);

/**
 * \brief       ei_linkedlist_t callback function.
 *              Used by ei_draw_rects to draw a rectangle on screen. 
 *
 * @param       link            Linked list element
 * @param       user_param      User parameters
 *
 * @return                      A boolean telling if this callback must stop being called on other list elements.
 *                              If EI_TRUE, the callback will not be called on them, if EI_FALSE it will.
 */
ei_bool_t ei_draw_rect(ei_linked_elem_t *link, void *user_param);

/**
 * \brief       Draws a widget on the root surface for the given draw_rect rectangle to update.
 */
void ei_draw_widget(ei_widget_t *widget, ei_rect_t *draw_rect);

/**
 * \brief       Indicates if there is an intersection between two rectangles.
 *
 * @param       rect1   First rectangle
 * @param       rect2   Second rectangle
 *
 * @return      Intersection boolean.
 */
ei_bool_t ei_is_rect_inter(const ei_rect_t *rect1, const ei_rect_t *rect2);

/**
 * \brief       Calculates the intersection between two rectangles, if any.
 *
 * @param       rect1   First rectangle
 * @param       rect2   Second rectangle
 *
 * @return      The resulting intersection rectangle, or NULL if none.
 */
ei_rect_t* ei_rect_intersection(const ei_rect_t *rect1, const ei_rect_t *rect2);

/**
 * \brief       Calculates the fusion between two rectangles if
 *              it is a smaller or equal area than both rectangle areas added.
 *
 * @param       rect1   First rectangle
 * @param       rect2   Second rectangle
 *
 * @return      The resulting fusion rectangle, or NULL if bigger.
 */
ei_rect_t* ei_smaller_fused(const ei_rect_t *rect1, const ei_rect_t *rect2);


#endif
