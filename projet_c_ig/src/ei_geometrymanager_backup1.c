/**
 *  @file       ei_geometrymanager.h
 *  @brief      Manages the positionning and sizing of widgets on the screen.
 *
 *  \author
 *  Created by François Bérard on 18.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#include <string.h>
#include "ei_geometrymanager.h"
#include "ei_common.h"
#include "ei_core.h"
#include "ei_utils.h"
#include "ei_widgettypes.h"
#include "ei_application.h"

// TODO : Gérer les listes avec la classe linkedlist
static ei_geometrymanager_t *first = NULL;

/**
 * \brief       Registers a geometry manager to the program so that it can be called to manager
 *              widgets. This must be done only once in the application.
 *
 * @param       geometrymanager         The structure describing the geometry manager.
 */
void ei_geometrymanager_register(ei_geometrymanager_t* geometrymanager)
{
        if (geometrymanager) {
                ei_geometrymanager_t *current, *next;

                if (first) {
                        current = first;
                        next = current;

                        do {
                                current = next;
                                next = current->next;
                        } while (next);

                        current->next = geometrymanager;
                }
                else
                        first = geometrymanager;

                /* Make sure it is the last geometry manager */
                geometrymanager->next = NULL;
        }
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
        ei_geometrymanager_t *current, *next;
        ei_geometrymanager_t *geometrymanager = NULL;
        ei_bool_t fail = EI_TRUE;

        current = first;
        next = current;
        while (current && (fail = strncmp(current->name, name, sizeof(ei_geometrymanager_name_t))) && next) {
                current = next;
                next = current->next;
        }

        if (!fail)
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

// ei_rect_t* rect_intersection(ei_rect_t *rect1, ei_rect_t *rect2)
// {
//         ei_rect_t *inter = NULL;

//         if (rect1 && rect2) {
//                 int x1 = rect1->top_left.x;
//                 int y1 = rect1->top_left.y;

//                 int w1 = rect1->size.width;
//                 int h1 = rect1->size.height;


//                 int x2 = rect2->top_left.x;
//                 int y2 = rect2->top_left.y;

//                 int w2 = rect2->size.width;
//                 int h2 = rect2->size.height;


//                 if ()


//                 inter = CALLOC_TYPE(ei_rect_t);

//                 inter.top_left.x = ;
//                 inter.top_left.y = ;

//                 inter.size.width = ;
//                 inter.size.height = ;
//         }

//         return inter;
// }


ei_rect_t* rect_intersection(const ei_rect_t *rect1, const ei_rect_t *rect2) 
{
        ei_rect_t *inter = NULL;
        // printf("calc inter entre\n\n");
        // print_rect(rect1);
        // printf("\net\n\n");
        // print_rect(rect2);

        if (rect1 && rect2) {
                int x1 = rect1->top_left.x;
                int y1 = rect1->top_left.y;

                int w1 = rect1->size.width;
                int h1 = rect1->size.height;


                int x2 = rect2->top_left.x;
                int y2 = rect2->top_left.y;

                int w2 = rect2->size.width;
                int h2 = rect2->size.height;



                int r2_left = x2;
                int r2_top = y2;
                int r2_right = x2 + w2;
                int r2_bottom = y2 + h2;

                int r1_left = x1;
                int r1_top = y1;
                int r1_right = x1 + w1;
                int r1_bottom = y1 + h1;


                bool is_intersection =  ( r2_left < r1_right 
                                        && r2_right > r1_left 
                                        && r2_top < r1_bottom 
                                        && r2_bottom > r1_top 
                                        );

                if(is_intersection)
                {
                        //printf("r1 coupe r2 !!!!!!!!!!!!!!!\n");
                        inter = CALLOC_TYPE(ei_rect_t);
                        assert(inter);

                        if (inter) {
                                int left = MAX(r1_left, r2_left);
                                int top = MAX(r1_top, r2_top);

                                inter->top_left.x = MAX(r1_left, r2_left);
                                inter->top_left.y = MAX(r1_top, r2_top);

                                inter->size.width = MIN( r1_right, r2_right) - left;
                                inter->size.height = MIN(r1_bottom, r2_bottom) - top;
                        }
                }
                else
                       printf("NO INTER\n");
        }
        // printf("\n\nRésultat\n\n");
        // print_rect(inter);

        return inter;
}

void print_rect(const ei_rect_t *rect)
{
        if (rect) {
                int left = rect->top_left.x;
                int top = rect->top_left.y;

                int right = rect->size.width + left;
                int bottom = rect->size.height + top;

                int w = rect->size.width;
                int h = rect->size.height;

                // printf("rect %08lX\n\n", rect);

                // printf("left %d\t", left);
                // printf("right %d\n", right);

                // printf("top %d\t", top);
                // printf("bottom %d\n", bottom);

                // printf("width %d\t", w);
                // printf("height %d\n\n\n", h);

                printf("left %d\t", left);
                printf("right %d\t", right);

                printf("top %d\t", top);
                printf("bottom %d\t", bottom);

                printf("width %d\t", w);
                printf("height %d\n\n", h);
        }
}

/*  Gere le clipping */
void ei_place_runfunc(struct ei_widget_t*       widget)
{
        //printf("place run %x", widget);

        if (widget && widget->wclass) {
                printf(" %s  ", widget->wclass->name);
        }

        //
        ei_rect_t *clipper;
        if (widget->parent){
                clipper = widget->parent->content_rect;  
        }
        else{
                clipper = &widget->screen_location;
        }

        ei_widget_t *root = ei_get_root();

        if (root) {
                ei_rect_t *real_clipper;
                real_clipper = rect_intersection(clipper, root->content_rect);
        print_rect(real_clipper);
                if (real_clipper) {
                        //printf ("  DRAW");
                        //widget->wclass->drawfunc(widget, ei_get_root_surface(), ei_get_picking_surface(), clipper);
                        widget->wclass->drawfunc(widget, ei_get_root_surface(), ei_get_picking_surface(), real_clipper);

                        SAFE_FREE(real_clipper);
                }
                else
                        printf ("INTER???\n");
        }
        else
                printf ("ROOT???\n");

        printf("\n");
}

void ei_place_releasefunc(struct ei_widget_t*   widget)
{
        SAFE_FREE(widget->geom_params);
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

/*
void ei_place_rec(ei_widget_t *widget, bool place_cur) {

        if (widget) {
                if (place_cur) {
                        // Placement du widget
                        ei_placer_param_t *param;
                        param = (ei_placer_param_t*)widget->geom_params;
                        if (param) {
                                ei_place(widget, param->anc, param->x, param->y, param->w, param->h,
                                                param->rel_x, param->rel_y, param->rel_w, param->rel_h);
                        }
                }

                ei_place_rec(widget->children_head, true);
                ei_place_rec(widget->next_sibling, true);
        }
}*/

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


        if (placer && widget) {
                /* Display widget */

                // If there is a parent invalidate parent instead
                // ei_widget_t *inval = widget;
                // if (inval && inval->parent)
                //         inval = inval->parent;

                // ei_invalidate_widget(inval);

                ei_rect_t old_pos = widget->screen_location;
                ei_app_invalidate_rect(&old_pos);


                ei_bool_t gp_alloc = EI_FALSE;

                // On verifie que le widget est bien géré par le placeur,
                // sinon on le modifie pour qu'il le soit
                if (widget) {
                        gp_alloc = EI_TRUE;
                        if (widget->geom_params) {
                                if (widget->geom_params->manager) {
                                        if (widget->geom_params->manager != placer) {
                                                widget->geom_params->manager->releasefunc(widget);
                                        }
                                }
                                else
                                        gp_alloc = EI_FALSE;
                        }
                }

                if (gp_alloc) {
                        widget->geom_params = CALLOC_TYPE(ei_placer_param_t);
                        widget->geom_params->manager = placer;
                }

                // Sauvegarde des paramètres
                ei_placer_param_t *param = (ei_placer_param_t*)widget->geom_params;
                assert(param);

                param->x = CALLOC_TYPE(int);
                param->y = CALLOC_TYPE(int);
                param->rel_x = CALLOC_TYPE(float);
                param->rel_y = CALLOC_TYPE(float);
                param->w = CALLOC_TYPE(int);
                param->h = CALLOC_TYPE(int);
                param->rel_w = CALLOC_TYPE(float);
                param->rel_h = CALLOC_TYPE(float);
                param->anc = CALLOC_TYPE(ei_anchor_t);

                if (anchor)
                        *param->anc = *anchor;
                else
                        param->anc = NULL;
                if (x)
                        *param->x = *x;
                else
                        param->x = NULL;
                if (y)
                        *param->y = *y;
                else
                        param->y = NULL;
                if (rel_x)
                        *param->rel_x = *rel_x;
                else
                        param->rel_x = NULL;
                if (rel_y)
                        *param->rel_y = *rel_y;
                else
                        param->rel_y = NULL;
                if(width)
                        *param->w = *width;
                else
                        param->w = NULL;
                if(height)
                        *param->h = *height;
                else
                        param->h = NULL;
                if (rel_width)
                        *param->rel_w = *rel_width;
                else
                        param->rel_w = NULL;
                if (rel_height)
                        *param->rel_h = *rel_height;
                else
                        param->rel_h = NULL;


                // Placement
                ei_anchor_t anc;
                bool keep = true;
                int xmin;
                int xmax;
                int ymin;
                int ymax;

                int x_anc;
                int y_anc;
                ei_rect_t parent_rect;
                ei_rect_t screen_location;

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

                        // Definition de l'ancrage
                        anc = ei_anc_northwest;
                        if (anchor && *anchor)
                                anc = *anchor;

                        // Verification de la validité du point x, y
                        if(x && keep){
                                keep = *x + xmin <= xmax;
                        }
                        else if (rel_x && keep){
                                keep = *rel_x >= 0. && *rel_x <= 1.;
                        }

                        if(y && keep){
                                keep = *y + ymin <= ymax;
                        }
                        else if (rel_x && keep){
                                keep = *rel_x >= 0. && *rel_x <= 1.;
                        }



                        // On continue seulement si le point donné en argument
                        // est dans le content_rect
                        if (keep){
                                // Assignation x_anx, y_anc
                                if (x)
                                        x_anc = *x + xmin;
                                else if(rel_x)
                                        x_anc = xmin + (int)(*rel_x * ((float)xmax - (float)xmin));
                                else
                                        x_anc = 0;
                                if(y)
                                        y_anc = *y + ymin;
                                else if (rel_y)
                                        y_anc = ymin + (int)(*rel_y * ((float)ymax - (float)ymin));
                                else
                                        y_anc = 0;

                                // On calcule la taille du widget

                                // Taille par defaut
                                // Priorité : taille fournie en argument > requested >
                                // defaut
                                screen_location.size = widget->requested_size;
                                int w;
                                int h;
                                bool size_abs = false;
                                w= widget->requested_size.width;
                                h = widget->requested_size.height;

                                if (width) {
                                        w = *width;
                                }
                                else if(rel_width){
                                        int rw = parent_rect.size.width;
                                        rw = (int)((float)rw * *rel_width);
                                        w = rw;
                                }

                                if(height){
                                        h = *height;
                                }
                                else if(rel_height){
                                        int rh = parent_rect.size.height;
                                        rh = (int)((float)rh * *rel_height);
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
                                // On recadre les valeurs qui debordent
                                x1 = (x1 >= xmin)?x1:xmin;
                                x2 = (x2 <= xmax)?x2:xmax;
                                y1 = (y1 >= ymin)?y1:ymin;
                                y2 = (y2 <= ymax )?y2:ymax;

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
                        else{
                                // on affiche pas le widget
                                screen_location = ei_rect_zero();
                        }
                        // Appel a geomnotify
                        widget->wclass->geomnotifyfunc(widget, screen_location);
                }
                // Gestion du root widget
                else{
                        if(x)
                                widget->screen_location.top_left.x = *x;

                        if(y)
                                widget->screen_location.top_left.y = *y;

                        if (width)
                                widget->requested_size.width = *width;

                        if (height)
                                widget->requested_size.height = *height;

                        widget->screen_location.size = widget->requested_size;
                        widget->content_rect = &widget->screen_location;
                }


                ei_rect_t new_pos = widget->screen_location;
                ei_app_invalidate_rect(&new_pos);


                //ei_place_rec(widget, false);

                // Appel récursif sur les enfants pour les replacer
                ei_widget_t *current = widget->children_head;
                ei_placer_param_t *current_param;
                while(current){
                        current_param = (ei_placer_param_t*)current->geom_params;
                        if (current_param){
                                ei_place(current, current_param->anc, current_param->x,
                                        current_param->y, current_param->w, current_param->h,
                                        current_param->rel_x, current_param->rel_y,
                                        current_param->rel_w, current_param->rel_h);}
                        else{
                                ei_place(current, NULL, NULL, NULL, NULL, NULL, NULL,
                                                NULL, NULL, NULL);
                        }
                        current = current->next_sibling;
                }
        }
}
