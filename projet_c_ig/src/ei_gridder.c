/**
 *  @file       ei_gridder.c
 *
 *  @brief      Functions associated with gridder.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 13.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 */

#include "ei_gridder.h"
#include "ei_geometrymanager.h"
#include "ei_common.h"
#include "ei_params.h"


/* Renvoie le screen_location en partant des paramètres géométriques du widget */
static ei_rect_t get_screen_location(ei_gridder_param_t *param, ei_point_t tl)
{
        ei_rect_t screen_location;

        //Rectangle élémentaire flottant
        float elem_width = param->elem_w;
        float elem_height = param->elem_h;
        if(param->col)
                screen_location.top_left.x = F2I(I2F(*param->col) * elem_width
                                + I2F(tl.x));
        else
                screen_location.top_left.x = tl.x;
        if(param->lin)
                screen_location.top_left.y = F2I(I2F(*param->lin) * elem_height
                                + I2F(tl.y));
        else
                screen_location.top_left.y = tl.y;

        if(param->w)
                screen_location.size.width = F2I(I2F(*param->w) * elem_width);
        else
                screen_location.size.width = F2I(elem_width);

        if(param->h)
                screen_location.size.height = F2I(I2F(*param->h) * elem_height);
        else
                screen_location.size.height = F2I(elem_height);


        return screen_location;
}

/* Affecte elem_width et elem_height de la taille du rectangle elementaire
 * flottant */
static void get_elem_rect(ei_widget_t *parent, float* elem_width, float* elem_height)
{
        ei_geometrymanager_t *gridder = ei_geometrymanager_from_name("gridder");

        if (gridder && elem_width && elem_height && parent
                        && parent->content_rect &&parent->children_head) {

                /* Calcul de la partition de la grille */
                float l_max = 0.;
                float c_max = 0.;
                int l_curr;
                int c_curr;
                ei_widget_t *current = parent->children_head;
                ei_gridder_param_t *param;

                /* On parcourt tous les fils du père (= freres du widget à placer) */
                while (current) {

                        if (current->geom_params && current->geom_params->manager &&
                                        current->geom_params->manager == gridder) {

                                // Lecture des paramètres
                                param = (ei_gridder_param_t*)current->geom_params;
                                if (param) {
                                        l_curr = 0.;
                                        c_curr = 0.;
                                        if (param->lin) {
                                                if (param->h)
                                                        l_curr =
                                                                *param->lin +
                                                                *param->h - 1;
                                                else
                                                        l_curr =
                                                                *param->lin;
                                        }
                                        if (param->col) {
                                                if (param->w)
                                                        c_curr =
                                                                *param->col
                                                                + *param->w - 1;
                                                else
                                                        c_curr =
                                                                *param->col;

                                        }

                                        l_max = I2F(MAX(l_max, l_curr));

                                        if (param->force_h)
                                                l_max = I2F(MAX(l_max, *param->force_h - 1));

                                        c_max = I2F(MAX(c_max, c_curr));

                                        if (param->force_w)
                                                c_max = I2F(MAX(c_max, *param->force_w - 1));

                                }
                        }

                        current = current->next_sibling;
                }

                // On divise le père pour obtenir la taille d'un
                // rectangle elementaire
                *elem_width = I2F(parent->content_rect->size.width)
                        / (c_max + 1);
                *elem_height = I2F(parent->content_rect->size.height)
                        / (l_max + 1);
        }
}

