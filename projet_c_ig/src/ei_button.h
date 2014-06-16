/**
 *  @file       ei_button.h
 *  @brief      Button API
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 14.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include "ei_types.h"
#include "hw_interface.h"
#include "ei_widgettypes.h"


void ei_frame_draw(ei_surface_t window, ei_rect_t rectangle, ei_frame_t *frame, ei_rect_t *clipper);

void ei_button_draw(ei_surface_t window, ei_rect_t rectangle, ei_button_t *button, ei_rect_t *clipper);

void ei_toplevel_draw(ei_surface_t surface, ei_toplevel_t *toplevel, ei_rect_t *clipper);

/**
 * \brief       Trace un bouton en relief (enfoncé ou relevé) ou plat
 * @param       window          Window surface sur laquelle on dessine le bouton
 * @param       rectangle       Rectangle le rectangle que l'on transforme en bouton
 * @param       couleur         Couleur la couleur du bouton central
 * @param       relief          Relief si raised =>relevé,none=>plat,sinon enfoncé
 * @param       rayon           Le rayon des angles
 * @param       marge           La marge
 * @param       clipper         Le clipper qui restreint le bouton
 */
void ei_button_draw_loc(ei_surface_t window,
                ei_rect_t rectangle,
                ei_color_t couleur,
                ei_relief_t relief,
                int rayon,
                int marge,
                ei_rect_t *clipper);

void ei_insert_text(ei_surface_t window,ei_rect_t rectangle,char* text, ei_font_t font,ei_color_t color, ei_anchor_t anchor, ei_rect_t* clipper);



#endif
