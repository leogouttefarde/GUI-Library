/**
 * @file	ei_callback.h
 *
 * @brief 	Defines the callbacks needed to resize and move toplevels and to click buttons.
 * 
 *  Created by Eric BUREL on 13.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 */

#ifndef     EI_CALLBACK_H
#define     EI_CALLBACK_H
#include "ei_core.h"
#include "ei_widget.h"
#include "ei_widgettypes.h"
#include "ei_params.h"
#include "ei_utils.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_common.h"
#include "ei_dynamics.h"
#include <stdbool.h>
#include <string.h>


ei_bool_t toplevel_callback_move_move(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param);

ei_bool_t toplevel_callback_click(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param);


ei_bool_t toplevel_callback_release(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param);

// Peut-être pas dans le bon fichier
// Enfonce les boutons
ei_bool_t button_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

// Quand on relache la souris sur le bouton
ei_bool_t button_callback_release(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

ei_bool_t all_callback_release(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param);
#endif
