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
#include <stdio.h>
#include "ei_widgetclass.h"
#include "ei_widgettypes.h"

// variable globale pour sotcker les tables de pointeur
static ei_widgetclass_t *frame_table = NULL;
static ei_widgetclass_t *button_table = NULL;
static ei_widgetclass_t *toplevel_table = NULL;
// Pour gérer les classes supplémentaires, on utilise l'attribut next
static ei_widgetclass_t *others_table = NULL;


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
        if (!strcmp(name,"frame")) {
                // Les fonctions liées à la classe frame sont déja declarées
                return frame_table;
        }
        else if (!strcmp(name,"button")){
                return button_table;
        }
        else if (!strcmp(name,"toplevel")){
                return toplevel_table;
        }
        else{
                return NULL;
        }
}

/***************************************** frame ****************/
// pointeur generique
void *frame_alloc(){
        ei_frame_t *frame = malloc(sizeof(ei_frame_t));
        memset(frame, 0, sizeof(ei_frame_t));

        return frame;
}

void frame_draw(struct ei_widget_t* widget, ei_surface_t surface,
                ei_surface_t pick_surface, ei_rect_t* clipper)
{
        ei_frame_t *frame;
        frame = (ei_frame_t*)widget;

        // lock de la surface
        hw_surface_lock(surface);

        /*
           printf("surface %X\n", surface);
           printf("frame->bg_color.red %X\n", frame->bg_color.red);
           printf("frame->bg_color.green %X\n", frame->bg_color.green);
           printf("frame->bg_color.blue %X\n", frame->bg_color.blue);
           printf("frame->bg_color.alpha %X\n", frame->bg_color.alpha);

           if (clipper) {
           printf("clipper->top_left->x %X\n", clipper->top_left.x);
           printf("clipper->top_left->y %X\n", clipper->top_left.y);

           printf("clipper->size->width %X\n", clipper->size.width);
           printf("clipper->size->height %X\n", clipper->size.height);
           }
           else
           printf("No clipper\n");
           */
        ei_fill(surface, &frame->bg_color, clipper);

        if (clipper) {
                int w;
                int h;
                int bw;
                w = clipper->size.width;
                h = clipper->size.height;
                bw = frame->border_width;

                if (frame->relief) {
                        ei_point_t tl = clipper->top_left;
                        // on génère les 4 bords sous forme de rectangle
                        ei_rect_t left = {tl, {bw, h}};
                        ei_rect_t top = {tl, {w, bw}};

                        // tr n'est pas tout a fait le top_right (décalé en x)
                        ei_point_t tr = {tl.x+w-bw, tl.y};
                        ei_rect_t right = {tr, {bw, h}};

                        // idem bl n'est pas tout a fait bottom_left (decalé en y)
                        //  AXE Y decroissant
                        ei_point_t bl = {tl.x, tl.y+h-bw};
                        ei_rect_t bottom = {bl, {w,bw}};

                        // definition des couleurs
                        ei_color_t dark = {0x11, 0x11, 0x11, 0xFF};
                        ei_color_t light = {0xDD, 0xDD, 0xDD, 0xDD};

                        // la disjonction sunken/raised commence ici
                        if (frame->relief == ei_relief_raised) {
                                ei_fill(surface, &dark,&left);
                                ei_fill(surface, &dark, &top);

                                ei_fill(surface, &light, &bottom);
                                ei_fill(surface, &light, &right);
                        }
                        else{
                                ei_fill(surface, &light,&left);
                                ei_fill(surface, &light, &top);

                                ei_fill(surface, &dark, &bottom);
                                ei_fill(surface, &dark, &right);
                        }
                }
        }
        //unlock de la surface
        hw_surface_unlock(surface);
        // hw_surface_free avant ou apres le unlock ??
}

void frame_release(struct ei_widget_t* widget){
        free((ei_frame_t*)widget);
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
        frame->img_rect = malloc(sizeof(ei_rect_t));
        if (frame->img_rect) {
                frame->img_rect->top_left = p;
                ei_size_t s = {10,10};
                frame->img_rect->size = s;
        }

        frame->relief = ei_relief_none;
        frame->text = "Frame";
        frame->text_anchor = ei_anc_center;
        // red green blue A
        ei_color_t tc = {0x00, 0x00, 0xFF, 0xFF};
        frame->text_color = tc;
        frame->text_font = ei_style_normal;
        ei_color_t bg = {0xFF,0x00,0x00,0xFF};
        frame->bg_color = bg;
}
void frame_geomnotify(struct ei_widget_t* widget, ei_rect_t rect){
        ;
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

        // Allocation
        frame_table = malloc(sizeof(ei_widgetclass_t));
        frame_table->allocfunc= frame_alloc;
        frame_table->drawfunc = frame_draw;
        frame_table->releasefunc = frame_release;
        frame_table->setdefaultsfunc = frame_setdefaults;
        frame_table->geomnotifyfunc = frame_geomnotify;
        strcpy(frame_table->name, "frame");
        frame_table->next = NULL;
}

