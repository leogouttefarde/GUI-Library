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
#include "math.h"
//**************** Basic functions ****************/

/**
 * @brief	Search fictive max point where max.x(resp. y) = maximal abciss (resp. ordinate) of points
 *              in the list
 *
 * @param	l		The list to search in
 *
 * @return	The max point		
 */
ei_point_t ei_search_max(ei_linked_point_t l);


/**
 * @brief	Same as ei_search_max with the min
 *
 *
 * @param	l		The list to search in
 *
 * @return	The min point		
 */
ei_point_t ei_search_min(ei_linked_point_t l);

/**
 * @brief	Search the size of the smallest rectangle around the points and
 *              returns its size.
 *
 *
 * @param	l		The list to search in
 *
 * @return	The size		
 */
ei_size_t ei_search_size(ei_linked_point_t l);

/**
 * @brief	Translate a point
 *
 *
 * @param	pt	Point to translate address
 *
 * @param       u       Translation vector (represented as a point)
 *		
 */
void ei_translate_point(ei_point_t *pt, ei_point_t u);

/**
 * @brief	Translate a list of points
 *
 *
 * @param	pt	List to translate address
 *
 * @param       u       Translation vector (represented as a point)
 *		
 */
void ei_translate(ei_linked_point_t *lp, ei_point_t u);

/**
 * @brief       New top_left fictive point is 0. Useful for
 *              rotations.	
 *
 *
 * @param	lp	List to center adress
 *
 *		
 */
void ei_center(ei_linked_point_t *lp);

/**
 * @brief       Rotate a point. Beware of floating numbers limited
 * precision
 *
 *
 * @param   lp  	Point to rotate adress
 * 
 * @param   center      Rotation center
 *
 * @param   theta       Rotation angle    
 *
 */
void ei_point_rotate(ei_linked_point_t pt, ei_point_t center, float theta);

/**
 * @brief       Rotate a list of point. Beware of floating numbers limited
 * precision
 *
 *
 * @param   lp  	List to rotate adress
 * 
 * @param   center      Rotation center
 *
 * @param   theta       Rotation angle    
 *
 */
void ei_rotate(ei_linked_point_t *lp, ei_point_t center, float theta);


/**
 * @brief       Horizontal symmetry, the axe crosses the middle of the fictive
 *  wrapping rectangle
 *
 * @param   lp  	List to symmetrize adress
 *
 */
void ei_sym_horiz(ei_linked_point_t *lp);


/**
 * @brief       Same as \ref ei_sym_horiz but vertical    
 *
 * @param   lp  	List to symmetrize adress
 *
 */
void ei_sym_vert(ei_linked_point_t *lp);

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

/*************** Frame related functions *******/

/**
 * @brief       Generete a part of the relief border of a frame
 *
 * @param   top_left        Top left point of the frame
 *
 * @param   side            "top", "bottom", "right" or "left"
 *
 * @param   size            Size of the frame (requested_size and NOT clipper size)
 *
 * @param   bw              Border width
 *
 * @return      The point list representing the desired part of the border
 */
ei_linked_point_t ei_relief(ei_point_t top_left, char* side, ei_size_t size, int bw);





#endif
