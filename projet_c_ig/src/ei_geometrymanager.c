/**
 *  @file       ei_geometrymanager.h
 *  @brief      Manages the positionning and sizing of widgets on the screen.
 *
 *  \author
 *  Created by François Bérard on 18.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#include "ei_geometrymanager_pv.h"
#include "ei_common.h"
#include "ei_core.h"
#include "ei_utils.h"
#include "ei_linkedlist.h"
// Pour le type placer_param
#include "ei_params.h"
#include <stdio.h>
static ei_linkedlist_t ei_geometrymanagers = { NULL, NULL };

/**
 * \brief       Registers a geometry manager to the program so that it can be called to manager
 *              widgets. This must be done only once in the application.
 *
 * @param       geometrymanager         The structure describing the geometry manager.
 */
void ei_geometrymanager_register(ei_geometrymanager_t* geometrymanager)
{
        ei_linkedlist_add(&ei_geometrymanagers, geometrymanager);
}



/**
 * \brief       Returns a geometry manager structure from its name.
 *
 * @param       name            The name of the geometry manager.
 *
 * @return                      The structure describing the geometry manager.
 */
ei_geometrymanager_t*   ei_geometrymanager_from_name    (ei_geometrymanager_name_t name)
{
        ei_geometrymanager_t *current = NULL;
        ei_geometrymanager_t *geometrymanager = NULL;
        ei_bool_t found = EI_FALSE;
        ei_linked_elem_t *link = ei_geometrymanagers.head;

        while (link && !found) {
                current = (ei_geometrymanager_t*)link->elem;

                if (current && !strncmp(current->name, name, sizeof(ei_geometrymanager_name_t)))
                        found = EI_TRUE;

                link = link->next;
        }

        if (found)
                geometrymanager = current;

        return geometrymanager;
}



/**
 * \brief       Tell the geometry manager in charge of a widget to forget it. This removes the
 *              widget from the screen. If the widget is not currently managed, this function
 *              returns silently.
 *              Side effects:
 *              <ul>
 *                      <li> the \ref ei_geometrymanager_releasefunc_t of the geometry manager in
 *                              charge of this widget is called, </li>
 *                      <li> the geom_param field of the widget is freed, </li>
 *                      <li> the current screen_location of the widget is invalided (which will
 *                              trigger a redraw), </li>
 *                      <li> the screen_location of the widget is reset to 0. </li>
 *              </ul>
 *
 * @param       widget          The widget to unmap from the screen.
 */
void ei_geometrymanager_unmap(ei_widget_t* widget)
{

}


/***** Placer *****/

