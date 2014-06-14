/**
 *  @file	widgetclass_pv.h
 *  @brief	Private widgetclass declarations
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 12.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#ifndef EI_WIDGETCLASS_PV_H
#define EI_WIDGETCLASS_PV_H

#include "ei_widgetclass.h"
#include "ei_widget.h"



void ei_widgetclass_free();
ei_bool_t ei_has_widgetclass(ei_widget_t *widget, ei_widgetclass_name_t name);


#endif
