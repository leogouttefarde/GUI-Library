/**
 * @file        ei_widget.h
 *
 * @brief       API for widgets management: creation, configuration, hierarchy, redisplay.
 * 
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 */

// pour malloc et NULL
#include "ei_widget.h"
#include "ei_widgettypes.h"
#include "ei_utils.h"
#include "ei_core.h"
#include "ei_common.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"

// Couleur de picking courante, qu'on incrémente a chaque creation de widget
static ei_color_t current_pick_color = {0x00, 0x00, 0x00, 0xFF};

void increase_color(ei_color_t *color){
        if(color->red < 0xFF) {
                (color->red)++;
        }
        else{
                if (color->green < 0xFF) {
                        (color->green)++;
                }
                else{
                        if(color->blue <0xFF) {
                                (color->blue)++;
                        }
                        else{
                                // Trop de widgets créés
                                exit(-1);
                        }
                }
        }

}


/**
 * @brief       Creates a new instance of a widget of some particular class, as a descendant of
 *              an existing widget.
 *
 *              The widget is not displayed on screen until it is managed by a geometry manager.
 *              The widget should be released by calling \ref ei_widget_destroy when no more needed.
 *
 * @param       class_name      The name of the class of the widget that is to be created.
 * @param       parent          A pointer to the parent widget. Can not be NULL.
 *
 * @return                      The newly created widget, or NULL if there was an error.
 */

// Quels paramètres faut-il initialiser ici ?
ei_widget_t* ei_widget_create (ei_widgetclass_name_t class_name, 
                ei_widget_t* parent){
        ei_widget_t *widget = NULL;
        ei_widgetclass_t *wclass;

        // Configuration grace au paramètres
        wclass = ei_widgetclass_from_name(class_name);

        if (wclass && wclass->allocfunc)
                // après allocation, widget aura les champs communs + les champs uniques 
                widget = wclass->allocfunc();

        if (widget) {
                widget->wclass = wclass;

                // Initialisation des attributs communs
                if (parent) {
                        widget->parent = parent;
                        if (parent->children_tail) {
                                parent->children_tail->next_sibling = widget;
                                parent->children_tail = widget;
                        }

                        if (!parent->children_head) {
                                parent->children_head = widget;
                                parent->children_tail = widget;
                        }
                }
                // on initialise correctement le root_widget
                else {
                        widget->next_sibling = NULL;
                        widget->children_head = NULL;
                        widget->children_tail = NULL;
                }

                // La couleur courante est une variable globale
                ei_color_t *pc = CALLOC_TYPE(ei_color_t);

                *pc = current_pick_color;
                widget->pick_color = pc;
                increase_color(&current_pick_color);

                if (parent){
                        widget->pick_id = ei_map_rgba(ei_get_picking_surface(), widget->pick_color);
                }
                else{
                        widget->pick_id = 0x0;
                }
                widget->requested_size = ei_size(100,100);

                widget->screen_location = ei_rect(ei_point_zero(), widget->requested_size);
                widget->content_rect = &(widget->screen_location);

                // Initialisation des attributs uniques + requested_size si
                // texte
                if (wclass->setdefaultsfunc)
                        wclass->setdefaultsfunc(widget);

                return widget;
        }
        else {
                return NULL;}
}

/**
 * @brief       Destroys a widget. Removes it from screen if it is managed by a geometry manager.
 *              Destroys all its descendants.
 *
 * @param       widget          The widget that is to be destroyed.
 */
void ei_widget_destroy (ei_widget_t* widget){
        ei_widget_t *current;
        if (widget){
                current = widget->children_head;
                while (current) {
                        ei_widget_destroy(current);
                        current = current->next_sibling;
                }
                widget->wclass->releasefunc(widget);
        }
}


// Fonction auxiliaire recursive pour ei_widget_pick
ei_widget_t* ei_widget_sel (ei_surface_t pick_surface, uint32_t pick_id, ei_widget_t *widget){
        ei_widget_t* result;
        if(widget){
                if (widget->parent) {
                        if ( widget->pick_id == pick_id) {
                                return widget;
                        }
                        result = ei_widget_sel(pick_surface, pick_id, widget->next_sibling);
                        if (result){
                                return result;
                        }
                }
                return ei_widget_sel(pick_surface, pick_id, widget->children_head);
        }
        else{
                return NULL;
        }
}

/**
 * @brief       Returns the widget that is at a given location on screen.
 *
 * @param       where           The location on screen, expressed in the root window coordinates.
 *
 * @return                      The top-most widget at this location, or NULL if there is no widget
 *                              at this location (except for the root widget).
 */
