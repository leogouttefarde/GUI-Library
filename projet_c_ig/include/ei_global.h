/**
 *  @file	ei_global.h
 *  @brief	Contains the root widget address, the root surface and the
 *              picking surface as global variables.
 *              Strangely looks like a Java class.
 *
 *  \author 
 *  Created by Eric BUREL on 05.06.14
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */
#ifndef EI_GLOBAL_H
#define EI_GLOBAL_H

#include <stdbool.h>
#include <stdlib.h>
#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"



void ei_set_root(ei_widget_t *root_set);
void ei_set_root_surface(ei_surface_t root_surface_set);
void ei_set_picking_surface(ei_surface_t picking_surface_set);

ei_widget_t* ei_get_root();
ei_surface_t ei_get_root_surface();
ei_surface_t ei_get_picking_surface();

ei_surface_t* ei_get_root_surface_pointer();
ei_surface_t* ei_get_picking_surface_pointer();


void debug_display_picking_surface();

#endif
