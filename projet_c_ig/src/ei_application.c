/**
 *  @file       ei_application.h
 *  @brief      Manages the main steps of a graphical application: initialization, main window,
 *              main loop, quitting, resource freeing.
 *
 *  \author 
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_common.h"
#include "ei_event.h"
#include "ei_widgettypes.h"
#include "ei_core.h"

#include "ei_utils.h"
static ei_bool_t quit_request = EI_FALSE;

/* Fonction de redimensionnement
 * Conserve les proportions des fils */
void resize(ei_widget_t *widget, ei_size_t add_size){

        //ei_anchor_t anc = ei_anc_northwest;

        // Taille parent , ancienne taille widget
        int p_w = widget->parent->content_rect->size.width;
        int p_h = widget->parent->content_rect->size.height;
        int w = widget->screen_location.size.width;
        int h = widget->screen_location.size.height;

        // Nouvelles tailles
        int a_w;
        int a_h;
        float rel_w;
        float rel_h;

        int *a_w_p = &a_w;
        int *a_h_p = &a_h;
        float *rel_w_p = &rel_w;
        float *rel_h_p = &rel_h;

        // Taille du widget
        // Calcul de la taille absolue du widget
        a_w = w + add_size.width;
        a_h = h + add_size.height;
        rel_w = (float)a_w / (float)p_w;
        rel_h = (float)a_h / (float)p_h;

        // On recupere les parametres
        ei_placer_param_t *param;
        param = (ei_placer_param_t*)widget->geom_params;

        // On distingue le cas ou le widget est absolu et le cas
        // relatif
        if (param->w)
                rel_w_p = NULL;
        else
                a_w_p = NULL;

        if (param->h)
                rel_h_p = NULL;
        else
                a_h_p = NULL;
        // Placement du widget pere
        // La taille devient automatiquement absolue
        ei_place(widget, param->anc, param->x, param->y, a_w_p, a_h_p, param->rel_x, param->rel_y, rel_w_p,
                        rel_h_p);

}

/* Fonction de mouvement
 * Deplacement brut */
void move(ei_widget_t *widget, ei_size_t dist){


        ei_anchor_t anc = ei_anc_northwest;

        int x;
        int y;

        // Position top_left et taille du pere
        int p_x = widget->screen_location.top_left.x;
        int p_y = widget->screen_location.top_left.y;

        // Nouveau x absolu
        x = p_x + dist.width;
        y = p_y + dist.height;

        // On deplace le pere
        ei_place(widget, &anc, &x, &y, NULL, NULL, NULL,
                        NULL, NULL, NULL);

/* Commenté pr compiler
ei_bool_t toplevel_callback_click(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param){
        ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
        toplevel->prec = event->param.mouse.where;*/
        // On cherche si on a cliquer sur le titre, le resize, le close
        // TODO
        // SI TITRE   
        // On bind ce widget, mouse_move et move
        // TODO TODO TODO TOU
        //  SI RESIZE
        //  On bind mouse_move, resize, le widget
        //  SI CLOSE
        //  ???? plus tard

        return EI_FALSE;
}

ei_bool_t toplevel_callback_move_move(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param){
        ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
/* Commenté pr compiler
        int w = event->param.mouse.where.x - toplevel->prec.x + 1;
        int h = event->param.mouse.where.y - toplevel->prec.y + 1;
        move(widget, ei_size(w,h));*/
        return EI_FALSE;
}

// Peut-être pas dans le bon fichier
// Enfonce les boutons
ei_bool_t button_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        if (widget){
                if (!strcmp(widget->wclass->name, "button")) {

                        ei_button_t *button = (ei_button_t*)widget;
                        button->relief = ei_relief_sunken;
                }
        }
        return EI_FALSE;
}

// Quand on relache la souris sur le bouton
ei_bool_t button_callback_release(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        ei_bool_t res;

        if (widget){
                if (!strcmp(widget->wclass->name, "button")) {

                        ei_button_t *button = (ei_button_t*)widget;
                        button->relief = ei_relief_raised;

                        // Appel du callback du bouton
                        if (button->callback){
                                res = button->callback((ei_widget_t*)button, NULL, button->user_param);
                        }
                }
        }
        return EI_FALSE;
}

