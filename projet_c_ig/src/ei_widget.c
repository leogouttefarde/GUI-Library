/**
 *  @file        ei_widget.c
 *
 *  @brief       API for widgets management: creation, configuration, hierarchy, redisplay.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 02.03.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 */

#include "ei_widget.h"
#include "ei_widgettypes.h"
#include "ei_utils.h"
#include "ei_root.h"
#include "ei_common.h"
#include "hw_interface.h"
#include "ei_utilities.h"
#include "ei_tag.h"
#include "ei_widgetclass_pv.h"


/* Contient la couleur de picking courante
 * le champ alpha doit toujours être a 0xFF*/
static ei_color_t ei_next_picking_color = { 0x00, 0x00, 0x00, 0xFF };

/* Incrémente la couleur de picking actuelle */
static void ei_color_increase(ei_color_t *color)
{
        if (color != NULL) {

                if(color->red < 0xFF)
                        color->red++;

                else if (color->green < 0xFF)
                        color->green++;

                else if(color->blue < 0xFF)
                        color->blue++;
        }
}

/* Creation d'un widget selon sa classe */
ei_widget_t* ei_widget_create(ei_widgetclass_name_t class_name,
                ei_widget_t* parent)
{
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
                }  else {
                        // on initialise correctement le root_widget
                        widget->next_sibling = NULL;
                        widget->children_head = NULL;
                        widget->children_tail = NULL;
                }

                // La couleur courante est une variable globale
                ei_color_t *pc = CALLOC_TYPE(ei_color_t);

                *pc = ei_next_picking_color;
                widget->pick_color = pc;

                ei_color_increase(&ei_next_picking_color);


                if (parent)
                        widget->pick_id = ei_map_rgba(ei_get_picking_surface(),
                                        widget->pick_color);
                else
                        widget->pick_id = 0x0;


                widget->requested_size = ei_size(100,100);

                widget->screen_location = ei_rect(ei_point_zero(),
                                widget->requested_size);
                widget->content_rect = &(widget->screen_location);

                // Initialisation des attributs uniques + requested_size si
                // texte
                if (wclass->setdefaultsfunc)
                        wclass->setdefaultsfunc(widget);


                ei_tag(widget, class_name);


                return widget;
        }
        else
                return NULL;
}

/* Detruit un widget */
void ei_widget_destroy(ei_widget_t* widget)
{
        if (widget) {
                ei_widget_t *current = widget->children_head, *next = NULL;
                ei_bool_t found = EI_FALSE;

                /* Remove widget from parent */
                ei_widget_t *parent = widget->parent;
                if (parent) {
                        current = parent->children_head;

                        /* Remove widget from parent head */
                        if (parent->children_head == widget) {
                                parent->children_head = widget->next_sibling;
                                found = EI_TRUE;
                        }

                        /* Remove widget from parent childs */
                        ei_widget_t *last = NULL;
                        while (current && !found) {
                                next = current->next_sibling;

                                if (next == widget) {
                                        current->next_sibling =
                                                widget->next_sibling;
                                        found = EI_TRUE;
                                }

                                last = current;
                                current = next;
                        }

                        /* Remove widget from parent tail */
                        if (parent->children_tail == widget) {
                                parent->children_tail = last;
                        }
                }

                /* Unmap geomatry manager */
                ei_geometrymanager_unmap(widget);


                /* Destroy all childs */
                current = widget->children_head;
                while (current) {
                        next = current->next_sibling;
                        ei_widget_destroy(current);
                        current = next;
                }

                /* Free widget */
                SAFE_FREE(widget->pick_color);

                if (widget->content_rect != &widget->screen_location)
                        SAFE_FREE(widget->content_rect);


                if (widget->wclass && widget->wclass->releasefunc)
                        widget->wclass->releasefunc(widget);


                SAFE_FREE(widget);
        }
}

/* Selectionne un widget en connaissant son pick_id */
static ei_widget_t* ei_widget_find_by_pick_id(uint32_t pick_id, ei_widget_t *widget)
{
        ei_widget_t* result;

        if(widget) {
                if (widget->parent) {
                        if ( widget->pick_id == pick_id) {
                                return widget;
                        }

                        result = ei_widget_find_by_pick_id(pick_id,
                                        widget->next_sibling);

                        if (result) {
                                return result;
                        }
                }
                return ei_widget_find_by_pick_id(pick_id, widget->children_head);
        } else {
                return NULL;
        }
}

