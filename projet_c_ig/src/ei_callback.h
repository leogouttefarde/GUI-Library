/**
 *  @file	ei_callback.h
 *
 *  @brief 	Defines the callbacks needed to resize and move toplevels, to click buttons and handle mouse release.
 * 
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 13.06.14.
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


ei_bool_t all_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

/**
 * @brief       callback to move a toplevel when the mouse moves    
 *
 * @param   widget      null pointer since the bind is on the tag "all"
 * @param   event       mouse movement
 * @param   user_param  can be recasted to the toplevel to move
 *
 * @return  the callback bool
 */
ei_bool_t all_callback_move_move(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param);

/**
 * @brief       callback to resize a toplevel when the mouse moves    
 *
 * @param   widget      null pointer since the bind is on the tag "all"
 * @param   event       mouse movement
 * @param   user_param  can be recasted to the toplevel to move
 *
 * @return  the callback bool
 */
ei_bool_t all_callback_move_resize(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param);

/**
 * @brief       callback to handle clicking on a toplevel. dectects the position
 *              of the click : title_bar, close, resize or nothing    
 *
 * @param   widget      clicked toplevel
 * @param   event       mouse click
 * @param   user_param  can be recasted to the toplevel to move
 *
 * @return  the callback bool
 */
ei_bool_t toplevel_callback_click(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param);


ei_bool_t radiobutton_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

/**
 * @brief       Callback to handle mouse release, either for buttons or
 *              toplevels   
 *
 * @param   widget      NULL pointer since the bind is on the tag "all"
 * @param   event       Mouse release
 * @param   user_param  NULL, the pressed widget is in a global
 *
 * @return  The callback bool
 */
ei_bool_t all_callback_release(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param);


/**
 * @brief       Callback to click buttons   
 *
 * @param   widget      Clicked  button
 * @param   event       Mouse release
 * @param   user_param  NULL
 *
 * @return  The callback bool
 */
ei_bool_t button_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

ei_bool_t entry_callback_click(ei_widget_t *widget, struct ei_event_t *event,void *user_param);

ei_bool_t entry_callback_keyboard(ei_widget_t *widget, struct ei_event_t *event,
                void *user_param);

#endif
