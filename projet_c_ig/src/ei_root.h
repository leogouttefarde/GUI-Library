/**
 *  @file	ei_root.h
 *  @brief	Contains the root widget address, the root surface and the
 *              picking surface as global variables.
 *              Strangely looks like a Java class.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 05.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#ifndef EI_ROOT_H
#define EI_ROOT_H

#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_common.h"
/**
 * @brief   Set root to the widget in argument
 *                  
 * @param   root_set        Pointer to the root
 */
void ei_set_root(ei_widget_t *root_set);
/**
 * @brief   Set the root surface
 *                  
 * @param   root_surface_set    Root surface
 */
void ei_set_root_surface(ei_surface_t root_surface_set);
/**
 * @brief   Set the picking surface
 *                  
 * @param   picking_surface_set    Picking surface
 */
void ei_set_picking_surface(ei_surface_t picking_surface_set);

/**
 * @brief   Return a pointer to the root widget
 *                  
 * @return  Pointer to the root widget
 */
ei_widget_t* ei_get_root();

/**
 * @brief   Return the root surface
 *                  
 * @return  Root surface
 */
ei_surface_t ei_get_root_surface();

/**
 * @brief   Return the picking surface
 *                  
 * @return  Picking surface
 */
ei_surface_t ei_get_picking_surface();


#endif
