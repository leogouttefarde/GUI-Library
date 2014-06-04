#ifndef EI_SHAPE_H
#define EI_SHAPE_H
/**
 * @file	ei_shape.h
 *
 * @brief 	Basical geometric transformation and shape generating
 *
 *  Created by Eric BUREL on 04.06.14
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */
#include "ei_types.h"
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


void ei_translate(ei_linked_point_t *lp, ei_point_t u);

// Translation de (-min.x, -max.y) => (0,0) est le "top_left"
void ei_center(ei_linked_point_t *lp);

// Rotation autour d'un point donné
// a eviter a cause de la precision (rotation flottante contre coord entieres)
void ei_rotate(ei_linked_point_t *lp, ei_point_t center, float theta);

void ei_sym_horiz(ei_linked_point_t *lp);

void ei_sym_vert(ei_linked_point_t *lp);
// Transforme un rectangle en une liste de point
ei_linked_point_t ei_rect_to_points(ei_rect_t rect);


/*************** Frame related functions *******/

// Genere un morceau de cadre avec l'orientation voulue
// size est la taille totale (bordure incluse !)
ei_linked_point_t ei_relief(ei_point_t top_left, char* side, ei_size_t size, int
                border_width);





#endif