/*  Gere le clipping */
void ei_place_runfunc(struct ei_widget_t*       widget)
{

        /* On commence par invalider l'ancien rectangle */
        ei_invalidate_rect(&widget->screen_location);

        /* On calcule la nouvelle screen_location */

        // Placement
        ei_anchor_t anc;
        int xmin;
        int xmax;
        int ymin;
        int ymax;

        // Point d'ancrage
        int x_anc;
        int y_anc;
        ei_rect_t parent_rect;
        ei_rect_t screen_location;

        // Récupération des paramètres pour l'ancrage
        ei_placer_param_t *param =
                (ei_placer_param_t*)widget->geom_params;

        // Definition du rectangle contenant le widget
        if (widget->parent){
                // Theoriquement le second cas n'arrive jamais
                if (widget->parent->content_rect) 
                        parent_rect = *(widget->parent->content_rect);
                else
                        parent_rect = hw_surface_get_rect(ei_get_root_surface());

                // Rectangle englobant
                xmin = parent_rect.top_left.x;
                ymin = parent_rect.top_left.y;
                xmax = xmin + parent_rect.size.width - 1;
                ymax = ymin + parent_rect.size.height - 1;



                // On continue seulement si le point donné en argument
                // est dans le content_rect

                // Assignation x_anx, y_anc
                if (param->x)
                        x_anc = *param->x + xmin;
                else if(param->rel_x)
                        x_anc = xmin + (int)floor(*param->rel_x * ((float)xmax - (float)xmin));
                else
                        x_anc = 0;
                if(param->y)
                        y_anc = *param->y + ymin;
                else if (param->rel_y)
                        y_anc = ymin + (int)floor(*param->rel_y * ((float)ymax - (float)ymin));
                else
                        y_anc = 0;

                // On calcule la taille du widget

                // Taille par defaut
                // Priorité : taille fournie en argument > requested >
                // defaut
                screen_location.size = widget->requested_size;
                int w;
                int h;
                w= widget->requested_size.width;
                h = widget->requested_size.height;

                if (param->w) {
                        w = *param->w;
                }
                else if(param->rel_w){
                        int rw = parent_rect.size.width;
                        rw = (int)floor((float)rw * *param->rel_w);
                        w = rw;
                }

                if(param->h){
                        h = *param->h;
                }
                else if(param->rel_h ){
                        int rh = parent_rect.size.height;
                        rh = (int)floor((float)rh * *param->rel_h);
                        h = rh;
                }
                // Calcul de la position du point d'ancrage
                // Vérification de la longueur
                // Assignation des champs du widget
                // x1, y1 = top_left du widget
                // x2, y2 = bottom_right
                int x1;
                int x2;
                int y1;
                int y2;

                // Definition de l'ancrage
                anc = ei_anc_northwest;
                if (param->anc && *param->anc)
                        anc = *param->anc;

                switch (anc) {
                        //      ei_anc_none             = 0,    ///< No anchor defined.
                case ei_anc_center : 
                        // Le + 1 est important (/2 tronque)
                        // exemple
                        // XXXX avec w = 4, C le centre
                        // 0C00
                        // 0000
                        x1 = x_anc - (w / 2) + 1;
                        x2 = x_anc + (w / 2);
                        y1 = y_anc - (h / 2) + 1;
                        y2 = y_anc + (h / 2);
                        break;                  ///< Anchor in the center.
                case ei_anc_north : 
                        x1 = x_anc - (w / 2) - 1;
                        x2 = x_anc + (w / 2);
                        y1 = y_anc;
                        y2 = y_anc + h -1 ;
                        break;  ///< Anchor on the top side, centered horizontally.
                case ei_anc_northeast :
                        x1 = x_anc - w + 1;
                        x2 = x_anc;
                        y1 = y_anc;
                        y2 = y_anc + h - 1;
                        break;  ///< Anchor on the top-right corner.
                case ei_anc_east :
                        x1 = x_anc - w +1;
                        x2 = x_anc;
                        y1 = y_anc - (h / 2) +1;
                        y2 = y_anc + (h / 2);
                        break;          ///< Anchor on the right side, centered vertically.
                case ei_anc_southeast:
                        x1 = x_anc - w + 1;
                        x2 = x_anc;
                        y1 = y_anc - h + 1;
                        y2 = y_anc;
                        break;  ///< Anchor on the bottom-right corner.
                case ei_anc_south:
                        x1 = x_anc - ( w / 2)  + 1;
                        x2 = x_anc + ( w / 2);
                        y1 = y_anc - h + 1;
                        y2 = y_anc;
                        break;          ///< Anchor on the bottom side, centered horizontally.
                case ei_anc_southwest:  
                        x1 = x_anc;
                        x2 = x_anc + w -1;
                        y1 = y_anc -h +1;
                        y2 = y_anc;
                        break;  ///< Anchor on the bottom-left corner.
                case ei_anc_west:
                        x1 = x_anc;
                        x2 = x_anc + w -1;
                        y1 = y_anc - (h / 2) + 1;
                        y2 = y_anc + (h / 2);
                        break;          ///< Anchor on the left side, centered vertically.
                case ei_anc_northwest: 
                        x1 = x_anc;
                        x2 = x_anc + w -1;
                        y1 = y_anc;
                        y2 = y_anc + h - 1;
                        break;
                default : exit(-1);
                          break;
                }
                w = x2 - x1 + 1;
                h = y2 - y1 + 1;
                // on assigne enfin les valeurs calculées
                screen_location.top_left.x
                        = x1;
                screen_location.top_left.y
                        = y1;
                screen_location.size.width 
                        = w;
                screen_location.size.height
                        = h;

        }
        // Root
        else{
                screen_location.top_left = ei_point(0,0);
                screen_location.size = ei_size(*param->w, *param->h);

        }

        // Appel a geomnotify
        widget->wclass->geomnotifyfunc(widget, screen_location);

        /* On invalide le nouveau rectangle*/
        ei_rect_t new_pos = widget->screen_location;
        ei_invalidate_rect(&new_pos);

        /* Appels récursifs sur les enfants */
        // Appel récursif sur les enfants pour les replacer
        ei_widget_t *current = widget->children_head;
        while(current && current->geom_params && current->geom_params &&
                        current->geom_params->manager &&
                        current->geom_params->manager->runfunc){
                current->geom_params->manager->runfunc(current);
                current = current->next_sibling;
        }

}

