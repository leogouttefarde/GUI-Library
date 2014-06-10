#ifndef EI_BUTTON_H
#define EI_BUTTON_H


#include "ei_types.h"
#include "hw_interface.h"
#include "ei_widgettypes.h"

typedef enum {
	complet = 0,
	haute = 1,
	basse = -1
} part_rect_t;

ei_linked_point_t* ei_button_arc(ei_point_t centre,int rayon,int angle_debut,int angle_fin,ei_linked_point_t* suivant);

ei_linked_point_t* trait(ei_point_t queue, ei_point_t tete, ei_linked_point_t* suivant);

ei_linked_point_t* ei_button_rounded_frame(ei_rect_t rectangle, int rayon, part_rect_t partie);

void ei_button_draw(ei_surface_t window, ei_rect_t rectangle, ei_button_t *button, ei_rect_t *clipper);

void ei_bar_draw(ei_surface_t surface, ei_toplevel_t *toplevel, ei_rect_t *clipper);

void ei_button_draw_loc(ei_surface_t window,
                ei_rect_t rectangle,
                ei_color_t couleur,
                ei_relief_t relief,
                int rayon,
                int marge,
                ei_rect_t *clipper);

void ei_button_text(ei_surface_t window,ei_rect_t clipper,char* text, ei_font_t font,ei_color_t color, ei_anchor_t anchor);

void free_lp(ei_linked_point_t* Liste);

ei_point_t plus(ei_point_t A, int abc, int ord);

void aff_img(ei_surface_t window, ei_rect_t rectangle,ei_surface_t img, ei_rect_t* img_rect, ei_anchor_t img_anchor);

void ei_frame_draw(ei_surface_t window,ei_rect_t rectangle,ei_frame_t* frame,
                ei_rect_t *clipper);

ei_rect_t reduction(ei_rect_t rectangle, int marge);

ei_color_t eclaircir(ei_color_t couleur, float coeff_couleur);

#endif
