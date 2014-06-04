/**
 * @file	ei_shape.h
 *
 * @brief 	Basical geometric transformation and shape generating
 *
 *  Created by Eric BUREL on 04.06.14
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */
#include "ei_shape.h"
// ne pas oublier les parenthèse, M_PI definie dans math.h
#ifndef M_PI
#define M_PI (3.141592653589793)
#endif
#define RAD_TO_DEG(x) ((x*180.0)/(M_PI)) 
//**************** Basic functions ****************/

/**
 * @brief	Search fictive max point where max.x(resp. y) = maximal abciss (resp. ordinate) of points
 *              in the list
 *
 * @param	l		The list to search in
 *
 * @return	The max point		
 */
ei_point_t ei_search_max(ei_linked_point_t l){
        // initialisation du max
        ei_point_t max;
        max = l.point;

        ei_linked_point_t *current;
        current = l.next;
        while (current){
                if (current->point.x > max.x){
                        max.x = current->point.x;
                }

                if (current->point.y > max.y){
                        max.y = current->point.y;
                }
        }
        return max;
}


/**
 * @brief	Same as ei_search_max with the min
 *
 *
 * @param	l		The list to search in
 *
 * @return	The min point		
 */

ei_point_t ei_search_min(ei_linked_point_t l){
        // initialisation du min
        ei_point_t min;
        min = l.point;

        ei_linked_point_t *current;
        current = l.next;
        while (current){
                if (current->point.x < min.x){
                        min.x = current->point.x;
                }

                if (current->point.y < min.y){
                        min.y = current->point.y;
                }
                current = current->next;
        }
        return min;
}
/**
 * @brief	Search the size of the smallest rectangle around the points and
 *              returns its size.
 *
 *
 * @param	l		The list to search in
 *
 * @return	The size		
 */
ei_size_t ei_search_size(ei_linked_point_t l){
        ei_point_t max;
        ei_point_t min;
        max = ei_search_max(l);
        min = ei_search_min(l);

        ei_size_t s = {max.x-min.x, max.y-min.y};
        return s;
}


// un vecteur est représenté par le type point
void ei_translate(ei_linked_point_t *lp, ei_point_t u){
        ei_linked_point_t *current;
        current = lp;
        while(current){
                current->point.x = current->point.x + u.x;
                current->point.y = current->point.y + u.y;
                current = current->next;
        }
}

// Translation de (-min.x, -max.y) => (0,0) est le "top_left"
void ei_center(ei_linked_point_t *lp){
        // identique a translate hormis que le vecteur est
        // choisi automatiquement
        ei_point_t max;
        ei_point_t min;
        max = ei_search_max(*lp);
        min = ei_search_min(*lp);
        ei_point_t u = {-min.x, -max.y};
        ei_translate(lp, u);
}

// Rotation autour d'un point donné
// THETA EN DEGRES
// A EVITER (PRECISION...)
void ei_rotate(ei_linked_point_t *lp, ei_point_t center, float theta){
        ei_linked_point_t *current;
        current = lp;
        while (current){
                current->point.x = (int)((float)(current->point.x -
                                        center.x))*cos(RAD_TO_DEG(theta)) + center.x;
                current->point.y = (int)((float)(current->point.y -
                                        center.y))*cos(RAD_TO_DEG(theta)) + center.y;
                current = current->next;
        }
}

// la rotation est peu précise, on préfère faire une série de translation
void ei_sym_horiz(ei_linked_point_t *lp){
        ei_point_t max;
        ei_point_t min;
        max = ei_search_max(*lp);
        min = ei_search_min(*lp);
        ei_point_t mid = {(max.x+min.x)/2, (max.y+min.y)/2};

        ei_linked_point_t *current;
        current = lp;
        while (current){
                // seul y varie 
                current->point.y = current->point.y - 2*(current->point.y - mid.y);
                current = current->next;
        }



}

void ei_sym_vert(ei_linked_point_t *lp){
        ei_point_t max;
        ei_point_t min;
        max = ei_search_max(*lp);
        min = ei_search_min(*lp);
        ei_point_t mid = {(max.x+min.x)/2, (max.y+min.y)/2};

        ei_linked_point_t *current;
        current = lp;
        while (current){
                // seul y varie 
                current->point.x = current->point.x - 2*(current->point.x - mid.x);
                current = current->next;
        }
}

// Transforme un rectangle en une liste de point
// premier point = top left, puis sens horaire
ei_linked_point_t ei_rect_to_points(ei_rect_t rect){
        // cf minimal.c pour un exemple d'utilisation des tableaux
        ei_linked_point_t point[4];
        int w = rect.size.width;
        int h = rect.size.height;

        // top_left;
        ei_linked_point_t tl = {rect.top_left, NULL};
        point[0] = tl;
        // top_right
        point[1] = point[0];
        point[1].point.x = point[1].point.x + w;
        point[0].next = &point[1];
        // bottom_right
        point[2] = point[1];
        point[2].point.y = point[2].point.y - h;
        point[1].next = &point[2];
        // bottom_left
        point[3] = point[2];
        point[3].point.x = point[3].point.x - w;
        point[2].next = &point[3];
        point[3].next = NULL;

        return point[0];
}


/*************** Frame related functions *******/

// Genere un morceau de cadre avec l'orientation voulue
// size est la taille totale (bordure incluse !)
ei_linked_point_t ei_relief(ei_point_t top_left, char* side, ei_size_t size, int
                border_width){

        ;
}
