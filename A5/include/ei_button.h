#ifndef EI_BUTTON_H
#define EI_BUTTON_H


#include "ei_types.h"
#include "hw_interface.h"

typedef enum {
	complet = 0,
	haute = 1,
	basse = -1
} part_rect_t;

ei_linked_point_t* ei_button_arc(ei_point_t centre,int rayon,int angle_debut,int angle_fin,ei_linked_point_t* suivant);

ei_linked_point_t* trait(ei_point_t queue, ei_point_t tete, ei_linked_point_t* suivant);

ei_linked_point_t* ei_button_rounded_frame(ei_rect_t rectangle, int rayon, part_rect_t partie);

void ei_button_draw(ei_surface_t window, ei_rect_t rectangle, ei_bouton_t button);

void ei_button_draw_loc(ei_surface_t window,
	  	ei_rect_t rectangle,
		ei_color_t couleur,
		ei_relief_t relief,
		int rayon,
		int marge);

void ei_button_text(char* text, ei_font_t text_font,ei_anchor_t);
	
void free_lp(ei_linked_point_t* Liste);

#endif