void ei_place_releasefunc(struct ei_widget_t* widget)
{
        ei_placer_param_t *param = (ei_placer_param_t*)widget->geom_params;

        if (param) {
                SAFE_FREE(param->anc);
                SAFE_FREE(param->x);
                SAFE_FREE(param->y);
                SAFE_FREE(param->rel_x);
                SAFE_FREE(param->rel_y);
                SAFE_FREE(param->w);
                SAFE_FREE(param->h);
                SAFE_FREE(param->rel_w);
                SAFE_FREE(param->rel_h);

                SAFE_FREE(param);
        }
}

/**
 * \brief       Registers the "placer" geometry manager in the program. This must be called only
 *              once before the \ref ei_place function can be called.
 */
void  ei_register_placer_manager()
{
        ei_geometrymanager_t *placer = CALLOC_TYPE(ei_geometrymanager_t);
        assert(placer);

        strcpy(placer->name, "placer");

        placer->runfunc = ei_place_runfunc;
        placer->releasefunc = ei_place_releasefunc;
        placer->next = NULL;

        ei_geometrymanager_register(placer);
}


/**
 * \brief       Configures the geometry of a widget using the "placer" geometry manager.
 *              If the widget was already managed by another geometry manager, then it is first
 *              removed from the previous geometry manager.
 *              If the widget was already managed by the "placer", then this calls simply updates
 *              the placer parameters: arguments that are not NULL replace previous values.
 *              When the arguments are passed as NULL, the placer uses default values (detailed in
 *              the argument descriptions below). If no size is provided (either absolute or
 *              relative), then the requested size of the widget is used, i.e. the minimal size
 *              required to display its content.
 *
 * @param       widget          The widget to place.
 * @param       anchor          How to anchor the widget to the position defined by the placer
 *                              (defaults to ei_anc_northwest).
 * @param       x               The absolute x position of the widget (defaults to 0).
 * @param       y               The absolute y position of the widget (defaults to 0).
 * @param       width           The absolute width for the widget (defaults to the requested width
 *                              of the widget).
 * @param       height          The absolute height for the widget (defaults to the requested height
 *                              of the widget).
 * @param       rel_x           The relative x position of the widget: 0.0 corresponds to the left
 *                              side of the master, 1.0 to the right side (defaults to 0.0).
 * @param       rel_y           The relative y position of the widget: 0.0 corresponds to the top
 *                              side of the master, 1.0 to the bottom side (defaults to 0.0).
 * @param       rel_width       The relative width of the widget: 0.0 corresponds to a width of 0,
 *                              1.0 to the width of the master (defaults to 0.0).
 * @param       rel_height      The relative height of the widget: 0.0 corresponds to a height of 0,
 *                              1.0 to the height of the master (defaults to 0.0).
 */