/*************************************** button ***************/
// pointeur generique
void *button_alloc(){
        ei_button_t *button = malloc(sizeof(ei_button_t));
        memset(button, 0, sizeof(ei_button_t));

        return button;
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
        ei_color_t c = {0xFF, 0xFF, 0x00, 0xFF};
        button->color = &c;
        button->border_width = 2;
        button->corner_radius = 3;
        button->relief = ei_relief_raised;
        button->text = "Press me";
        ei_size_t s = {strlen(button->text)*5, 15};
        button->widget.requested_size = s;
        button->text_font = ei_style_normal;
        ei_color_t tc = {0xFF, 0xFF, 0xFF, 0xFF};
        button->text_color = tc;
        button->text_anchor = ei_anc_center;
        button->img = NULL;
        button->img_rect = malloc(sizeof(ei_rect_t));
        ei_point_t p = {10,10};
        if (button->img_rect) {
                button->img_rect->top_left = p;
                ei_size_t s = {10,10};
                button->img_rect->size = s;
        }
        button->img_anchor = ei_anc_center;
        button->callback = NULL;
        button->user_param = NULL;
}

void button_geomnotify(struct ei_widget_t* widget, ei_rect_t rect){
        ;
}
/**
 * \brief	Registers the "button" widget class in the program. This must be called only
 *		once before widgets of the class "button" can be created and configured with
 *		\ref ei_button_configure.
 */
void	ei_button_register_class(){
        // Allocation
        button_table = malloc(sizeof(ei_widgetclass_t));
        button_table->allocfunc= &button_alloc;
        button_table->drawfunc = &button_draw;
        button_table->releasefunc = &button_release;
        button_table->setdefaultsfunc = &button_setdefaults;
        button_table->geomnotifyfunc = &button_geomnotify;
        strcpy(button_table->name, "button");
        button_table->next = NULL;
}

/*************************** toplevel **************/
// pointeur generique
void *toplevel_alloc(){
        return (ei_toplevel_t*)malloc(sizeof(ei_toplevel_t));
} 

void toplevel_release(struct ei_widget_t* widget){
        free((ei_toplevel_t*)widget);
}

void toplevel_draw(struct ei_widget_t* widget, ei_surface_t surface,
                ei_surface_t pick_surface, ei_rect_t* clipper){
        ei_toplevel_t *toplevel;
        toplevel = (ei_toplevel_t*)widget;
        // lock de la surface
        hw_surface_lock(surface);
        ei_fill(surface, &toplevel->color,clipper);
        //unlock de la surface
        hw_surface_unlock(surface);


}
void toplevel_setdefaults(struct ei_widget_t* widget){
        // on commence par effectuer un recast
        &ei_toplevel_t *toplevel;
        toplevel = (ei_toplevel_t*)widget;
        ei_size_t s = {100, 100};
        toplevel->widget.requested_size = s;
        ei_color_t c = {0x00, 0xFF, 0xFF, 0xFF};
        toplevel->color = c;
        toplevel->border_width = 4;
        toplevel->title = "Toplevel";
        toplevel->closable = true;
        toplevel->resizable = true;
        ei_size_t ms = {50,50};
        toplevel->min_size = &ms;
}

void toplevel_geomnotify(struct ei_widget_t* widget, ei_rect_t rect){
        ;
}
/**
 * \brief	Registers the "toplevel" widget class in the program. This must be called only
 *		once before widgets of the class "toplevel" can be created and configured with
 *		\ref ei_toplevel_configure.
 */
void	ei_toplevel_register_class(){
        // Allocation
        toplevel_table = malloc(sizeof(ei_widgetclass_t));
        toplevel_table->allocfunc= &toplevel_alloc;
        toplevel_table->drawfunc = &toplevel_draw;
        toplevel_table->releasefunc = &toplevel_release;
        toplevel_table->setdefaultsfunc = &toplevel_setdefaults;
        toplevel_table->geomnotifyfunc = &toplevel_geomnotify;
        strcpy(toplevel_table->name, "toplevel");
        toplevel_table->next = NULL;
}
