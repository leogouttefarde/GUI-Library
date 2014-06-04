/**
 *  @file	widgetclass.h
 *  @brief	Definition and registration of widget classes.
 *
 *  \author 
 *  Created by François Bérard on 02.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "ei_widgetclass.h"
#include "ei_widgettypes.h"

// variable globale pour sotcker les tables de pointeur
static ei_widgetclass_t *frame_table;
static ei_widgetclass_t *button_table;
static ei_widgetclass_t *toplevel_table;
// Pour gérer les classes supplémentaires, on utilise l'attribut next
static ei_widgetclass_t *others_table;


/**
 * @brief	Registers a class to the program so that widgets of this class can be created.
 *		This must be done only once in the application.
 *
 * @param	widgetclass	The structure describing the class.
 */
void ei_widgetclass_register	(ei_widgetclass_t* widgetclass){
        ;
}


/**
 * @brief	Returns the structure describing a class, from its name.
 *
 * @param	name		The name of the class of widget.
 *
 * @return			The structure describing the class.
 */
ei_widgetclass_t* ei_widgetclass_from_name (ei_widgetclass_name_t name)
{
        if (strcmp(ei_widgetclass_stringname(name),"frame")){
                // Les fonctions liées à la classe frame sont déja declarées
                return frame_table;
        }
        return NULL;
}


/**
 * \brief	Registers the "frame" widget class in the program. This must be called only
 *		once before widgets of the class "frame" can be created and configured with
 *		\ref ei_frame_configure.
 */
// On utilise des pointeurs sur fonction
// Cette procedure leur donne une valeur
void	ei_frame_register_class (){
        // Declaration des fonctions liées à la classe frame

        // pointeur generique
        void *frame_alloc(){
                return (ei_frame_t*)malloc(sizeof(ei_frame_t));
        } 

        void frame_release(struct ei_widget_t* widget){
                free((ei_frame_t*)widget);
        }

        void frame_draw(struct ei_widget_t* widget, ei_surface_t surface,
                        ei_surface_t pick_surface, ei_rect_t* clipper){
                ei_frame_t *frame;
                frame = (ei_frame_t*)widget;
                // lock de la surface
                hw_surface_lock(surface);
                ei_fill(surface, frame->bg_color,clipper);
                if (frame->relief) {
                        // on recupere les 4 points du bord de la surface
                        ei_rect_t rect;
                        rect = hw_surface_get_rect(surface);
                        int w = rect.size.width;
                        int h = rect.size.height;

                        ei_point_t top_left = rect.top_left;
                        ei_point_t top_right = top_left;
                        top_right.x = top_right.x+w;
                        ei_point_t bottom_left = top_left;
                        bottom_left.y = top_left.y-h;
                        ei_point_t bottom_right = top_right;
                        bottom_right.y = top_right.y-h;
                        // on relie les 4 bords pour obtenir les deux moities du cadre
                        ei_linked_point_t* dark;
                        ei_linked_point_t* light;
                        ei_linked_point_t* tmp;

                        dark = malloc(sizeof(ei_linked_point_t));
                        light = malloc(sizeof(ei_linked_point_t));

                        ei_color_t dark_color = {0x11, 0x11, 0x11, 0xFF};
                        ei_color_t light_color = {0xDD, 0xDD, 0xDD, 0xDD};
                        tmp = dark;
                        tmp->point = bottom_left;
                        tmp->next = malloc(sizeof(ei_linked_point_t));
                        tmp = tmp->next;
                        tmp->point = top_left;
                        tmp->next = malloc(sizeof(ei_linked_point_t));
                        tmp = tmp->next;
                        tmp->point = top_right;
                        tmp->next = NULL;

                        tmp = light;
                        tmp->point = top_right;
                        tmp->next = malloc(sizeof(ei_linked_point_t));
                        tmp = tmp->next;
                        tmp->point = bottom_right;
                        tmp->next = malloc(sizeof(ei_linked_point_t));
                        tmp = tmp->next;
                        tmp->point = bottom_left;
                        tmp->next = NULL;

                        // la disjonction sunken/raised commence ici
                        if (frame->relief == ei_relief_raised) {
                                // A IMPLEMENTER : gestion d'une bordure de taille >1
                                ei_draw_polyline(surface, dark, dark_color,
                                                clipper);
                                ei_draw_polyline(surface, light,light_color, 
                                                clipper);

                        }
                        else{

                                ei_draw_polyline(surface, dark, light_color,
                                                clipper);
                                ei_draw_polyline(surface, light,dark_color, 
                                                clipper);
                                // A FAIRE
                        }
                }
                //unlock de la surface
                hw_surface_unlock(surface);


        }
        void frame_setdefaults(struct ei_widget_t* widget){
                // on commence par effectuer un recast
                ei_frame_t *frame;
                frame = (ei_frame_t*)widget;
                frame->border_width = 3;
                // ei_surface_t represente un pointeur générique
                frame->img = NULL;
                frame->img_anchor = ei_anc_center;
                ei_point_t p = {0,0};
                frame->img_rect->top_left = p;
                ei_size_t s = {10,10};
                frame->img_rect->size = s;
                frame->relief = ei_relief_none;
                frame->text = "Frame" ;
                frame->text_anchor = ei_anc_center;
                // red blue green A
                ei_color_t tc = {0x00, 0x00, 0xFF, 0xFF};
                frame->text_color = tc;
                frame->text_font = ei_style_normal;
                ei_color_t bg = {0xFF,0x00,0x00,0xFF};
                frame->bg_color = bg;
        }

        void frame_geomnotify(struct ei_widget_t* widget, ei_rect_t rect){
                ;
        }

        // Allocation
        extern ei_widgetclass_t *frame_table;
        frame_table->allocfunc= &frame_alloc;
        frame_table->drawfunc = &frame_draw;
        frame_table->releasefunc = &frame_release;
        frame_table->setdefaultsfunc = &frame_setdefaults;
        frame_table->geomnotifyfunc = &frame_geomnotify;
        frame_table->name[0] = 'f';
        frame_table->name[1] = 'r';
        frame_table->name[2] = 'a';
        frame_table->name[3] = 'm';
        frame_table->name[4] = 'e';
        frame_table->name[5] = '\0';
        frame_table->next = NULL;
}
/**
 * \brief	Registers the "button" widget class in the program. This must be called only
 *		once before widgets of the class "button" can be created and configured with
 *		\ref ei_button_configure.
 */
