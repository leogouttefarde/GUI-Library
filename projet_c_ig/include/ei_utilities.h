#ifndef EI_UTILITIES_H
#define EI_UTILITIES_H

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


static inline void make_string_copy(const char **dest, const char *src)
{
        const uint32_t src_size = strlen(src);
        const uint32_t src_size_f = src_size + 1;
        *dest = malloc(src_size_f);

        if (*dest)
                strncpy(*dest, src, src_size);
}

#endif