void ei_place(ei_widget_t *widget,
                ei_anchor_t     *anchor,
                int *x,
                int *y,
                int *width,
                int *height,
                float *rel_x,
                float *rel_y,
                float *rel_width,
                float *rel_height)
{
        ei_geometrymanager_t *placer = ei_geometrymanager_from_name("placer");
        assert(placer);

        ei_bool_t gp_alloc = EI_FALSE;

        if (placer && widget) {
                /* Display widget */


                // On verifie que le widget est bien géré par le placeur,
                // sinon on le modifie pour qu'il le soit
                gp_alloc = EI_TRUE;
                if (widget->geom_params && widget->geom_params->manager) {
                        if (widget->geom_params->manager != placer) {
                                widget->geom_params->manager->releasefunc(widget);
                        }
                        else
                                gp_alloc = EI_FALSE;
                }

                if (gp_alloc) {
                        ei_placer_param_t *param = CALLOC_TYPE(ei_placer_param_t);

                        if (param != NULL) {
                                widget->geom_params = (ei_geometry_param_t*)param;
                                widget->geom_params->manager = placer;

                                param->anc = CALLOC_TYPE(ei_anchor_t);
                                param->x = CALLOC_TYPE(int);
                                param->y = CALLOC_TYPE(int);
                                param->rel_x = CALLOC_TYPE(float);
                                param->rel_y = CALLOC_TYPE(float);
                                param->w = CALLOC_TYPE(int);
                                param->h = CALLOC_TYPE(int);
                                param->rel_w = CALLOC_TYPE(float);
                                param->rel_h = CALLOC_TYPE(float);
                        }
                }

                // On verifie qu'on a pas un rel_w, rel_h absurdes
                if (!width && rel_width){
                        if (*rel_width < 0.)
                                exit(-1);
                }

                if (!height && rel_height){
                        if (*rel_height <  0.)
                                exit(-1);
                }

                // Sauvegarde des paramètres
                ei_placer_param_t *param = (ei_placer_param_t*)widget->geom_params;
                assert(param);

                if (anchor){
                        if(!param->anc)
                                param->anc = CALLOC_TYPE(ei_anchor_t);  
                        *param->anc = *anchor;}
                else{
                        SAFE_FREE(param->anc)
                }
                if (x){
                        if (!param->x)
                                param->x = CALLOC_TYPE(int);
                        *param->x = *x;
                }
                else if (rel_x){
                        SAFE_FREE(param->x);
                        if (!param->rel_x)
                                param->rel_x = CALLOC_TYPE(float);
                        *param->rel_x = *rel_x;
                }
                else{
                        SAFE_FREE(param->rel_x);
                        if (!param->x)
                                param->x = CALLOC_TYPE(int);
                        *param->x = 0;
                }

                if (y){
                        if(!param->y)
                                param->y = CALLOC_TYPE(int);
                        *param->y = *y;
                }
                else if (rel_y){
                        SAFE_FREE(param->y);
                        if(!param->y)
                                param->rel_y = CALLOC_TYPE(float);
                        *param->rel_y = *rel_y;
                }
                else{
                        SAFE_FREE(param->rel_y);
                        if(!param->y)
                                param->y = CALLOC_TYPE(int);
                        *param->y = 0;
                }

                if (width){
                        if(!param->w)
                                param->w = CALLOC_TYPE(int);
                        *param->w = *width;
                }
                else if (rel_width){
                        SAFE_FREE(param->w);
                        if(!param->w)
                                param->w = CALLOC_TYPE(int);
                        *param->rel_w = *rel_width;
                }
                else{
                        SAFE_FREE(param->rel_w);
                        if(!param->w)
                                param->w = CALLOC_TYPE(int);
                        *param->w = widget->requested_size.width;
                }

                if (height){
                        *param->h = *height;
                }
                else if (rel_height){
                        SAFE_FREE(param->h);
                        if (!param->rel_h)
                                param->rel_h = CALLOC_TYPE(float);
                        *param->rel_h = *rel_height;
                }
                else{
                        SAFE_FREE(param->rel_h);
                        if (!param->h)
                                param->h = CALLOC_TYPE(int);
                        *param->h = widget->requested_size.height;
                }

                widget->geom_params->manager->runfunc(widget);
        }
}

void ei_geometrymanager_free()
{
        ei_linkedlist_empty(&ei_geometrymanagers, EI_TRUE);
}