ei_widget_t* ei_widget_pick (ei_point_t* where)
{
        ei_surface_t picking_surface = ei_get_picking_surface();

        hw_surface_lock(picking_surface);

        // Génération de l'adresse mémoire du point "where"
        ei_size_t size = hw_surface_get_size(picking_surface);

        // on recupere l'adresse du premier pixel de la surface
        uint8_t* addr = hw_surface_get_buffer(picking_surface);

        // on recupere l'adresse du pixel donné en parametre
        // addr +1 augmente d'un octet ou de 4 ? On suppose 1
        addr = (addr + 4*sizeof(uint8_t)*(where->x + (where->y)*size.width));

        // On parcourt ensuite l'ensemble des widgets pour trouver le widget
        // correspondant
        ei_widget_t *root = ei_get_root();
        ei_widget_t *selection;


        selection = ei_widget_sel(picking_surface, *(uint32_t*)addr, root);

        hw_surface_unlock(picking_surface);

        return selection;
}




/**
 * @brief       Configures the attributes of widgets of the class "frame".
 *
 *              Parameters obey the "default" protocol: if a parameter is "NULL" and it has never
 *              been defined before, then a default value should be used (default values are
 *              specified for each parameter). If the parameter is "NULL" but was defined on a
 *              previous call, then its value must not be changed.
 *
 * @param       widget          The widget to configure.
 * @param       requested_size  The size requested for this widget. The geometry manager may
 *                              override this size due to other constraints.
 *                              Defaults to the "natural size" of the widget, ie. big enough to
 *                              display the text or the image, or (0, 0) if the widget has no text
 *                              and no image.
 * @param       color           The color of the background of the widget. Defaults to
 *                              \ref ei_default_background_color.
 * @param       border_width    The width in pixel of the border decoration of the widget. The final
 *                              appearance depends on the "relief" parameter. Defaults to 0.
 * @param       relief          Appearance of the border of the widget. Defaults to
 *                              \ref ei_relief_none.
 * @param       text            The text to display in the widget, or NULL. Only one of the
 *                              parameter "text" and "img" should be used (i.e. non-NULL). Defaults
 *                              to NULL.
 * @param       text_font       The font used to display the text. Defaults to \ref ei_default_font.
 * @param       text_color      The color used to display the text. Defaults to 
 *                              \ref ei_font_default_color.
 * @param       text_anchor     The anchor of the text, i.e. where it is placed whithin the widget
 *                              when the size of the widget is bigger than the size of the text.
 *                              Defaults to \ref ei_anc_center.
 * @param       img             The image to display in the widget, or NULL. Any surface can be
 *                              used, but usually a surface returned by \ref hw_image_load. Only one
 *                              of the parameter "text" and "img" should be used (i.e. non-NULL).
 Defaults to NULL.
 * @param       img_rect        If not NULL, this rectangle defines a subpart of "img" to use as the
 *                              image displayed in the widget. Defaults to NULL.
 * @param       img_anchor      The anchor of the image, i.e. where it is placed whithin the widget
 *                              when the size of the widget is bigger than the size of the image.
 *                              Defaults to \ref ei_anc_center.
 */
void    ei_frame_configure (ei_widget_t* widget,
                ei_size_t*              requested_size,
                const ei_color_t*       color,
                int*                    border_width,
                ei_relief_t*            relief,
                char**                  text,
                ei_font_t*              text_font,
                ei_color_t*             text_color,
                ei_anchor_t*            text_anchor,
                ei_surface_t*           img,
                ei_rect_t**             img_rect,
                ei_anchor_t*            img_anchor)
{
        if (widget && widget->wclass
                && !strcmp(widget->wclass->name, "frame")) {

                // on recaste pour passer a un type frame
                ei_frame_t *frame = (ei_frame_t*)widget;

                if (requested_size) {
                        frame->widget.requested_size = *requested_size;
                }
                if (color) {
                        frame->bg_color = *color;
                }
                if (border_width) {
                        frame->border_width = *border_width;
                }
                if (relief) {
                        frame->relief = *relief;
                }
                if (text) {
                        frame->text = *text;
                }
                if (text_font){
                        frame->text_font = *text_font;
                }
                if (text_color){
                        frame->text_color = *text_color;
                }
                if (text_anchor){
                        frame->text_anchor = *text_anchor;
                }
                if (img){
                        frame->img = *img;
                }
                if (img_rect){
                        frame->img_rect = *img_rect;
                }
                if (img_anchor){
                        frame->img_anchor = *img_anchor;
                }


        }
}




