
#include "ei_dynamics.h"

/* Calcule le point d'ancrage a partir du top_left et du bottom_right*/
ei_point_t top_left_to_anchor(ei_point_t tl, ei_point_t br, ei_anchor_t anc){
        int x1 = tl.x;
        int y1 = tl.y;
        int x2 = br.x;
        int y2 = br.y;

        int x;
        int y;

        switch(anc){
        case ei_anc_northwest:
                x = x1;
                y = y1;
                break;
        case ei_anc_north:
                x = (x1 + x2) / 2;
                y = y1;
                break;
        case ei_anc_northeast:
                x = x2;
                y = y1;
                break;
        case ei_anc_east:
                x = x2;
                y = (y1 +y2) / 2;
                break;
        case ei_anc_southeast:
                x = x2;
                y = y2;
                break;
        case ei_anc_south:
                x = (x1 + x2) / 2;
                y = y2;
                break;
        case ei_anc_southwest:
                x = x1;
                y = y2;
                break;
        case ei_anc_west:
                x = x1;
                y = (y1 +y2) / 2;
                break;
        case ei_anc_center:
                x = (x1 + x2) / 2;
                y = (y1 + y2) / 2;
                break;
        default:
                break;
        }

        return ei_point(x,y);

}

/* Fonction de resize pour le placer*/

void resize_placer(ei_widget_t *widget, ei_point_t where){
        ei_geometrymanager_t *placer = ei_geometrymanager_from_name("placer");
        // La fonction resize ne fonctionne que sur le placer
        if (widget->geom_params && widget->geom_params->manager 
                        && widget->geom_params->manager == placer
                        && widget->parent && widget->parent->content_rect) {
                // Calcul de la taille de redimensionnement
                if (!strcmp(widget->wclass->name, "toplevel")){
                        ei_size_t add_size;
                        ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;

                        if (toplevel) {
                                int w = 0;
                                int h = 0;

                                // ancienne position souris
                                int o_x = toplevel->move_pos.x;
                                int o_y = toplevel->move_pos.y;

                                // nouvelle position souris
                                int n_x = where.x;
                                int n_y = where.y;

                                // distance de deplacement
                                w = n_x - o_x;
                                h = n_y - o_y;


                                // On verifie sur quels axes le widget est redimensionnable:
                                switch (toplevel->resizable) {
                                case ei_axis_both :
                                        break;
                                case ei_axis_x:
                                        w = 0;
                                        break;
                                case ei_axis_y:
                                        h = 0;
                                        break;
                                default:
                                        w = 0;
                                        h = 0;
                                }

                                add_size = ei_size(w,h);
                        }

                        // On recupere les parametres
                        ei_placer_param_t *param;
                        param = (ei_placer_param_t*)widget->geom_params;

                        // Taille parent , ancienne taille widget
                        int p_x = widget->parent->content_rect->top_left.x;
                        int p_y = widget->parent->content_rect->top_left.y;
                        int p_w = widget->parent->content_rect->size.width;
                        int p_h = widget->parent->content_rect->size.height;
                        int w_w = widget->screen_location.size.width;
                        int w_h = widget->screen_location.size.height;
                        int w_x = widget->screen_location.top_left.x;
                        int w_y = widget->screen_location.top_left.y;
                        // Nouvelles tailles
                        int* w = CALLOC_TYPE(int);
                        int* h = CALLOC_TYPE(int);
                        float* rel_w = CALLOC_TYPE(float);
                        float* rel_h = CALLOC_TYPE(float);

                        // Nouvelles positions
                        int* x = CALLOC_TYPE(int);
                        int* y = CALLOC_TYPE(int);
                        float* rel_x = CALLOC_TYPE(float);
                        float* rel_y = CALLOC_TYPE(float);

                        // Calcul de la nouvelle taille absolue du widget
                        *w = w_w + add_size.width;
                        *h = w_h + add_size.height;

                        // Calcul des tailles relatives
                        *rel_w = (float)*w / (float)p_w;
                        *rel_h = (float)*h / (float)p_h;

                        // Nouvelle position top_left, bottom_right du widget
                        // (le redimensionnement se fait avec ancrage NW)
                        // DANS LE REPERE DU PARENT
                        ei_point_t tl = ei_point(w_x - p_x, w_y - p_y);
                        ei_point_t br = ei_point_add(tl, ei_point(*w - 1, *h - 1));

                        // Calcul du nouveau point d'ancrage
                        ei_anchor_t anc;
                        if(!param->anc || !*param->anc)
                                anc = ei_anc_northwest;
                        else
                                anc = *param->anc;

                        ei_point_t anc_point = top_left_to_anchor(tl, br, anc);
                        *x = anc_point.x;
                        *y = anc_point.y;

                        // x y relatifs
                        *rel_x = (float)(*x) / (float)(p_w -1);
                        *rel_y = (float)(*y) / (float)(p_h -1);

                        // On distingue le cas ou le widget a une taille absolue et le cas relatif
                        if (param->w)
                                SAFE_FREE(rel_w);
                        else
                                SAFE_FREE(w);

                        if (param->h)
                                SAFE_FREE(rel_h);
                        else
                                SAFE_FREE(h);
                        if (param->x)
                                SAFE_FREE(rel_x);
                        else
                                SAFE_FREE(x);

                        if (param->y)
                                SAFE_FREE(rel_y);
                        else
                                SAFE_FREE(y);

                        // Placement du widget
                        ei_place(widget, &anc, x, y, w, h, rel_x, rel_y, rel_w, rel_h);

                        // On sauvegarde le dernier point
                        toplevel->move_pos = where;

                        SAFE_FREE(w);
                        SAFE_FREE(h);
                        SAFE_FREE(rel_w);
                        SAFE_FREE(rel_h);
                        SAFE_FREE(x);
                        SAFE_FREE(y);
                        SAFE_FREE(rel_x);
                        SAFE_FREE(rel_y);
                }
        }
}

