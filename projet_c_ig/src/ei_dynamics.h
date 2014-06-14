/**
 *  @file	ei_dynamics.h
 *
 *  @brief       Dynamic functions for moving and resizing either gridder managed widget or placer managed widgets.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 13.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 */

#ifndef EI_DYNAMICS_H
#define EI_DYNAMICS_H

#include "ei_types.h"
#include "ei_widget.h"
#include "ei_widgettypes.h"
#include "ei_params.h"
#include "ei_common.h"
#include "ei_utils.h"
#include "ei_gridder.h"
#include <stdlib.h>
#include <assert.h> 

/**
 * @brief   Select the resizing function depending on the widget geometry
 *          manager and applies it
 *                  
 * @param   widget        Widget to resize
 * @param   where         Last position of the cursor
 */
void resize(ei_widget_t *widget, ei_point_t where);

/**
 * @brief   Select the movement function depending on the widget geometry
 *          manager and applies it
 *                  
 * @param   widget        Widget to move
 * @param   where         Last position of the cursor
 */
void move(ei_widget_t *widget, ei_point_t where);


#endif
