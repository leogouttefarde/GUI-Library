/**
 * @file        ei_shape.h
 *
 * @brief       Basical geometric transformation and shape generating
 *
 *  Created by Eric BUREL on 04.06.14
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */
#include "ei_shape.h"
#include "ei_common.h"

//**************** Basic functions ****************/



void ei_direct_append(ei_linked_point_t *lp, ei_point_t point)
{
        lp->next = CALLOC_TYPE(ei_linked_point_t);
        assert(lp->next);

        lp->next->point = point;
        lp->next->next = NULL;
}

// Transforme un rectangle en une liste de point
// premier point = top left, puis sens horaire
ei_linked_point_t ei_rect_to_points(ei_rect_t rect)
{
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

