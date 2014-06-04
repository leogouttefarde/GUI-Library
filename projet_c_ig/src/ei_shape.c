/**
 * @file	ei_shape.h
 *
 * @brief 	Basical geometric transformation and shape generating
 *
 *  Created by Eric BUREL on 04.06.14
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */
#include "string.h"
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
                current = current->next;
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
void ei_translate_point(ei_point_t *pt, ei_point_t u){
        pt->x = pt->x + u.x;
        pt->y = pt->y + u.y;
}

// un vecteur est représenté par le type point
void ei_translate(ei_linked_point_t *lp, ei_point_t u){
        ei_linked_point_t *current;
        current = lp;
        while(current){
                ei_translate_point(&(current->point), u);
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
        float y = (float)(max.y + min.y) / 2.; 
        ei_linked_point_t *current;
        current = lp;
        while (current){
                // seul y varie 
                current->point.y = current->point.y - (int)(2.*((float)current->point.y - y));
                current = current->next;
        }

}

void ei_sym_vert(ei_linked_point_t *lp){
        ei_point_t max;
        ei_point_t min;
        max = ei_search_max(*lp);
        min = ei_search_min(*lp);
        float x = (float)(max.x + min.x) / 2.; 
        ei_linked_point_t *current;
        current = lp;
        while (current){
                // seul y varie 
                current->point.x = current->point.x - (int)(2.*((float)current->point.x - x));
                current = current->next;
        }
}


void ei_append_point(ei_linked_point_t *lp, ei_point_t point) {
        ei_linked_point_t *prec;
        ei_linked_point_t *current;
        current = lp;
        while(current) {
                prec = current;
                current = current->next;
        }
        if (prec){
                prec->next = malloc(sizeof(ei_linked_point_t));
                prec->next->point = point;
        }
}

void ei_direct_append(ei_linked_point_t *lp, ei_point_t point){
        lp->next = malloc(sizeof(ei_linked_point_t));
        lp->next->point = point;
        lp->next->next = NULL;
}


// Transforme un rectangle en une liste de point
// premier point = top left, puis sens horaire
ei_linked_point_t ei_rect_to_points(ei_rect_t rect){
        // cf minimal.c pour un exemple d'utilisation des tableaux
        int w = rect.size.width;
        int h = rect.size.height;
        ei_linked_point_t res;
        ei_linked_point_t *current;
        ei_point_t tl = rect.top_left;
        ei_point_t tr = {tl.x + w, tl.y};
        ei_point_t br = {tr.x, tr.y+h};
        ei_point_t bl = {br.x - w, br.y};

        res = (ei_linked_point_t){tl, NULL};
        current = &res;
        ei_direct_append(current, tr);
        current = current->next;
        ei_direct_append(current, br);
        current=current->next;
        ei_direct_append(current, bl);

        return res;
}

/*************** Frame related functions *******/

// Genere un morceau de cadre avec l'orientation voulue
// size est la taille totale (bordure incluse !)
ei_linked_point_t ei_relief(ei_point_t top_left, char* side, ei_size_t size, int
                bw){

        ei_linked_point_t result;

        if (!strcmp(side,"bottom")){
                result = ei_relief(top_left, "top", size, bw);
                ei_sym_horiz(&result);
                return result;
                ei_point_t u = {0,size.height-bw};
                ei_translate(&result,u);
        } 
        else if (!strcmp(side,"right")){
                result = ei_relief(top_left, "left", size, bw);
                ei_sym_vert(&result);        
        }
        else {  
                ei_linked_point_t *current;
                int w;
                int h;
                ei_point_t tmp;
                if (!strcmp(side, "top")) {
                        /*  hauteur : bw      X---------X
                         *  longueur : lg      X-------X
                         */
                        w = size.width;
                        h = bw;

                        current = &result;
                        result.point = top_left;
                        // top_right
                        tmp = top_left;
                        ei_translate_point(&tmp, (ei_point_t){w,0});
                        ei_direct_append(current, tmp);
                        current = current->next;
                        // bottom_right
                        ei_translate_point(&tmp, (ei_point_t){-h, +h});
                        ei_direct_append(current, tmp);
                        current = current->next;
                        //bottom_left;
                        ei_translate_point(&tmp, (ei_point_t){-w+2*h, 0});
                        current->next = NULL;
                }
                else if (!strcmp(side, "left"))
                {

                        /*  hauteur : size.height      X-----X
                         *  longueur : bw              X-------X
                         *                             X-----X
                         */

                        w = bw;
                        h = size.height;

                        current = &result;
                        result.point = top_left;
                        // top_right
                        tmp = top_left;
                        ei_translate_point(&tmp, (ei_point_t){w,w});
                        ei_direct_append(current, tmp);
                        current = current->next;
                        // bottom_right
                        ei_translate_point(&tmp, (ei_point_t){0, h-2*w});
                        ei_direct_append(current, tmp);
                        current = current->next;
                        //bottom_left;
                        ei_translate_point(&tmp, (ei_point_t){-w, w});
                        current->next = NULL;
                }
        }

        return result;
}
