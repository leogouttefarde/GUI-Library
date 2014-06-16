/**
 *  @file       ei_widgetclass_pv.h
 *  @brief      Private widgetclass declarations
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


/*
 * \brief       Frees all widgetclasses.
 */
void ei_widgetclass_free();

/*
 * \brief       Indicates if a widget belongs to a specified widgetclass.
 */
ei_bool_t ei_has_widgetclass(ei_widget_t *widget, ei_widgetclass_name_t name);

/**
 * \brief       Registers the "radiobutton" widget class in the program. This must be called only
 *              once before widgets of the class "radiobutton" can be created and configured with
 *              \ref ei_button_configure.
 */
void ei_radiobutton_register_class();

/**
 * \brief       Registers the "entry" widget class in the program. This must be called only
 *              once before widgets of the class "entry" can be created and configured with
 *              \ref ei_entry_configure.
 */
void ei_entry_register_class ();

#endif
