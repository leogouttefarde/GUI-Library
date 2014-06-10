
#include "ei_callback.h"
#include "ei_event.h"
#include "ei_common.h"


static ei_widget_t *pressed = NULL;
static ei_callback_t *callback = NULL;


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
        ei_placer_param_t param_sauv;
        param = (ei_placer_param_t*)widget->geom_params;
        param_sauv = *param;
        // On distingue le cas ou le widget a une taille absolue et le cas relatif
        if (param->w)
                rel_w_p = NULL;
        else
                a_w_p = NULL;

        if (param->h)
                rel_h_p = NULL;
        else
                a_h_p = NULL;

        ei_anchor_t anc = ei_anc_northwest;
        // Placement du widget pere avec ancrage northwest, point d'ancrage =
        // top_left
        ei_place(widget, &anc, &widget->screen_location.top_left.x, 
                        &widget->screen_location.top_left.y, a_w_p, 
                        a_h_p, NULL, NULL, rel_w_p , rel_h_p);
        /* VERSION AMELIORE - conservation des paramètres d'ancrage de
         * l'utilisateur
         *
         * 
        // Calcul de la nouvelle position du point d'ancrage
        // Position du widget
        int x1 = widget->screen_location.top_left.x;
        int y1 = widget->screen_location.top_left.y;
        int x2 = x1 + w - 1;
        int y2 = y1 + h - 1;

        // Calcul du nouveau point d'ancrage
        ei_anchor_t *anc = param_sauv.anc;
        if(!anc || !*anc){
        anc = malloc(sizeof(ei_anchor_t));
         *anc = ei_anchor_t ei_anc_northwest;
         }
         switch(*anc){
         case ei_anc_northwest:
         param->x = 
         break;
         case ei_anc_north:
         break;
         case ei_anc_northeast:
         break;
         case ei_anc_east:
         break;
         case ei_anc_southeast:
         break;
         case ei_anc_south:
         break;
         case ei_anc_southwest:
         break;
         case ei_anc_west:
         break;
         case ei_anc_center:
         break;
         default:
         break;
         }*/
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
