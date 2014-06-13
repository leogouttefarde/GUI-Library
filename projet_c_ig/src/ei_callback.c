
#include "ei_callback.h"
#include <stdio.h>

static ei_widget_t *pressed = NULL;
static ei_callback_t callback = NULL;

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
                        && widget->geom_params->manager == placer) {
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
                int w;
                int h;
                float rel_w;
                float rel_h;

                // Nouvelles positions
                int x;
                int y;
                float rel_x;
                float rel_y;

                // Pointeurs sur les nouvelles tailles, positions
                int *w_p = &w;
                int *h_p = &h;
                float *rel_w_p = &rel_w;
                float *rel_h_p = &rel_h;

                int *x_p = &x;
                int *y_p = &y;
                float *rel_x_p = &rel_x;
                float *rel_y_p = &rel_y;


                // Calcul de la nouvelle taille absolue du widget
                w = w_w + add_size.width;
                h = w_h + add_size.height;
                //Gestion du cas ou on atteint la min_size du toplevel
                if (!strcmp(widget->wclass->name, "toplevel")){
                        ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
                        if(toplevel->min_size){       
                                w = MAX(toplevel->min_size->width, w);
                                h = MAX(toplevel->min_size->height, h);
                        }
                }
                rel_w = (float)w / (float)p_w;
                rel_h = (float)h / (float)p_h;



                // Nouvelle position top_left, bottom_right du widget
                // (le redimensionnement se fait avec ancrage NW)
                // DANS LE REPERE DU PARENT
                int x1 = w_x - p_x;
                int x2 = x1 + w - 1;
                int y1 = w_y - p_y;
                int y2 = y1 + h - 1;

                // Calcul du nouveau point d'ancrage
                ei_anchor_t anc;
                if(!param->anc || !*param->anc)
                        anc = ei_anc_northwest;
                else
                        anc = *param->anc;

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
                        x = x1 + w - 1;
                        y = y1;
                        break;
                case ei_anc_east:
                        x = x1 + w - 1;
                        y = (y1 +y2) / 2;
                        break;
                case ei_anc_southeast:
                        x = x1 + w - 1;
                        y = y1 + h - 1;
                        break;
                case ei_anc_south:
                        x = (x1 + x2) / 2;
                        y = y1 + h -1;
                        break;
                case ei_anc_southwest:
                        x = x1;
                        y = y + h -1;
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

                rel_x = (float)(x) / (float)(p_w -1);
                rel_y = (float)(y) / (float)(p_h -1);

                // On distingue le cas ou le widget a une taille absolue et le cas relatif
                if (param->w)
                        rel_w_p = NULL;
                else
                        w_p = NULL;

                if (param->h)
                        rel_h_p = NULL;
                else
                        h_p = NULL;
                if (param->x)
                        rel_x_p = NULL;
                else
                        x_p = NULL;

                if (param->y)
                        rel_y_p = NULL;
                else
                        y_p = NULL;

                // Placement du widget
                ei_place(widget, &anc, x_p, y_p, w_p, h_p, rel_x_p, rel_y_p, rel_w_p, rel_h_p);
        }
}



//TODO Version qui conserve la relativité
/* Fonction de mouvement
 * Deplacement brut */
void move(ei_widget_t *widget, ei_size_t dist)
{
        assert(widget);
        ei_geometrymanager_t *placer = ei_geometrymanager_from_name("placer");
        // La fonction resize ne fonctionne que sur le placer
        if (widget->geom_params && widget->geom_params->manager 
                        && widget->geom_params->manager == placer) {
                if (widget) {
                        ei_placer_param_t *param =
                                (ei_placer_param_t*)widget->geom_params;
                        ei_anchor_t anc = ei_anc_northwest;

                        int* x = malloc(sizeof(int));
                        int* y = malloc(sizeof(int));
                        float* rel_x = malloc(sizeof(int));
                        float* rel_y = malloc(sizeof(int));
                        int p_x = 0;
                        int p_y = 0;
                        int p_w = 1;
                        int p_h = 1;
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
                        }

                        // Nouveau x absolu (dans le repere du parent)
                        *x = w_x + dist.width - p_x;
                        *y = w_y + dist.height - p_y;

                        // Nouveau x relatif
                        if (param->x){
                                rel_x = NULL;
                        }
                        else{
                                *rel_x = (float)(*x) / (float)(p_w -1);
                                printf("rel x %f\n", *rel_x);
                                x = NULL;
                        }
                        if (param->y){
                                rel_y = NULL;
                        }
                        else{
                                *rel_y = (float)(*y) / (float)(p_h -1);
                                printf("y %f\n", (float)*y);
                                printf("rel y %f\n", *rel_y);
                                y = NULL;
                        }

                        // On deplace le pere
                        ei_place(widget, &anc, x, y, param->w, param->h, rel_x,
                                        rel_y, param->rel_w, param->rel_h);
                }
        }
}


/***** Callbacks *****/

// Gestion du mouvement de la souris pour un toplevel
ei_bool_t all_callback_move_move(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param)
{

        ei_toplevel_t *toplevel = (ei_toplevel_t*)user_param;
        if (toplevel) {
                int w = event->param.mouse.where.x - toplevel->move_pos.x;
                int h = event->param.mouse.where.y - toplevel->move_pos.y;
                move(&toplevel->widget, ei_size(w,h));

                // On sauvegarde le dernier point
                toplevel->move_pos = event->param.mouse.where;
        }

        return EI_FALSE;
}