void	ei_button_register_class(){
        // Declaration des fonctions liées à la classe button

        // pointeur generique
        void *button_alloc(){
                return (ei_button_t*)malloc(sizeof(ei_button_t));
        } 

        void button_release(struct ei_widget_t* widget){
                free((ei_button_t*)widget);
        }

        void button_draw(struct ei_widget_t* widget, ei_surface_t surface,
                        ei_surface_t pick_surface, ei_rect_t* clipper){
                ei_button_t *button;
                button = (ei_button_t*)widget;
                // lock de la surface
                hw_surface_lock(surface);
                ei_fill(surface, button->color,clipper);
                //unlock de la surface
                hw_surface_unlock(surface);


        }
        void button_setdefaults(struct ei_widget_t* widget){
                // on commence par effectuer un recast
                ei_button_t *button;
                button = (ei_button_t*)widget;
        }

        void button_geomnotify(struct ei_widget_t* widget, ei_rect_t rect){
                ;
        }

        // Allocation
        extern ei_widgetclass_t *button_table;
        button_table->allocfunc= &button_alloc;
        button_table->drawfunc = &button_draw;
        button_table->releasefunc = &button_release;
        button_table->setdefaultsfunc = &button_setdefaults;
        button_table->geomnotifyfunc = &button_geomnotify;
        button_table->name[0] = 'b';
        button_table->name[1] = 'u';
        button_table->name[2] = 't';
        button_table->name[3] = 't';
        button_table->name[4] = 'o';
        button_table->name[5] = 'n';
        button_table->name[6] = '\0';
        button_table->next = NULL;
}

/**
 * \brief	Registers the "toplevel" widget class in the program. This must be called only
 *		once before widgets of the class "toplevel" can be created and configured with
 *		\ref ei_toplevel_configure.
 */
void ei_toplevel_register_class	(){
        ;
}
