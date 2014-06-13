/**
 * @file	ei_dynamics.h
 *
 * @brief       Dynamic functions for moving and resizing either gridder managed widget or placer managed widgets.
 * 
 *  Created by Eric BUREL on 13.06.14.
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
void resize(ei_widget_t *widget, ei_point_t where);
void move(ei_widget_t *widget, ei_point_t where);


#endif