/* Fonction de resize pour le gridder*/
void resize_gridder(ei_widget_t *widget, ei_point_t where){


        ei_gridder_param_t *param =
                (ei_gridder_param_t*)widget->geom_params;

        // Position du curseur relativement au widget
        ei_point_t pos = where;
        pos.x = pos.x - widget->screen_location.top_left.x;
        pos.y = pos.y - widget->screen_location.top_left.y;

        // Calcul du rectangle elementaire
        float elem_w = param->elem_w;
        float elem_h = param->elem_h;

        //Calcul de la nouvelle ligne, colonne
        int *w = CALLOC_TYPE(int);
        int *h = CALLOC_TYPE(int);

        *w = F2I(I2F(pos.x) / elem_w) + 1;
        *h = F2I(I2F(pos.y) / elem_h) + 1;

        int *force_w = CALLOC_TYPE(int);
        int *force_h = CALLOC_TYPE(int);

        // On introduit un forcage
        if (param->force_w)
                *force_w = MAX(*param->col + *w, *param->force_w);
        if (param->force_h)
                *force_h = MAX(*param->lin + *h, *param->force_h);

        //Grid si changement
        if (*w != *param->w || *h != *param->h)
                ei_grid(widget, param->lin, param->col, w , h,force_w,
                                force_h);

        // Maj position toplevel
        ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
        toplevel->move_pos = where;
        //Free
        SAFE_FREE(w);
        SAFE_FREE(h);
        SAFE_FREE(force_w);
        SAFE_FREE(force_h);
}



/* Fonction de redimensionnement
 * Conserve les proportions des fils 
 *
 * Pendant le redimensionnement, on ancre
 * le coin northwest pour plus de confort
 * */
void resize(ei_widget_t *widget, ei_point_t where)
{
        assert(widget);
        ei_geometrymanager_t *placer = ei_geometrymanager_from_name("placer");
        ei_geometrymanager_t *gridder = ei_geometrymanager_from_name("gridder");
        if (widget->geom_params && widget->geom_params->manager 
                        && widget->geom_params->manager == placer) {
                resize_placer(widget, where);
        }
        else if (widget->geom_params && widget->geom_params->manager 
                        && widget->geom_params->manager == gridder) {
                resize_gridder(widget, where);
        }
}


