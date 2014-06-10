
#include "ei_callback.h"
#include "ei_event.h"
#include "ei_common.h"


static ei_widget_t *pressed = NULL;
static ei_callback_t *callback = NULL;


/* Fonction de redimensionnement
 * Conserve les proportions des fils */
void resize(ei_widget_t *widget, ei_size_t add_size){

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
        rel_w = (float)w / (float)p_w;
        rel_h = (float)h / (float)p_h;


        // Nouvelle position top_left, bottom_right du widget
        // (le redimensionnement se fait avec ancrage NW)
        int x1 = w_x;
        int x2 = x1 + w - 1;
        int y1 = w_y;
        int y2 = y1 + h - 1;

        // Calcul du nouveau point d'ancrage
        ei_anchor_t *anc;
        anc = malloc(sizeof(ei_anchor_t));
        if(!param->anc || !*param->anc)
                *anc = ei_anc_northwest;
        else
                *anc = *param->anc;

        switch(*anc){
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

        rel_x = (float)(x - p_x) / (float)(p_w -1);
        rel_y = (float)(y - p_y) / (float)(p_h -1);

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
        ei_place(widget, anc, x_p, y_p, w_p, h_p, rel_x_p, rel_y_p, rel_w_p, rel_h_p);
}

/* Fonction de mouvement
 * Deplacement brut */
void move(ei_widget_t *widget, ei_size_t dist)
{
        assert(widget);
        if (widget) {
                ei_placer_param_t *param =
                        (ei_placer_param_t*)widget->geom_params;
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
                ei_place(widget, &anc, &x, &y, param->w, param->h, NULL,
                                NULL, param->rel_w, param->rel_h);
        }
}

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

// toplevel-> all
ei_bool_t all_callback_move_resize(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param)
{
        ei_toplevel_t *toplevel = (ei_toplevel_t*)user_param;

        if (toplevel) {
                int w = 0;
                int h = 0;
                // On verifie sur quels axes le widget est redimensionnable:
                switch (toplevel->resizable){
                case ei_axis_both :
                        w = event->param.mouse.where.x - toplevel->move_pos.x;
                        h = event->param.mouse.where.y - toplevel->move_pos.y;
                        break;
                case ei_axis_x:
                        w = event->param.mouse.where.x - toplevel->move_pos.x;
                        break;
                case ei_axis_y:
                        h = event->param.mouse.where.y - toplevel->move_pos.y;
                        break;
                default : break;
                }

                resize(&toplevel->widget, ei_size(w,h));
                // On sauvegarde le dernier point
                toplevel->move_pos = event->param.mouse.where;
        }

        return EI_FALSE;
}

ei_bool_t toplevel_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        assert(widget);

        if (widget) {
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
                toplevel->move_pos = event->param.mouse.where;
                // On cherche si on a cliqué sur le titre, le resize, le close
                int m_x = event->param.mouse.where.x;
                int m_y = event->param.mouse.where.y;
                int t_h = toplevel->bar_height;
                int r_s = toplevel->resize_size;
                int y = widget->screen_location.top_left.y;
                int x = widget->screen_location.top_left.x;
                int w = widget->screen_location.size.width;
                int h = widget->screen_location.size.height;

                pressed = widget;

                // On verifie que le toplevel est redimensionnable
                if(m_y < y + t_h){
                        // Si titre, on bind CE WIDGET et la fonction de deplacement
                        callback = all_callback_move_move;
                }
                else if (toplevel->resizable && m_y >= y + h -1 - r_s && m_x >= x + w - 1 - r_s){
                        // Si resize, on bind ce widget et la fonction de resize
                        callback = all_callback_move_resize;
                }
                else
                        pressed = NULL;

                if (pressed)
                        ei_bind(ei_ev_mouse_move, NULL, "all", callback,
                                        toplevel);
        }

        // TODO si close on supprime le widget
        return EI_FALSE;
}


// TODO A SUPPRIMER
// remplacer par un all_callback_release
/*ei_bool_t toplevel_callback_release(ei_widget_t *widget, struct ei_event_t
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
return EI_FALSE;
}*/

// Peut-être pas dans le bon fichier
// Enfonce les boutons
ei_bool_t button_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        if (widget && !strcmp(widget->wclass->name, "button")) {

                ei_button_t *button = (ei_button_t*)widget;
                button->relief = ei_relief_sunken;
                button->clic = true;

                pressed = widget;
        }
        return EI_FALSE;
}

// TODO la gestion du relief doit etre faite par le callback
// all_callback_release
// gère le cas ou on  release sur le widget
// Quand on relache la souris sur le bouton
/*ei_bool_t button_callback_release(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
  {
  ei_bool_t done = EI_FALSE;

  if (widget){
  if (!strcmp(widget->wclass->name, "button")) {

  ei_button_t *button = (ei_button_t*)widget;
  if (button->clic){
  button->relief = ei_relief_raised;
  button->clic = false;
// Appel du callback du bouton
if (button->callback){
done = button->callback((ei_widget_t*)button, NULL, button->user_param);
}
}
}
}
return done;
}*/



ei_bool_t all_callback_release(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        ei_bool_t done = EI_FALSE;

        // Find all button widgets and release if pressed
        if (pressed && pressed->wclass) {
                if (!strcmp(pressed->wclass->name, "button")) {

                        ei_button_t *button = (ei_button_t*)pressed;
                        button->relief = ei_relief_raised;
                        button->clic = false; // Useless

                        // Appel du callback du bouton
                        if (button->callback){
                                done = button->callback((ei_widget_t*)button, NULL, button->user_param);
                        }
                }
                else if (callback && !strcmp(pressed->wclass->name, "toplevel")) {

                        ei_unbind(ei_ev_mouse_move, NULL, "all", callback, pressed);
                }
        }

        pressed = NULL;
        callback = NULL;

        return done;
}