#include "ei_dynamics.h"

/***** Resize et move *****/

/* Fonction de redimensionnement
 * Conserve les proportions des fils 
 *
 * Pendant le redimensionnement, on ancre
 * le coin northwest pour plus de confort
 * */
void resize(ei_widget_t *widget, ei_size_t add_size)
{
        ei_geometrymanager_t *placer = ei_geometrymanager_from_name("placer");
        // La fonction resize ne fonctionne que sur le placer
        if (widget->geom_params && widget->geom_params->manager 
                        && widget->geom_params->manager == placer
                        && widget->parent && widget->parent->content_rect) {
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
                int* w = malloc(sizeof(int));
                int* h = malloc(sizeof(int));
                float* rel_w = malloc(sizeof(float));
                float* rel_h = malloc(sizeof(float));

                // Nouvelles positions
                int* x = malloc(sizeof(int));
                int* y = malloc(sizeof(int));
                float* rel_x = malloc(sizeof(float));
                float* rel_y = malloc(sizeof(float));



                // Calcul de la nouvelle taille absolue du widget
                *w = w_w + add_size.width;
                *h = w_h + add_size.height;

                /****/
                //Gestion du cas ou on atteint la min_size du toplevel
                // Plus facile de le placer ici que dans le geomnotify
                if (!strcmp(widget->wclass->name, "toplevel")){
                        ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
                        if(toplevel->min_size){       
                                *w = MAX(toplevel->min_size->width, *w);
                                *h = MAX(toplevel->min_size->height, *h);
                        }
                }
                /****/

                // Calcul des tailles relatives
                *rel_w = (float)*w / (float)p_w;
                *rel_h = (float)*h / (float)p_h;


                // Nouvelle position top_left, bottom_right du widget
                // (le redimensionnement se fait avec ancrage NW)
                // DANS LE REPERE DU PARENT
                int x1 = w_x - p_x;
                int x2 = x1 + *w - 1;
                int y1 = w_y - p_y;
                int y2 = y1 + *h - 1;

                // Calcul du nouveau point d'ancrage
                ei_anchor_t anc;
                if(!param->anc || !*param->anc)
                        anc = ei_anc_northwest;
                else
                        anc = *param->anc;

                switch(anc){
                case ei_anc_northwest:
                        *x = x1;
                        *y = y1;
                        break;
                case ei_anc_north:
                        *x = (x1 + x2) / 2;
                        *y = y1;
                        break;
                case ei_anc_northeast:
                        *x = x1 + *w - 1;
                        *y = y1;
                        break;
                case ei_anc_east:
                        *x = x1 + *w - 1;
                        *y = (y1 +y2) / 2;
                        break;
                case ei_anc_southeast:
                        *x = x1 + *w - 1;
                        *y = y1 + *h - 1;
                        break;
                case ei_anc_south:
                        *x = (x1 + x2) / 2;
                        *y = y1 + *h -1;
                        break;
                case ei_anc_southwest:
                        *x = x1;
                        *y = y1 + *h -1;
                        break;
                case ei_anc_west:
                        *x = x1;
                        *y = (y1 +y2) / 2;
                        break;
                case ei_anc_center:
                        *x = (x1 + x2) / 2;
                        *y = (y1 + y2) / 2;
                        break;
                default:
                        break;
                }

                // x y relatifs
                *rel_x = (float)(*x) / (float)(p_w -1);
                *rel_y = (float)(*y) / (float)(p_h -1);

                // On distingue le cas ou le widget a une taille absolue et le cas relatif
                if (param->w)
                        rel_w = NULL;
                else
                        w = NULL;

                if (param->h)
                        rel_h = NULL;
                else
                        h = NULL;
                if (param->x)
                        rel_x = NULL;
                else
                        x = NULL;

                if (param->y)
                        rel_y = NULL;
                else
                        y = NULL;

                // Placement du widget
                ei_place(widget, &anc, x, y, w, h, rel_x, rel_y, rel_w, rel_h);
        }
}


void move_placer(ei_widget_t *widget, ei_size_t dist){

}



/* Fonction de mouvement
 * Deplacement brut */
void move(ei_widget_t *widget, ei_size_t dist)
{
        assert(widget);
        ei_geometrymanager_t *placer = ei_geometrymanager_from_name("placer");
        // La fonction resize ne fonctionne que sur le placer
        if (widget->geom_params && widget->geom_params->manager 
                        && widget->geom_params->manager == placer) {
                ei_placer_param_t *param =
                        (ei_placer_param_t*)widget->geom_params;
                ei_anchor_t anc = ei_anc_northwest;

                int* x = malloc(sizeof(int));
                int* y = malloc(sizeof(int));
                float* rel_x = malloc(sizeof(int));
                float* rel_y = malloc(sizeof(int));
                int p_x;
                int p_y;
                int p_w;
                int p_h;
                // Position top_left et taille du widget
                int w_x = widget->screen_location.top_left.x;
                int w_y = widget->screen_location.top_left.y;



                // Idem parent
                if (widget->parent->content_rect) {
                        ei_rect_t p_rect = *widget->parent->content_rect;
                        p_x = p_rect.top_left.x;
                        p_y = p_rect.top_left.y;
                        p_w = p_rect.size.width;
                        p_h = p_rect.size.height;

                        // Nouveau x absolu (dans le repere du parent)
                        *x = w_x + dist.width - p_x;
                        *y = w_y + dist.height - p_y;

                        // Nouveau x relatif
                        if (param->x){
                                rel_x = NULL;
                        }
                        else{
                                *rel_x = (float)(*x) / (float)(p_w -1);
                                x = NULL;
                        }
                        if (param->y){
                                rel_y = NULL;
                        }
                        else{
                                *rel_y = (float)(*y) / (float)(p_h -1);
                                y = NULL;
                        }

                        // On deplace le pere
                        ei_place(widget, &anc, x, y, param->w, param->h, rel_x,
                                        rel_y, param->rel_w, param->rel_h);
                }
        }
}