/* Calcule la couleur correspondant au point du clic et cherche le widget
 * correspondant*/
ei_widget_t* ei_widget_pick(ei_point_t* where)
{
        ei_widget_t *selection = NULL;
        ei_size_t size;
        ei_surface_t picking_surface = ei_get_picking_surface();

        hw_surface_lock(picking_surface);

        // Génération de l'adresse mémoire du point "where"
        size = hw_surface_get_size(picking_surface);

        // On recupere l'adresse du premier pixel de la surface
        uint8_t *addr = hw_surface_get_buffer(picking_surface);

        if (addr) {
                uint32_t pick_id;

                // On recupere l'adresse du pixel donné en parametre
                addr = (addr + sizeof(uint32_t)*(where->x + (where->y)*size.width));

                pick_id = *(uint32_t*)addr;

                // On parcourt ensuite l'ensemble des widgets pour trouver le widget
                // correspondant
                selection = ei_widget_find_by_pick_id(pick_id, ei_get_root());
        }

        hw_surface_unlock(picking_surface);

        return selection;
}

/* Permet de configurer une frame */
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
        if (ei_has_widgetclass(widget, "frame")) {

                /* On recaste pour passer a un type frame */
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
                        make_string_copy(&frame->text, *text);
                }
                if (text_font) {
                        frame->text_font = *text_font;
                }
                if (text_color) {
                        frame->text_color = *text_color;
                }
                if (text_anchor) {
                        frame->text_anchor = *text_anchor;
                }
                if (img) {
                        frame->img = *img;
                }
                if (img_rect && *img_rect) {
                        SAFE_ALLOC(frame->img_rect, ei_rect_t);
                        frame->img_rect = *img_rect;
                } else {
                        SAFE_FREE(frame->img_rect);
                }
                if (img_anchor) {
                        frame->img_anchor = *img_anchor;
                }
        }
}

/* Permet de configurer un bouton */
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
        if (ei_has_widgetclass(widget, "button")) {

                ei_button_t *button = (ei_button_t*)widget;


                if (requested_size) {
                        button->widget.requested_size = *requested_size;
                }

                if (color) {
                        if (button->color)
                                *button->color = *color;
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
                        make_string_copy(&button->text, *text);
                }

                if (text_font) {
                        button->text_font = *text_font;
                }

                if (text_color) {
                        button->text_color = *text_color;
                }

                if (text_anchor) {
                        button->text_anchor = *text_anchor;
                }

                if (img) {
                        button->img=*img;
                }

                if (img_rect && *img_rect) {
                        SAFE_ALLOC(button->img_rect, ei_rect_t);
                        *button->img_rect = **img_rect;
                } else {
                        SAFE_FREE(button->img_rect);
                }

                if (img_anchor) {
                        button->img_anchor = *img_anchor;
                }

                if (callback) {
                        button->callback = *callback;
                }

                if (user_param){
                        button->user_param = *user_param;
                }
        }
}

/* Permet de configurer un toplevel */
void    ei_toplevel_configure   (ei_widget_t*   widget,
                ei_size_t*      requested_size,
                ei_color_t*     color,
                int*            border_width,
                char**          title,
                ei_bool_t*      closable,
                ei_axis_set_t*  resizable,
                ei_size_t**     min_size)
{
        if (ei_has_widgetclass(widget, "toplevel")) {

                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;


                if (border_width) {
                        toplevel->border_width = *border_width;
                }

                if (requested_size) {
                        ei_size_t requested_size_final = *requested_size;

                        requested_size_final.height = requested_size_final.height
                                + toplevel->bar_height + 2 * toplevel->border_width;
                        requested_size_final.width = requested_size_final.width 
                                + 2 * toplevel->border_width;

                        toplevel->widget.requested_size = requested_size_final;
                }

                if (color) {
                        toplevel->color = *color;
                }

                if (title) {
                        make_string_copy(&toplevel->title, *title);
                }

                if (closable) {
                        toplevel->closable = *closable;
                }

                if (resizable) {
                        toplevel->resizable = *resizable;
                }

                if (min_size && *min_size && toplevel->min_size) {
                        *toplevel->min_size = **min_size;
                } else {
                        SAFE_FREE(toplevel->min_size);
                }
        }
}