/**
 * @brief       Configures the attributes of widgets of the class "button".
 *
 * @param       widget, requested_size, color, border_width, relief,
 *              text, text_font, text_color, text_anchor,
 *              img, img_rect, img_anchor
 *                              See the parameter definition of \ref ei_frame_configure.
 * @param       corner_radius   The radius (in pixels) of the rounded corners of the button.
 *                              0 means straight corners. Defaults to k_default_button_corner_radius.
 * @param       callback        The callback function to call when the user clicks on the button.
 *                              Defaults to NULL (no callback).
 * @param       user_param      A programmer supplied parameter that will be passed to the callback
 *                              when called. Defaults to NULL.
 */
void    ei_button_configure (ei_widget_t*               widget,
                ei_size_t*              requested_size,
                const ei_color_t*       color,
                int*                    border_width,
                int*                    corner_radius,
                ei_relief_t*            relief,
                char**                  text,
                ei_font_t*              text_font,
                ei_color_t*             text_color,
                ei_anchor_t*            text_anchor,
                ei_surface_t*           img,
                ei_rect_t**             img_rect,
                ei_anchor_t*            img_anchor,
                ei_callback_t*          callback,
                void**                  user_param)
{
        if (widget && widget->wclass
                && !strcmp(widget->wclass->name, "button")) {

                ei_button_t *button = (ei_button_t*)widget;

                if (requested_size) {
                        button->widget.requested_size = *requested_size;
                }
                if (color) {
                        button->color = color;
                }
                if (border_width) {
                        button->border_width = *border_width;
                }
                if (corner_radius) {
                        button->corner_radius = *corner_radius;
                }
                if (relief) {
                        button->relief = *relief;
                }
                if (text) {
                        button->text = *text;
                }
                if(text_font){
                        button->text_font = *text_font;
                }
                if(text_color){
                        button->text_color = *text_color;
                }
                if(text_anchor){
                        button->text_anchor = *text_anchor;
                }
                if(img) {
                        button->img = *img;
                }
                if(img_rect && *img_rect){
                        button->img_rect = CALLOC_TYPE(ei_rect_t);
                        *button->img_rect = **img_rect;
                }
                if(img_anchor) {
                        button->img_anchor = *img_anchor;
                }
                if(callback) {
                        button->callback = *callback;
                }
                if (user_param){
                        button->user_param = *user_param;
                }
        }
}


ei_bool_t resize_handle_motion(ei_widget_t *widget, ei_event_t *event, void *user_param);

ei_bool_t resize_handle_button_release(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
        assert(widget);
        if (widget) {
                ei_unbind(ei_ev_mouse_move, widget, NULL, resize_handle_motion, NULL);
                printf("resize_handle_button_release !\n");

                SAFE_FREE(user_param);
        }

        return EI_FALSE;
}

ei_bool_t resize_handle_motion(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
        assert(widget);
        assert(event);
        if (widget && event) {
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget->parent;
                assert(toplevel);

                ei_point_t *old_pos = &toplevel->move_pos;

                int x = event->param.mouse.where.x;
                int y = event->param.mouse.where.y;

                if (toplevel) {
                        //ei_place(widget, NULL, &x, &y, NULL, NULL, NULL, NULL, NULL, NULL);


                        ei_size_t add_size = { x - old_pos->x , y - old_pos->y };
                        *old_pos = event->param.mouse.where;
                        printf("resize!  x %d \t y %d\n", add_size.width, add_size.height);

                        resize(toplevel, add_size);
                }
        }

        return EI_FALSE;
}

ei_bool_t resize_handle_button_press(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
        assert(widget);
        if (widget) {
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget->parent;
                assert(toplevel);

                toplevel->move_pos = event->param.mouse.where;

                ei_bind(ei_ev_mouse_move, widget, NULL, resize_handle_motion, NULL);
                printf("resize_handle_button_press !\n");
        }

        return EI_FALSE;
}


ei_bool_t mv_handle_motion(ei_widget_t *widget, ei_event_t *event, void *user_param);

ei_bool_t mv_handle_button_release(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
        assert(widget);
        if (widget) {
                printf("mv_handle_button_release !\n");
                ei_unbind(ei_ev_mouse_move, widget, NULL, mv_handle_motion, NULL);
        }

        return EI_FALSE;
}

ei_bool_t mv_handle_motion(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
        assert(widget);
        assert(event);
        if (widget && event) {
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget->parent;
                assert(toplevel);

                ei_point_t *old_pos = &toplevel->move_pos;

                int x = event->param.mouse.where.x;
                int y = event->param.mouse.where.y;

                //ei_place(widget, NULL, &x, &y, NULL, NULL, NULL, NULL, NULL, NULL);

                if (toplevel) {
                        ei_size_t dist = { x - old_pos->x , y - old_pos->y };
                        *old_pos = event->param.mouse.where;

                        move(toplevel, dist);


                        printf("move!  x %d \t y %d\n", dist.width, dist.height);
                }
        }

        return EI_FALSE;
}

