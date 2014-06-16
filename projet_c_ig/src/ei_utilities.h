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

ei_linked_point_t* ei_button_arc(ei_point_t center, int radius, int head_angle,
                                 int tail_angle, ei_linked_point_t *next);

ei_linked_point_t* trait(ei_point_t queue, ei_point_t tete, ei_linked_point_t *suivant);

/**
 * @brief Converts a rectangle in a list of linked point, which represent the rounded countours of the rectangle, or you can choose to get only the top or bottom part of this rectangle
 *
 * @param rectangle The original rectangle
 * @param rayon The radius of the rounded angles
 * @param partie Which part of the rectangle -1=>bot,0=>complete,1=>top
 *
 * @return The head of a list of linked point
 */
ei_linked_point_t* ei_button_rounded_frame(ei_rect_t rectangle, int rayon, part_rect_t partie);

/**
 * @brief       Adds x / y coordinates to a point.
 *
 * @param       p       The point
 * @param       x       X coordinate
 * @param       y       Y coordinate
 *
 * @return              The resulting point.
 */
ei_point_t plus(ei_point_t p, int x, int y);

/**
 *  @brief Frees a linked list of points.
 *
 *  @param The list to free.
 */
void free_lp(ei_linked_point_t *Liste);

/**
 * @brief Reduces a rectangle.
 *
 * @param       rectangle       The rectangle to reduce
 * @param       marge           The reduction size
 *
 * @return                      The resulting rectangle
 */
ei_rect_t reduction(ei_rect_t rectangle, int marge);

/**
 * @brief       Brightens a color.
 *
 * @param       couleur         The color to brighten.
 * @param       coeff_couleur   The light ratio to add.
 *
 * @return                      The new color
 */
ei_color_t eclaircir(ei_color_t couleur, float coeff_couleur);

/**
 * @brief       Darkens a color.
 *
 * @param       couleur         The color to darken.
 * @param       coeff_couleur   The darkness ratio to add.
 *
 * @return                      The new color
 */
ei_color_t obscurcir(ei_color_t couleur, float coeff_couleur);

/**
 * @brief       Find the point associate to a rectangle and
 *              two constraints from the anchor position.
 *
 * @param       rectangle       The rectangle.
 * @param       width           The width constraint.
 * @param       height          The height constraint.
 * @param       position        The position needed.
 *
 * @return                      The anchor's coordinates.
 */
ei_point_t find_anchor(ei_rect_t rectangle, int width, int height, ei_anchor_t position);

/**
 * @brief       Called when the size of the text is bigger than the place allocated,
 *              so it translates the anchor in order to display the start of the text.
 *
 * @param       anchor  The anchor to translate.
 *
 * @return              The resulting anchor.
 */
ei_anchor_t anchor_translation(ei_anchor_t anchor);

/**
 * @brief       Reallocates a string on a string pointer.
 *
 * @param       dest_ptr        Destination string's pointer
 * @param       src             String to copy by reallocation
 *
 * @warning     If the destination's string is not NULL, it will be freed first.
 */
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
