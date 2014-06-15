/**
 * @file	ei_shape.h
 *
 * @brief       At first designed to contain basical geometrical operations, now
 *              only contains a rectangle/point_list converter.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 04.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#ifndef EI_SHAPE_H
#define EI_SHAPE_H

#include "ei_types.h"
#include "ei_utils.h"


/**
 * @brief       Convert a rectangle in a list of point, top_left is the first
 *              then clockwise cross    
 *
 * @param   rect        The rectangle to converte
 *
 * @return  The corresponding point list
 */
ei_linked_point_t ei_rect_to_points(ei_rect_t rect);

/**
 * @brief       Brutally replace lp->next with point. lp->next->next is NULL    
 *
 * @param   lp        Usually the address of the list last element
 *
 * @param   point       The point to apppend
 */
void ei_direct_append(ei_linked_point_t *lp, ei_point_t point);

#endif