/**
 * \brief       Creates an application.
 *              <ul>
 *                      <li> initializes the hardware (calls \ref hw_init), </li>
 *                      <li> registers all classes of widget and all geometry managers, </li>
 *                      <li> creates the root window (either in a system window, or the entire
 *                              screen), </li>
 *                      <li> creates the root widget to accress the root window. </li>
 *              </ul>
 *
 * @param       main_window_size        If "fullscreen is false, the size of the root window of the
 *                                      application.
 *                                      If "fullscreen" is true, the current monitor resolution is
 *                                      used as the size of the root window, this size is returned
 *                                      in this parameter.
 * @param       fullScreen              If true, the root window is the entire screen. Otherwise, it
 *                                      is a system window.
 */
void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen)
{
        ei_init();
        hw_init();

        ei_set_root_surface(hw_create_window(main_window_size, fullscreen));

        // Enregistrement des classes
        ei_frame_register_class();
        ei_button_register_class();
        ei_toplevel_register_class();

        // Initialisation du root_widget
        ei_set_root(ei_widget_create ("frame", NULL));

        ei_register_placer_manager();

        ei_widget_t *root_widget = ei_get_root();
        ei_place(root_widget, NULL, NULL, NULL, &main_window_size->width, &main_window_size->height
                        , NULL, NULL, NULL, NULL );

        ei_set_picking_surface(hw_surface_create(ei_get_root_surface(), main_window_size, EI_TRUE));

        // Pour gérer le clic sur les boutons ils faut faire un bind sur le tag
        // "button" dans cette fonction avec les callback 1 et 2 définies dans
        // ei_widget_class
        ei_bind(ei_ev_mouse_buttondown, NULL, "button", button_callback_click, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "button", button_callback_release, NULL);
}

/**
 * \brief       Releases all the resources of the application, and releases the hardware
 *              (ie. calls \ref hw_quit).
 */
void ei_app_free()
{
        ei_surface_t picking = ei_get_picking_surface();

        if (picking)
                hw_surface_free(picking);
}

/**
 * \brief       Runs the application: enters the main event loop. Exits when
 *              \ref ei_app_quit_request is called.
 */
void ei_app_run()
{
        ei_event_t event;
        ei_surface_t root_surface = ei_get_root_surface();

        while (!quit_request) {

                ei_draw_widget(ei_get_root()); // DEBUG : DRAW ALL
                //ei_draw_widgets();

                ei_invalidate_rects();

                // Update des surfaces
                hw_surface_update_rects(root_surface, NULL); // DEBUG : UPDATE ALL
                //hw_surface_update_rects(root_surface, ei_get_update_rects());

                ei_invalidate_reset();


                hw_event_wait_next(&event);

                ei_event_process(&event);
        };
}

/**
 * \brief       Adds a rectangle to the list of rectangles that must be updated on screen. The real
 *              update on the screen will be done at the right moment in the main loop.
 *
 * @param       rect            The rectangle to add, expressed in the root window coordinates.
 *                              A copy is made, so it is safe to release the rectangle on return.
 */
void ei_app_invalidate_rect(ei_rect_t* rect)
{
        ei_invalidate_rect(rect);
}

/**
 * \brief       Tells the application to quite. Is usually called by an event handler (for example
 *              when pressing the "Escape" key).
 */
void ei_app_quit_request()
{
        quit_request = EI_TRUE;
}


/**
 * \brief       Returns the "root widget" of the application: a "frame" widget that encapsulate the
 *              root window.
 *
 * @return                      The root widget.
 */
ei_widget_t* ei_app_root_widget(){

        return ei_get_root();
}

/**
 * \brief       Returns the surface of the root window. Used to create surfaces with similar r, g, b
 *              channels.
 *
 * @return                      The surface of the root window.
 */
ei_surface_t ei_app_root_surface(){

        return ei_get_root_surface();
}

