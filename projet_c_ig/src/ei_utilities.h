/**
 *  @file       ei_utilities.h
 *  @brief      Utility functions.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 04.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#ifndef EI_UTILITIES_H
#define EI_UTILITIES_H

#include "ei_common.h"
#include "ei_types.h"


typedef enum {
	complet = 0,
	haute = 1,
	basse = -1
} part_rect_t;

ei_linked_point_t* ei_button_arc(ei_point_t centre,int rayon,int angle_debut,int angle_fin,ei_linked_point_t* suivant);

ei_linked_point_t* trait(ei_point_t queue, ei_point_t tete, ei_linked_point_t* suivant);

ei_linked_point_t* ei_button_rounded_frame(ei_rect_t rectangle, int rayon, part_rect_t partie);

ei_point_t plus(ei_point_t A, int abc, int ord);

void free_lp(ei_linked_point_t* Liste);

ei_rect_t reduction(ei_rect_t rectangle, int marge);

ei_color_t eclaircir(ei_color_t couleur, float coeff_couleur);

ei_color_t obscurcir(ei_color_t couleur, float coeff_couleur);

ei_point_t find_anchor(ei_rect_t rectangle,int width,int height, ei_anchor_t position);

void anchor_translation(ei_anchor_t *anchor);
	
static inline void make_string_copy(char **dest_ptr, const char *src)
{
        if (dest_ptr && src) {
                const uint32_t src_size = strlen(src);
                const uint32_t src_size_full = src_size + 1;
                char *dest = MALLOC(src_size_full);

                SAFE_FREE(*dest_ptr);
                *dest_ptr = dest;

                if (dest)
                        strncpy(dest, src, src_size_full);
        }
}

#endif