// Gestion du resize
ei_bool_t all_callback_move_resize(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param)
{
        ei_toplevel_t *toplevel = (ei_toplevel_t*)user_param;

        if (toplevel) {
                int w = 0;
                int h = 0;

                // ancienne position souris
                int o_x = toplevel->move_pos.x;
                int o_y = toplevel->move_pos.y;

                // nouvelle position souris
                int n_x = event->param.mouse.where.x;
                int n_y = event->param.mouse.where.y;

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

                resize((ei_widget_t*)toplevel, ei_size(w,h));

                // On sauvegarde le dernier point
                toplevel->move_pos = event->param.mouse.where;
        }

        return EI_FALSE;
}

// Callback pour gérer le clic sur un toplevel(deplacement, redimensionnement,
// fermeture)
// Si nécessaire effectue les bind avec toplevel en param
ei_bool_t toplevel_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        assert(widget);

        if (widget && widget->wclass && !strcmp(widget->wclass->name, "toplevel")) {
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;

                // On cherche si on a cliqué sur le titre, le resize, le close
                int m_x = event->param.mouse.where.x;
                int m_y = event->param.mouse.where.y;
                int t_h = toplevel->bar_height;
                // le bouton close est a top_left + 1/4 * bar_height
                int c_s =   (int)floor((float)t_h * 1. / 4.);
                int r_s = toplevel->resize_size;
                int y = widget->screen_location.top_left.y;
                int x = widget->screen_location.top_left.x;
                int w = widget->screen_location.size.width;
                int h = widget->screen_location.size.height;


                pressed = widget;
                // Gestion du close
                if (toplevel->closable && (m_y >= y + c_s && m_y <= y +
                                        3 * c_s && m_x >= x + c_s && m_x <= x +
                                        3 * c_s)){
                        // On note simplement la demande de fermeture avec un
                        // booleen (pas de callback a appeler)
                        toplevel->close = EI_TRUE;
                }
                else{
                        // On sauvegarde le dernier point
                        toplevel->move_pos = event->param.mouse.where;

                        // On verifie que le toplevel est redimensionnable
                        if(m_y < y + t_h){
                                // Si titre, on bind CE WIDGET et la fonction de deplacement
                                callback = all_callback_move_move;
                        }
                        else if (toplevel->resizable && (m_y >= (y + h -1 - r_s)) && (m_x >= (x + w - 1 - r_s))){
                                // Si resize, on bind ce widget et la fonction de resize
                                callback = all_callback_move_resize;
                        }
                        else{
                                pressed = NULL;
                        }
                        if (pressed)
                                ei_bind(ei_ev_mouse_move, NULL, "all", callback,
                                                (void*)toplevel);
                }
        }
        return EI_FALSE;
}



// Enfonce les boutons
ei_bool_t button_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        if (widget && !strcmp(widget->wclass->name, "button")) {

                ei_button_t *button = (ei_button_t*)widget;
                button->relief = ei_relief_sunken;

                ei_app_invalidate_rect(&widget->screen_location);

                pressed = widget;
        }
        return EI_FALSE;
}


// Gere le relachement de la souris
ei_bool_t all_callback_release(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        ei_bool_t done = EI_FALSE;

        // Find all button widgets and release if pressed
        if (pressed && pressed->wclass) {
                if (!strcmp(pressed->wclass->name, "button")) {

                        ei_button_t *button = (ei_button_t*)pressed;
                        button->relief = ei_relief_raised;

                        ei_app_invalidate_rect(&pressed->screen_location);

                        // Appel du callback du bouton seulement si curseur sur
                        // le bouton
                        if (button->callback){
                                ei_widget_t *selected;
                                selected = ei_widget_pick(&event->param.mouse.where);
                                if (selected == (ei_widget_t*)button)
                                        done = button->callback(selected, NULL, button->user_param);
                        }
                }
                else if (!strcmp(pressed->wclass->name, "toplevel")) {

                        ei_toplevel_t *toplevel = (ei_toplevel_t*)pressed;

                        if (callback)
                                ei_unbind(ei_ev_mouse_move, NULL, "all", *callback, pressed);

                        if (toplevel->close) {
                                // On verifie que l'on a relaché sur le bouton
                                int y = pressed->screen_location.top_left.y;
                                int x = pressed->screen_location.top_left.x;
                                int m_x = event->param.mouse.where.x;
                                int m_y = event->param.mouse.where.y;
                                int t_h = toplevel->bar_height;
                                // le bouton close est a top_left + 1/4 * bar_height
                                int c_s =   (int)floor((float)t_h * 1. / 4.);
                                if (toplevel->closable && m_y >= y + c_s && m_y <= y +
                                                3 * c_s && m_x >= x + c_s && m_x <= x +
                                                3 * c_s){
                                        // RECHERCHE ET UNBIND
                                        // DESTRUCTION
                                        printf("Toplevel detruit");
                                        ei_widget_destroy(pressed);
                                        pressed = NULL;
                                }
                        }

                        if (pressed)
                                toplevel->close = EI_FALSE;
                }

                if (pressed)
                        pressed->geom_params->manager->runfunc(pressed);
        }

        pressed = NULL;
        callback = NULL;

        return done;
}
