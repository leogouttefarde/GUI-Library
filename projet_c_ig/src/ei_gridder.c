#include "ei_gridder.h"
/***** Gridder *****/

// Renvoie le screen_location en partant du rectangle elementaire
// et des paramètres
ei_rect_t get_screen_location(ei_gridder_param_t *param, ei_rect_t elem_rect){
        ei_rect_t screen_location;
        int w = elem_rect.size.width;
        int h = elem_rect.size.height;
        int tl_x = elem_rect.top_left.x;
        int tl_y = elem_rect.top_left.y;
        if(param->col) 
                screen_location.top_left.x = *param->col * w + tl_x; 
        else
                screen_location.top_left.x = tl_x;
        if(param->lin) 
                screen_location.top_left.y = *param->lin * h + tl_y; 
        else
                screen_location.top_left.y = 0 + tl_y;
        if(param->w)
                screen_location.size.width = *param->w * w;
        else
                screen_location.size.width = w;
        if(param->h)
                screen_location.size.height = *param->h * h;
        else
                screen_location.size.height = h;

        return screen_location;
}

// Renvoie le rectangle élementaire en fonction du pere
ei_rect_t get_elem_rect(ei_widget_t *parent){

        ei_geometrymanager_t *gridder = ei_geometrymanager_from_name("gridder");
        // Calcul de la partition de la grille
        float l_max = 0.;
        float c_max = 0.;
        int l_curr;
        int c_curr;
        ei_widget_t *current = parent->children_head;
        ei_gridder_param_t *param;
        while(current){
                // On regarde si le fils courant est géré par le gridder
                if (current->geom_params && current->geom_params->manager &&
                                current->geom_params->manager == gridder){
                        // Lecture des paramètres
                        param =
                                (ei_gridder_param_t*)current->geom_params;
                        if(param){
                                l_curr = 0.;
                                c_curr = 0.;
                                if(param->lin){
                                        if(param->h)
                                                l_curr =
                                                        *param->lin + *param->h 
                                                        - 1;
                                        else
                                                l_curr =
                                                        *param->lin;
                                }
                                if(param->col){
                                        if(param->w)
                                                c_curr =
                                                        *param->col + *param->w
                                                        - 1;
                                        else
                                                c_curr =
                                                        *param->col;

                                }

                                l_max = (float)MAX(l_max, l_curr);
                                if (param->force_h)
                                        l_max = (float)MAX(l_max, *param->force_h);
                                c_max = (float)MAX(c_max, c_curr);
                                if (param->force_w)
                                        c_max = (float)MAX(c_max, *param->force_w);
                        }
                }
                current = current->next_sibling;
        }
        // On divise le père pour obtenir la taille d'un
        // rectangle elementaire
        ei_rect_t elem_rect = *parent->content_rect;
        elem_rect.size.width = (int)floor(
                        (float)elem_rect.size.width / (c_max + 1));
        elem_rect.size.height = (int)floor(
                        (float)elem_rect.size.height
                        / (l_max + 1));

        return elem_rect;
}

// Runfunc du gridder
void ei_grid_runfunc(ei_widget_t *widget){
        /* Principe
         * 
         * On remonte au père
         * On parcourt tous les fils du pere
         *    Pour chaque frere on regarde si gridder
         * On en deduit la taille max,  la taille min
         * On divise l'écran en carré elementaires
         * On utilise ei_place avec des params bien choisis
         *    ei_place appelle la runfunc
         * On reappelle ei_grid pour bien remettre les params
         */

        /* On commence par invalider l'ancien rectangle */
        ei_invalidate_rect(&widget->screen_location);

        if(widget->parent){
                // Lecture des paramètres
                ei_gridder_param_t *param =
                        (ei_gridder_param_t*)widget->geom_params;
                if (param){
                        // On calcule le rectangle élementaire
                        ei_rect_t elem_rect = get_elem_rect(widget->parent);
                        // On en déduit la screen_location 
                        ei_rect_t screen_location = get_screen_location(param,
                                        elem_rect);
                        // Appel a geomnotify
                        widget->wclass->geomnotifyfunc(widget, screen_location);

                        /* On invalide le nouveau rectangle*/
                        ei_rect_t new_pos = widget->screen_location;
                        ei_invalidate_rect(&new_pos);

                        /* Appels récursifs sur les enfants */
                        // Appel récursif sur les enfants pour les replacer
                        ei_widget_t *current = widget->children_head;
                        while(current  && current->geom_params &&
                                        current->geom_params->manager &&
                                        current->geom_params->manager->runfunc){
                                current->geom_params->manager->runfunc(current);
                                current = current->next_sibling;
                        }
                }
        }
}