ei_bool_t mv_handle_button_press(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
        assert(widget);
        assert(event);

        if (widget) {
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget->parent;
                assert(toplevel);

                toplevel->move_pos = event->param.mouse.where;

                ei_bind(ei_ev_mouse_move, widget, NULL, mv_handle_motion, NULL);
                printf("mv_handle_button_press !\n");
        }

        return EI_FALSE;
}

void add_child(ei_widget_t *widget, ei_widget_t *child)
{
        if (widget) {
                ei_widget_t *tail = widget->children_tail;
                if (tail) {
                        tail->next_sibling = child;
                }
                else {
                        widget->children_head = child;
                        widget->children_tail = child;
                }
        }
}

/**
 * @brief       Configures the attributes of widgets of the class "toplevel".
 *
 * @param       widget          The widget to configure.
 * @param       requested_size  The content size requested for this widget, this does not include
 *                              the decorations (border, title bar). The geometry manager may
 *                              override this size due to other constraints.
 *                              Defaults to (320x240).
 * @param       color           The color of the background of the content of the widget. Defaults
 *                              to \ref ei_default_background_color.
 * @param       border_width    The width in pixel of the border of the widget. Defaults to 4.
 * @param       title           The string title diplayed in the title bar. Defaults to "Toplevel".
 * @param       closable        If true, the toplevel is closable by the user, the toplevel must
 *                              show a close button in its title bar. Defaults to \ref EI_TRUE.
 * @param       resizable       Defines if the widget can be resized horizontally and/or vertically
 *                              by the user. Defaults to \ref ei_axis_both.
 * @param       min_size        For resizable widgets, defines the minimum size. Defaults to
 *                              (160, 120).
 */
void    ei_toplevel_configure   (ei_widget_t*   widget,
                ei_size_t*      requested_size,
                ei_color_t*     color,
                int*            border_width,
                char**          title,
                ei_bool_t*      closable,
                ei_axis_set_t*  resizable,
                ei_size_t**     min_size)
{
        if (widget && widget->wclass
                && !strcmp(widget->wclass->name, "toplevel")) {

                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
                if (requested_size){
                        toplevel->widget.requested_size = *requested_size;
                }
                if (color){
                        toplevel->color = *color;
                }
                if (border_width) {
                        toplevel->border_width = *border_width;
                }
                if(title){
                        toplevel->title = *title;
                }
                if (closable){
                        toplevel->closable = *closable;
                }
                if(resizable){
                        toplevel->resizable = *resizable;
                }
                if(min_size){
                        toplevel->min_size = *min_size;
                }

                ei_widget_t *toplevel_title = ei_widget_create("frame", widget);
                ei_widget_t *square_widget = ei_widget_create("frame", widget);

                // add_child(toplevel, toplevel_title);
                // add_child(toplevel, square_widget);

                ei_size_t       frame_size              = {300,30};
                int             frame_x                 = 10;
                int             frame_y                 = 10;
                ei_color_t      frame_color             = {0xFF, 0, 0, 0xff};
                ei_relief_t     frame_relief            = ei_relief_raised;
                int             frame_border_width      = 6;

                ei_frame_configure(toplevel_title, &frame_size, &frame_color,
                                &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
                                NULL, NULL, NULL);

                ei_place(toplevel_title, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL);

                frame_y = 50;
                frame_color.red = 0;
                frame_color.green = 255;
                ei_frame_configure(square_widget, &frame_size, &frame_color,
                                &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
                                NULL, NULL, NULL);

                ei_place(square_widget, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL);


                ei_bind(ei_ev_mouse_buttondown, toplevel_title, NULL, mv_handle_button_press, NULL);
                ei_bind(ei_ev_mouse_buttonup, toplevel_title, NULL, mv_handle_button_release, NULL);

                ei_bind(ei_ev_mouse_buttondown, square_widget, NULL, resize_handle_button_press, NULL);
                ei_bind(ei_ev_mouse_buttonup, square_widget, NULL, resize_handle_button_release, NULL);


                // ei_unbind(ei_ev_mouse_buttonup, widget, NULL, mv_handle_button_release, NULL);
                // ei_unbind(ei_ev_mouse_buttonup, widget, NULL, resize_handle_button_release, NULL);
        }
}


