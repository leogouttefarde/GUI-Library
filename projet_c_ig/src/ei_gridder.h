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

#include "ei_widget.h"


/**
 * \brief       Configures the geometry of a widget using the "gridder" geometry manager.
 *              If the widget was already managed by another geometry manager, then it is first
 *              removed from the previous geometry manager.
 *              If the widget was already managed by the "gridder", then this calls simply updates
 *              the gridder parameters: arguments that are not NULL replace previous values.
 *              When the arguments are passed as NULL, the gridder uses default values.
 *              If no size is provided (either absolute or relative), then the requested size
 *              of the widget is used, i.e. the minimal size required to display its content.
 *
 * @param   widget        Widget to grid
 *
 * @param   col           Column
 *
 * @param   lin           Line
 *
 * @param   w             Width
 *
 * @param   h             Height
 *
 * @param   force_w       Forces the width
 *
 * @param   force_h       Forces the height
 *
 */
void ei_grid(ei_widget_t *widget, int *col, int *lin,
             int *w, int *h, int *force_w, int *force_h);

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
