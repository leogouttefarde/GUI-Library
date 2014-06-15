/**
 *  @file       ei_gridder.h
 *
 *  @brief      Functions associated with gridder.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 13.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 */

#ifndef EI_GRIDDER_H
#define EI_GRIDDER_H

#include "ei_geometrymanager.h"
#include "ei_utils.h"
#include <string.h>
#include <math.h>
#include "ei_common.h"
#include "ei_core.h"
#include "ei_params.h"

// Gestion des paramètres
void ei_grid(ei_widget_t *widget, int *col, int *lin, int *w, int *h, int
                *force_w, int *force_h);


/**
 * @brief   Gridder runfunc, calculate the screen_location
 *                  
 * @param   widget        Widget to grid
 */
void ei_grid_runfunc(ei_widget_t *widget);


/**
 * @brief   Gridder releasefunc, release the geometrical parameters
 *                  
 * @param   widget        Widget whose geometry is to destroy
 */
void ei_grid_releasefunc(struct ei_widget_t* widget);

/**
 * \brief       Registers the "gridder" geometry manager in the program. This must be called only
 *              once before the \ref ei_grid function can be called.
 */
void  ei_register_gridder_manager();

#endif
