#ifndef EI_SHAPE_H
#define EI_SHAPE_H
/**
 * @file	ei_shape.h
 *
 * @brief 	Basic geometrical transformations and shape generating for
 *              classes
 *
 *  Created by Eric BUREL on 04.06.14
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */
#include "ei_types.h"
#include "ei_utils.h"
//**************** Basic functions ****************/


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
