/**
 *  @file       ei_application.c
 *
 *  \author 
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 02.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#include "ei_application.h"
#include "ei_common.h"
#include "ei_event_pv.h"
#include "ei_widgetclass_pv.h"
#include "ei_widgettypes.h"
#include "ei_root.h"
#include "ei_callback.h"
#include "ei_utils.h"
#include "ei_gridder.h"
#include "ei_geometrymanager_pv.h"
#include "ei_tag.h"


static ei_bool_t quit_request = EI_FALSE;

/* Cree l'application */
void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen)
{
        ei_init();
        hw_init();

        ei_set_root_surface(hw_create_window(main_window_size, fullscreen));

        // Enregistrement des classes
        ei_frame_register_class();
        ei_button_register_class();
        ei_toplevel_register_class();
        ei_radiobutton_register_class();

        // Initialisation du root_widget
        ei_set_root(ei_widget_create ("frame", NULL));

        ei_register_placer_manager();
        ei_register_gridder_manager();

        ei_widget_t *root_widget = ei_get_root();
        ei_place(root_widget, NULL, NULL, NULL, &main_window_size->width, &main_window_size->height
                        , NULL, NULL, NULL, NULL );

        ei_set_picking_surface(hw_surface_create(ei_get_root_surface(), main_window_size, EI_TRUE));

        // Pour gérer le clic sur les boutons ils faut faire un bind sur le tag
        // "button" dans cette fonction avec les callback 1 et 2 définies dans
        // ei_widget_class
        ei_bind(ei_ev_mouse_buttondown, NULL, "button", button_callback_click, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", toplevel_callback_click, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "radiobutton", radiobutton_callback_click, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", all_callback_release, NULL);
}

/* Libere les ressources graphiques allouées à l'application */
void ei_app_free()
{
        ei_surface_t picking = ei_get_picking_surface();

        if (picking)
                hw_surface_free(picking);

        ei_unbind_all();

        ei_widget_destroy(ei_get_root());

        ei_widgetclass_free();

        ei_geometrymanager_free();

        ei_invalidate_reset();

        ei_tag_free();

        hw_quit();

#ifdef LEAK_TRACKER
        printf("\nLeaks : %d\n", ALLOCATION_COUNTER);
#endif
}

/* Boucle infinie, coeur de l'application */
void ei_app_run()
{
        ei_event_t event;
        ei_surface_t root_surface = ei_get_root_surface();
        ei_linked_rect_t *lrect = NULL;

        // On appelle la runfunc du root pour lancer le programme
        ei_widget_t *root = ei_get_root();
        root->geom_params->manager->runfunc(root);

        while (!quit_request) {

                /* Draw rectangles */
                ei_draw_rects();

                /* Update newly drawn rectangles */
                lrect = ei_get_update_rects();

                if (lrect)
                        // Mise a jour a l'ecran
                        hw_surface_update_rects(root_surface, lrect);

                /* Clear draw rectangle */
                ei_invalidate_reset();

                /* Wait for an event */
                hw_event_wait_next(&event);

                /* Process each event */
                ei_event_process(&event);
        };
}

/* Permet à l'utilisateur d'invalider manuellement une partie de l'écran pour la
 * mettre à jour */
void ei_app_invalidate_rect(ei_rect_t* rect)
{
        ei_invalidate_rect(rect);
}

/* Demande la fin de l'exécution */
void ei_app_quit_request()
{
        quit_request = EI_TRUE;
}


/* Renvoie l'adresse du root_widget */
ei_widget_t* ei_app_root_widget(){

        return ei_get_root();
}

/* Renvoie la surface de dessin */
ei_surface_t ei_app_root_surface(){

        return ei_get_root_surface();
}

