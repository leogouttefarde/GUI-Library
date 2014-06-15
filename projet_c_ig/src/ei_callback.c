
#include "ei_callback.h"
#include "ei_radiobutton.h"
#include "ei_common.h"
#include "ei_widgetclass_pv.h"


/*
 * \brief       Currently pressed widget
 */
static ei_widget_t *pressed = NULL;

/*
 * \brief       Currently pressed widget's callback
 */
static ei_callback_t callback = NULL;


/* Gestion du move */
ei_bool_t all_callback_move_move(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param)
{
        UNUSED(widget);

        move((ei_widget_t*)user_param , event->param.mouse.where);

        return EI_FALSE;
}

/* Gestion du resize */
ei_bool_t all_callback_move_resize(ei_widget_t *widget, struct ei_event_t
                *event, void *user_param)
{
        UNUSED(widget);

        resize((ei_widget_t*)user_param, event->param.mouse.where);

        return EI_FALSE;
}

/* Si nécessaire effectue les bind avec toplevel en param */
ei_bool_t toplevel_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        UNUSED(user_param);


        assert(widget);

        if (ei_has_widgetclass(widget, "toplevel")) {
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;

                // On cherche si on a cliqué sur le titre, le resize, le close
                int m_x = event->param.mouse.where.x;
                int m_y = event->param.mouse.where.y;
                int t_h = toplevel->bar_height;

                // Le bouton close est a top_left + 1/4 * bar_height
                int c_s =   (int)floor((float)t_h * 1. / 4.);
                int r_s = toplevel->resize_size;
                int y = widget->screen_location.top_left.y;
                int x = widget->screen_location.top_left.x;
                int w = widget->screen_location.size.width;
                int h = widget->screen_location.size.height;


                pressed = widget;
                toplevel->move_pos = event->param.mouse.where;

                // PRIORITE : resize > close > move
                if (toplevel->resizable
                    && (m_y >= (y + h -1 - r_s))
                    && (m_x >= (x + w - 1 - r_s))) {

                        // Si resize, on bind ce widget et la fonction de resize
                        callback = all_callback_move_resize;
                        if (pressed)
                                ei_bind(ei_ev_mouse_move, NULL, "all", callback, (void*)toplevel);

                } else if (toplevel->closable
                           && m_y >= y + c_s
                           && m_y <= y + 3 * c_s
                           && m_x >= x + c_s
                           && m_x <= x + 3 * c_s) {

                        // On note simplement la demande de fermeture avec un
                        // booleen (pas de callback a appeler)
			toplevel->rel_btn_close = ei_relief_sunken;
                        toplevel->close = EI_TRUE;

                        ei_invalidate_rect(&widget->screen_location);
                }
                else if(m_y < y + t_h) {

                        // Si titre, on bind CE WIDGET et la fonction de deplacement
                        callback = all_callback_move_move;

                        if (pressed)
                                ei_bind(ei_ev_mouse_move, NULL, "all", callback, (void*)toplevel);
                }
        }

        return EI_FALSE;
}



/* Enfonce les boutons */
ei_bool_t button_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        UNUSED(event);
        UNUSED(user_param);

        if (ei_has_widgetclass(widget, "button")) {

                ei_button_t *button = (ei_button_t*)widget;
                button->relief = ei_relief_sunken;
                ei_invalidate_rect(&widget->screen_location);

                pressed = widget;
        }
        return EI_FALSE;
}

/* Gere le relachement de la souris */
ei_bool_t all_callback_release(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        UNUSED(widget);
        UNUSED(user_param);

        ei_bool_t done = EI_FALSE;

        /* Find all button widgets and release if pressed */
        if (ei_has_widgetclass(pressed, "button")) {

                ei_button_t *button = (ei_button_t*)pressed;
                button->relief = ei_relief_raised;

                ei_invalidate_rect(&pressed->screen_location);

                // Appel du callback du bouton seulement si curseur sur
                // le bouton
                if (button->callback) {

                        ei_widget_t *selected;
                        selected = ei_widget_pick(&event->param.mouse.where);

                        if (selected == (ei_widget_t*)button)
                                done = button->callback(selected, NULL, button->user_param);
                }
        }
        else if (ei_has_widgetclass(pressed, "toplevel")) {

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
                        int c_s = (int)floor((float)t_h * 1. / 4.);

                        if (toplevel->closable
                            && m_y >= y + c_s
                            && m_y <= y + 3 * c_s && m_x >= x + c_s
                            && m_x <= x + 3 * c_s)
                                ei_widget_destroy(pressed), pressed = NULL;

			else
                                toplevel->rel_btn_close = ei_relief_raised;
                }

                if (pressed)
                        toplevel->close = EI_FALSE;
        }

        if (pressed)
                pressed->geom_params->manager->runfunc(pressed);

        pressed = NULL;
        callback = NULL;

        return done;
}

/* Enfonce le bouton radio cliqué et relève les autres */
ei_bool_t radiobutton_callback_click(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        UNUSED(user_param);

        if (widget && !strcmp(widget->wclass->name, "radiobutton")) {

                ei_radiobutton_t *radiobutton = (ei_radiobutton_t*)widget;
                ei_linked_rdbtn_rec_t* lrec = radiobutton->lrec;
                ei_bool_t released = EI_TRUE;
                int m_x = event->param.mouse.where.x;
                int m_y = event->param.mouse.where.y;
                int id = 0;

                while (lrec && released) {
                        if (m_x >= lrec->rec.top_left.x
                            && m_x <= (lrec->rec.top_left.x + lrec->rec.size.width)
                            && m_y >= lrec->rec.top_left.y
                            && m_y <= (lrec->rec.top_left.y + lrec->rec.size.height)) {
                                released = EI_FALSE;
                        }

                        lrec = lrec->next;
                        id++;
                }

                if (!released) {
                        assert(radiobutton->lrec);
                        modify_btn_rel(radiobutton,id);
                }

                ei_invalidate_rect(&widget->screen_location);

                pressed = widget;
        }

        return EI_FALSE;
}