// Gestion des paramètres
void ei_grid(ei_widget_t *widget, int *lin, int *col, int *w, int *h, int
                *force_w, int *force_h){

        ei_geometrymanager_t *gridder = ei_geometrymanager_from_name("gridder");
        assert(gridder);

        ei_bool_t gp_alloc = EI_FALSE;

        if (gridder && widget) {
                // On verifie que le widget est bien géré par le placeur,
                // sinon on le modifie pour qu'il le soit
                if (widget) {
                        gp_alloc = EI_TRUE;
                        if (widget->geom_params) {
                                if (widget->geom_params->manager) {
                                        if (widget->geom_params->manager != gridder) {
                                                widget->geom_params->manager->releasefunc(widget);
                                        }
                                        else{
                                                gp_alloc = EI_FALSE;
                                        }
                                }
                        }
                }

                if (gp_alloc) {
                        ei_gridder_param_t *param = CALLOC_TYPE(ei_gridder_param_t);

                        if (param) {
                                widget->geom_params = (ei_geometry_param_t*)param;
                                widget->geom_params->manager = gridder;
                                param->lin = CALLOC_TYPE(int);
                                param->col = CALLOC_TYPE(int);
                                param->w = CALLOC_TYPE(int);
                                param->h = CALLOC_TYPE(int);
                                param->force_w = CALLOC_TYPE(int);
                                param->force_h = CALLOC_TYPE(int);
                        }
                }
        }

        // Sauvegarde des paramètres
        ei_gridder_param_t *param = (ei_gridder_param_t*)widget->geom_params;
        assert(param);

        if (!param->lin)
                param->lin = CALLOC_TYPE(int);
        if (lin)
                *param->lin = *lin;
        else
                *param->lin = 0;

        if (!param->col)
                param->col = CALLOC_TYPE(int);
        if (col)
                *param->col = *col;
        else
                *param->col = 0;


        if (!param->w)
                param->w = CALLOC_TYPE(int);
        if(w)
                *param->w = *w;
        else
                *param->w = 1;

        if (!param->h)
                param->h = CALLOC_TYPE(int);
        if(h)
                *param->h = *h;
        else
                *param->h = 1;

        if(force_w){
                if (!param->force_w)
                        param->force_w = CALLOC_TYPE(int);
                *param->force_w = *force_w;
        }
        else{
                SAFE_FREE(param->force_w);
        }

        if(force_h){
                if (!param->force_h)
                        param->force_h = CALLOC_TYPE(int);
                *param->force_h = *force_h;
        }
        else{
                SAFE_FREE(param->force_h);
        }

        /* Appels de la runfunc sur tous les sur les freres pour bien les
         * replacer */
        // On marque le widget courant comme vu
        ei_widget_t *current = widget->parent->children_head;
        while(current &&
                        current->geom_params &&
                        current->geom_params->manager &&
                        current->geom_params->manager->runfunc){
                current->geom_params->manager->runfunc(current);
                current = current->next_sibling;
        }
}



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

                SAFE_FREE(param);
                widget->geom_params = NULL;
        }
}
// Register
/**
 * \brief       Registers the "gridder" geometry manager in the program. This must be called only
 *              once before the \ref ei_place function can be called.
 */
void  ei_register_gridder_manager()
{
        ei_geometrymanager_t *gridder = CALLOC_TYPE(ei_geometrymanager_t);
        assert(gridder);

        strcpy(gridder->name, "gridder");

        gridder->runfunc = ei_grid_runfunc;
        gridder->releasefunc = ei_grid_releasefunc;
        gridder->next = NULL;

        ei_geometrymanager_register(gridder);
}