void move_placer(ei_widget_t *widget, ei_point_t where){

        ei_placer_param_t *param =
                (ei_placer_param_t*)widget->geom_params;

        if (!strcmp(widget->wclass->name, "toplevel")){
                // On ne deplace que les toplevels en pratique
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;    
                // Calcul de la distance de deplacement
                ei_size_t dist;
                dist.width = where.x - toplevel->move_pos.x;
                dist.height = where.y - toplevel->move_pos.y;
                // Sauvegarde de la nouvelle position de la souris
                toplevel->move_pos = where;    
                // Calcul
                int* x = CALLOC_TYPE(int);
                int* y = CALLOC_TYPE(int);
                float* rel_x = CALLOC_TYPE(int);
                float* rel_y = CALLOC_TYPE(int);
                int p_x;
                int p_y;
                int p_w;
                int p_h;
                // Position top_left et taille du widget
                int w_x = widget->screen_location.top_left.x;
                int w_y = widget->screen_location.top_left.y;
                int w_w = widget->screen_location.size.width;
                int w_h = widget->screen_location.size.height;


                // Idem parent
                if (widget->parent->content_rect) {
                        ei_rect_t p_rect = *widget->parent->content_rect;
                        p_x = p_rect.top_left.x;
                        p_y = p_rect.top_left.y;
                        p_w = p_rect.size.width;
                        p_h = p_rect.size.height;

                        // Topleft et br du widget dans le repere du parent
                        // APRES deplacement
                        ei_point_t tl = ei_point(w_x + dist.width - p_x, w_y + dist.height- p_y);
                        ei_point_t br = ei_point_add(tl, ei_point(w_w - 1, w_h - 1));

                        // Calcul du nouveau point d'ancrage
                        ei_anchor_t anc;
                        if(!param->anc || !*param->anc)
                                anc = ei_anc_northwest;
                        else
                                anc = *param->anc;

                        ei_point_t anc_point = top_left_to_anchor(tl, br, anc);
                        *x = anc_point.x;
                        *y = anc_point.y;

                        // Nouveau x relatif
                        if (param->x){
                                SAFE_FREE(rel_x);
                        }
                        else{
                                *rel_x = (float)(*x) / (float)(p_w -1);
                                SAFE_FREE(x);
                        }
                        if (param->y){
                                SAFE_FREE(rel_y);
                        }
                        else{
                                *rel_y = (float)(*y) / (float)(p_h -1);
                                SAFE_FREE(y);
                        }

                        // On deplace le pere
                        ei_place(widget, &anc, x, y, param->w, param->h, rel_x,
                                        rel_y, param->rel_w, param->rel_h);

                        // Maj position toplevel
                        toplevel->move_pos = where;
                }

                //Free
                SAFE_FREE(x);
                SAFE_FREE(y);
                SAFE_FREE(rel_x);
                SAFE_FREE(rel_y);
        }
}
void move_gridder(ei_widget_t *widget, ei_point_t where){
        /* PRINCIPE
         *
         *  On calcule ecart = where - topleft
         *  On divise  ecart.x par width elementaire 
         *  (width elem = width widget / param->w)
         *  On divise ecart.y par height elementaire
         *  On obtient le nombre de cases a décaler
         *  On decale
         *
         *  Version naive, on aura des problemes aux bords
         *  mais pas si mal normalement
         */

        ei_gridder_param_t *param =
                (ei_gridder_param_t*)widget->geom_params;

        if (!strcmp(widget->wclass->name, "toplevel")){
                ei_rect_t location = widget->screen_location;
                // Calcul du rectangle elementaire
                float elem_w = param->elem_w;
                float elem_h = param->elem_h;


                // On trouve x,y tel que la souris soit a x, y blocs du widget
                // En theorie x,y valent 0 ou 1 sauf deplacement rapide de la
                // souris
                int x = where.x - location.top_left.x;
                int y = where.y - location.top_left.y;
                if (x<0) //<< Souris a gauche du widget
                        x = F2I(I2F(x) / elem_w);
                else if (x - location.size.width + 1 > 0)
                        x = F2I(I2F(x - location.size.width + 1) / elem_w) + 1;
                else
                        x = 0;

                if (y<0) //<< Souris a droite du widget
                        y = F2I(I2F(y) / elem_h);
                else if (y - location.size.height + 1 > 0)
                        y = F2I(I2F(y - location.size.height + 1) / elem_h ) + 1;
                else 
                        y = 0;

                // On calcule la nouvelle position
                int *col = CALLOC_TYPE(int);
                int *lin = CALLOC_TYPE(int);

                *col = *param->col + x;
                *lin = *param->lin + y;

                // Forcage
                int *force_w = CALLOC_TYPE(int);
                int *force_h = CALLOC_TYPE(int);

                *force_w = *param->w + MAX(*param->col, *col);
                *force_h = *param->h + MAX(*param->lin, *lin);
                if (param->force_w)
                        *force_w = MAX(*force_w, *param->force_w);
                if (param->force_h)
                        *force_h =  MAX(*force_h, *param->force_h);

                if (*lin != *param->lin || *col != *param->col)
                        ei_grid(widget, lin, col ,param->w, param->h, force_w,
                                        force_h);

                // Maj position toplevel
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
                toplevel->move_pos = where;

                //Free
                SAFE_FREE(lin);
                SAFE_FREE(col);
                SAFE_FREE(force_w);
                SAFE_FREE(force_h);
        }

}

/* Fonction de mouvement
 * Deplacement brut */
void move(ei_widget_t *widget, ei_point_t where)
{
        assert(widget);
        ei_geometrymanager_t *placer = ei_geometrymanager_from_name("placer");
        ei_geometrymanager_t *gridder = ei_geometrymanager_from_name("gridder");
        // La fonction resize ne fonctionne que sur le placer
        if (widget->geom_params && widget->geom_params->manager 
                        && widget->geom_params->manager == placer) {
                move_placer(widget, where);
        }
        else if (widget->geom_params && widget->geom_params->manager 
                        && widget->geom_params->manager == gridder) {
                move_gridder(widget, where);
        }
}