/* Runfunc du gridder */
void ei_grid_runfunc(ei_widget_t *widget)
{
        if(widget->parent) {

                // Lecture des paramètres
                ei_gridder_param_t *param = (ei_gridder_param_t*)widget->geom_params;

                if (param) {

                        // On calcule le rectangle élementaire
                        get_elem_rect(widget->parent, &param->elem_w, &param->elem_h);

                        // On en déduit la screen_location 
                        ei_rect_t screen_location = get_screen_location(param,
                                        widget->parent->content_rect->top_left);

                        // Appel a geomnotify
                        widget->wclass->geomnotifyfunc(widget, screen_location);


                        /* Appels récursifs sur les enfants */
                        // Appel récursif sur les enfants pour les replacer
                        ei_widget_t *current = widget->children_head;

                        while(current && current->geom_params
                                        && current->geom_params->manager
                                        && current->geom_params->manager->runfunc) {

                                current->geom_params->manager->runfunc(current);
                                current = current->next_sibling;
                        }
                }
        }
}

/* Gestion des paramètres */
void ei_grid(ei_widget_t *widget, int *lin, int *col,
                int *w, int *h, int*force_w, int *force_h)
{

        ei_geometrymanager_t *gridder = ei_geometrymanager_from_name("gridder");
        assert(gridder);

        ei_bool_t gp_alloc = EI_TRUE;

        if (gridder && widget) {

                // On verifie que le widget est bien géré par le gridder,
                // sinon on le modifie pour qu'il le soit
                if (widget->geom_params
                    && widget->geom_params->manager
                    && widget->geom_params->manager == gridder)
                        gp_alloc = EI_FALSE;

                if (gp_alloc) {
                        ei_geometrymanager_unmap(widget);

                        ei_gridder_param_t *param = CALLOC_TYPE(ei_gridder_param_t);

                        if (param) {
                                widget->geom_params = (ei_geometry_param_t*)param;
                                widget->geom_params->manager = gridder;
                        }
                }

                // Sauvegarde des paramètres
                ei_gridder_param_t *param = (ei_gridder_param_t*)widget->geom_params;
                assert(param);

                SAFE_ALLOC(param->lin, int);
                if (lin)
                        *param->lin = *lin;
                else
                        *param->lin = 0;

                SAFE_ALLOC(param->col, int);
                if (col)
                        *param->col = *col;
                else
                        *param->col = 0;


                SAFE_ALLOC(param->w, int);
                if (w)
                        *param->w = *w;
                else
                        *param->w = 1;

                SAFE_ALLOC(param->h, int);
                if (h)
                        *param->h = *h;
                else
                        *param->h = 1;

                if (force_w) {
                        SAFE_ALLOC(param->force_w, int);
                        *param->force_w = *force_w;
                }
                else
                        SAFE_FREE(param->force_w);

                if (force_h) {
                        SAFE_ALLOC(param->force_h, int);
                        *param->force_h = *force_h;
                }
                else
                        SAFE_FREE(param->force_h);

                param->elem_w = 1.0;
                param->elem_h = 1.0;

                /* Appels de la runfunc sur tous les sur les freres pour bien les replacer */
                ei_widget_t *current = widget->parent->children_head;

                while(current && current->geom_params
                      && current->geom_params->manager
                      && current->geom_params->manager->runfunc) {

                        current->geom_params->manager->runfunc(current);
                        current = current->next_sibling;
                }
        }
}

/* Release */
void ei_grid_releasefunc(struct ei_widget_t* widget)
{
        ei_gridder_param_t *param = (ei_gridder_param_t*)widget->geom_params;

        if (param) {
                SAFE_FREE(param->lin);
                SAFE_FREE(param->col);
                SAFE_FREE(param->w);
                SAFE_FREE(param->h);
                SAFE_FREE(param->force_w);
                SAFE_FREE(param->force_h);
        }
}

/**
 * \brief       Registers the "gridder" geometry manager in the program. This must be called only
 *              once before the \ref ei_place function can be called.
 */
void ei_register_gridder_manager()
{
        ei_geometrymanager_t *gridder = CALLOC_TYPE(ei_geometrymanager_t);
        assert(gridder);

        strcpy(gridder->name, "gridder");

        gridder->runfunc = ei_grid_runfunc;
        gridder->releasefunc = ei_grid_releasefunc;
        gridder->next = NULL;

        ei_geometrymanager_register(gridder);
}

