#include "ei_widget.h"
#include "ei_widgettypes.h"
#include "ei_utils.h"
#include "ei_types.h"
#include <stdbool.h>
#include <string.h>
#ifndef     EI_CALLBACK_H
#define     EI_CALLBACK_H

/* Fonction de redimensionnement
 * Conserve les proportions des fils */
void resize(ei_widget_t *widget, ei_size_t add_size);

/* Fonction de mouvement
 * Deplacement brut */
void move(ei_widget_t *widget, ei_size_t dist);


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
