#ifndef EI_BUTTON_H
#define EI_BUTTON_H


#include "ei_types.h"

ei_linked_point_t* ei_button_arc(ei_point_t centre,int rayon,int angle_debut,int angle_fin);

ei_linked_point_t* ei_button_rounded_frame(ei_rect_t rectangle);
