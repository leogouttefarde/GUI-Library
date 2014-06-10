#include "ei_callback.h"
#include "ei_event.h"

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
}

ei_bool_t toplevel_callback_move_move(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param){
        ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
        int w = event->param.mouse.where.x - toplevel->move_pos.x + 1;
        int h = event->param.mouse.where.y - toplevel->move_pos.y + 1;
        move(widget, ei_size(w,h));
        // On sauvegarde le dernier point
        toplevel->move_pos = event->param.mouse.where;
        return EI_FALSE;
}

ei_bool_t toplevel_callback_move_resize(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param){
        ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
        int w = 0;
        int h = 0;
        // On verifie sur quels axes le widget est redimensionnable:
        switch (toplevel->resizable){
        case ei_axis_both :
                w = event->param.mouse.where.x - toplevel->move_pos.x + 1;
                h = event->param.mouse.where.y - toplevel->move_pos.y + 1;
        case ei_axis_x:
                w = event->param.mouse.where.x - toplevel->move_pos.x + 1;
        case ei_axis_y:
                h = event->param.mouse.where.y - toplevel->move_pos.y + 1;
        }

        resize(widget, ei_size(w,h));
        // On sauvegarde le dernier point
        toplevel->move_pos = event->param.mouse.where;
        return EI_FALSE;
}

ei_bool_t toplevel_callback_click(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param){
        ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
        toplevel->move_pos = event->param.mouse.where;
        // On cherche si on a cliqué sur le titre, le resize, le close
        int m_x = event->param.mouse.where.x;
        int m_y = event->param.mouse.where.y;
        int t_h = toplevel->title_height;
        int r_s = toplevel->resize_size;
        int y = widget->screen_location.top_left.y;
        int x = widget->screen_location.top_left.x;
        int w = widget->screen_location.size.width;
        int h = widget->screen_location.size.height;

        // On verifie que le toplevel est redimensionnable
        if(m_y < y + t_h){
                // Si titre, on bind CE WIDGET et la fonction de deplacement
                toplevel->move = true;
                toplevel->resize = false;
                ei_bind(ei_ev_mouse_move, widget, NULL, toplevel_callback_move_move, NULL);
        }
        else if (toplevel->resizable && m_y >= y + h -1 - r_s && m_x >= x + w - 1 - r_s){
                toplevel->move = false;
                toplevel->resize = true;
                // Si resize, on bind ce widget et la fonction de resize
                ei_bind(ei_ev_mouse_move, widget, NULL,
                                toplevel_callback_move_resize, NULL);
        }
        // TODO si close on supprime le widget
        return EI_FALSE;
}


// TODO A SUPPRIMER
// remplacer par un all_callback_release
ei_bool_t toplevel_callback_release(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param){
        ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
        // On regarde si le release etait précédé d'un déplacement ou d'un
        // redimensionnement
        if (toplevel->move){
                toplevel->move = false;
                toplevel->resize = false;
                ei_unbind(ei_ev_mouse_move, widget, NULL,
                                toplevel_callback_move_move, NULL);
        }
        else if (toplevel->resize){
                toplevel->move = false;
                toplevel->resize = false;
                ei_unbind(ei_ev_mouse_move, widget, NULL,
                                toplevel_callback_move_resize, NULL);
        }
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

// NE PAS SUPPRIMER
// gère le cas ou on  release sur le widget
// Quand on relache la souris sur le bouton
ei_bool_t button_callback_release(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        ei_bool_t res = EI_FALSE;

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
        return res;
}
