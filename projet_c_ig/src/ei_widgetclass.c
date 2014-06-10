/**
 *  @file       widgetclass.h
 *  @brief      Definition and registration of widget classes.
 *
 *  \author 
 *  Created by François Bérard on 02.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */
#include "ei_widgetclass.h"
#include "ei_widgettypes.h"
#include "ei_shape.h"
#include "ei_utils.h"
#include "ei_button.h"
#include "ei_common.h"


// TODO : Gérer les widgetclass via des linkedlist
// et les parcourir pour avoir une unique variable statique

// variable globale pour stocker les tables de pointeur
static ei_widgetclass_t *frame_table = NULL;
static ei_widgetclass_t *button_table = NULL;
static ei_widgetclass_t *toplevel_table = NULL;

static ei_widgetclass_t *toplevel_title_table = NULL;
static ei_widgetclass_t *toplevel_close_table = NULL;
static ei_widgetclass_t *toplevel_resize_table = NULL;
// Pour gérer les classes supplémentaires, on utilise l'attribut next
static ei_widgetclass_t *others_table = NULL;


/**
 * @brief       Registers a class to the program so that widgets of this class can be created.
 *              This must be done only once in the application.
 *
 * @param       widgetclass     The structure describing the class.
 */
void ei_widgetclass_register    (ei_widgetclass_t* widgetclass)
{
        // TODO : Corriger en gérant les widgetclass via des linkedlist
        // et les parcourir dans ei_widgetclass_from_name
        // pour avoir une unique variable statique

        // Faux
        /*if (others_table){
          ei_widgetclass_t *tmp = others_table->next;
          others_table = widgetclass;
          widgetclass->next = tmp;
          }
          else {
          others_table = widgetclass;
          others_table->next = NULL;
          }*/
}


/**
 * @brief       Returns the structure describing a class, from its name.
 *
 * @param       name            The name of the class of widget.
 *
 * @return                      The structure describing the class.
 */
ei_widgetclass_t* ei_widgetclass_from_name (ei_widgetclass_name_t name)
{
        // TODO : Gérer les widgetclass via des linkedlist
        // et les parcourir pour avoir une unique variable statique

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
        else if (!strcmp(name, "tl_title")){
                return toplevel_title_table;
        }
        else if (!strcmp(name, "tl_close")){
                return toplevel_close_table;
        }
        else if (!strcmp(name, "tl_resize")){
                return toplevel_resize_table;
        }
        else{
                return NULL;
        }
}


/* Dessin de pick_surface */
void pick_surface_draw(ei_surface_t pick_surface, ei_widget_t *widget, ei_rect_t *clipper){
        hw_surface_lock(pick_surface);
        ei_linked_point_t lp;
        ei_rect_t rect = widget->screen_location;
        lp = ei_rect_to_points(rect);

        ei_draw_polygon(pick_surface, &lp, *widget->pick_color, clipper);
        hw_surface_unlock(pick_surface);        
}

/******************************************************************************/
/* Frame */

/* Allocation */
void *frame_alloc()
{
        ei_frame_t *frame = CALLOC_TYPE(ei_frame_t);
        assert(frame);

        return frame;
}

void frame_draw(struct ei_widget_t* widget, ei_surface_t surface,
                ei_surface_t pick_surface, ei_rect_t* clipper)
{
        ei_frame_t *frame = (ei_frame_t*)widget;
        assert(frame);
        assert(clipper);
        assert(surface);
        assert(pick_surface);

        /* Dessin visible */

        // lock de la surface
        if (surface){
                hw_surface_lock(surface);


                ei_rect_t rec;
                rec = frame->widget.screen_location;
                //rec.size = frame->widget.requested_size;

                ei_frame_draw(surface,rec,frame,clipper);

                //unlock de la surface
                hw_surface_unlock(surface);
        }
        if (pick_surface){
                /* Dessin de la surface de picking */
                pick_surface_draw(pick_surface, widget, clipper);
        }
}

void frame_release(struct ei_widget_t* widget)
{
        SAFE_FREE(widget);
}
void frame_setdefaults(struct ei_widget_t* widget)
{
        assert(widget);

        // on commence par effectuer un recast
        ei_frame_t *frame;
        frame = (ei_frame_t*)widget;
        frame->border_width = 3;
        // ei_surface_t represente un pointeur générique
        frame->img = NULL;
        frame->img_anchor = ei_anc_center;
        frame->img_rect = CALLOC_TYPE(ei_rect_t);

        if (frame->img_rect) {
                frame->img_rect->top_left = ei_point_zero();
                frame->img_rect->size = ei_size(10,10);
        }

        frame->relief = ei_relief_none;
        frame->text = NULL;
        frame->text_anchor = ei_anc_center;
        // red green blue A
        ei_color_t tc = {0x00, 0x00, 0xFF, 0xFF};
        frame->text_color = tc;
        frame->text_font = ei_default_font;

        // On obtient la taille correspondant au text voulu
        // Exemple ici avec frame mais surtout utile pour button
        // DONNE SEG_FAULT
        /*int w;
          int h;
          hw_text_compute_size("Frame", frame->text_font, &w, &h);
          frame->widget.requested_size = (ei_size(w,h));*/
        if (frame->text && frame->text_font)
                // frame->widget.requested_size = ei_size(20, strlen(frame->text)*10);
                hw_text_compute_size(frame->text, frame->text_font,
                                &frame->widget.requested_size.width,
                                &frame->widget.requested_size.height);
        else
                frame->widget.requested_size = ei_size(100,100);
        ei_color_t bg = {0xFF,0x00,0x00,0xFF};
        frame->bg_color = bg;
}

// PRINCIPE : déduit le content_rect de la screen_location
void frame_geomnotify(struct ei_widget_t* widget, ei_rect_t rect)
{
        ei_rect_t* content_rect;
        content_rect = malloc(sizeof(ei_rect_t));
        if (rect.size.width !=0 && rect.size.height != 0){
                widget->screen_location = rect;
                // La screen_location est copiée tel quel
                ei_frame_t *frame = (ei_frame_t*)widget;
                // Gestion des bordures pour le content_rect
                int bw = frame->border_width;
                *content_rect = rect;
                content_rect->top_left.x =  content_rect->top_left.x +
                        bw;

                content_rect->top_left.y =  content_rect->top_left.y +
                        bw;

                content_rect->size.width =  content_rect->size.width +
                        - 2*bw;
                content_rect->size.height =  content_rect->size.height +
                        -2*bw;
        }
        else{
                widget->screen_location = ei_rect_zero();
                content_rect = &widget->screen_location;
        }
        widget->content_rect = content_rect;
}

/**
 * \brief       Registers the "frame" widget class in the program. This must be called only
 *              once before widgets of the class "frame" can be created and configured with
 *              \ref ei_frame_configure.
 */
// On utilise des pointeurs sur fonction
// Cette procedure leur donne une valeur
void    ei_frame_register_class ()
{
        // Declaration des fonctions liées à la classe frame

        // Allocation
        frame_table = calloc(1, sizeof(ei_widgetclass_t));
        assert(frame_table);

        frame_table->allocfunc= frame_alloc;
        frame_table->drawfunc = frame_draw;
        frame_table->releasefunc = frame_release;
        frame_table->setdefaultsfunc = frame_setdefaults;
        frame_table->geomnotifyfunc = frame_geomnotify;
        strcpy(frame_table->name, "frame");

        frame_table->next = NULL;
}

/******************************************************************************/
/* Boutons */

// pointeur generique
void *button_alloc()
{
        ei_button_t *button = CALLOC_TYPE(ei_button_t);
        assert(button);

        return button;
}

void button_release(struct ei_widget_t* widget)
{
        SAFE_FREE(widget);
}

void button_draw(struct ei_widget_t* widget, ei_surface_t surface,
                ei_surface_t pick_surface, ei_rect_t* clipper)
{
        ei_button_t *button = (ei_button_t*)widget;
        assert(button);
        assert(clipper);
        assert(pick_surface);
        assert(surface);

        if (surface){
                // lock de la surface
                hw_surface_lock(surface);
                ei_button_draw(surface,button->widget.screen_location,button, clipper);
                //unlock de la surface
                hw_surface_unlock(surface);
        }

        if (pick_surface){
                /* Dessin de la surface de picking */
                pick_surface_draw(pick_surface, widget, clipper);
        }
}

void button_setdefaults(struct ei_widget_t* widget)
{
        assert(widget);

        // on commence par effectuer un recast
        ei_button_t *button;
        button = (ei_button_t*)widget;

        ei_color_t *color = CALLOC_TYPE(ei_color_t);
        assert(color);

        color->red = 0xFF;
        color->green = 0xFF;
        color->blue = 0x00;
        color->alpha = 0xFF;

        button->color = color;


        button->border_width = 2;
        button->corner_radius = 3;
        button->relief = ei_relief_raised;
        button->text = NULL;
        button->text_font = ei_default_font;
        ei_color_t tc = {0xFF, 0xFF, 0xFF, 0xFF};
        button->text_color = tc;
        button->text_anchor = ei_anc_center;
        /*int w;
          int h;
          hw_text_compute_size(button->text, button->text_font, &w, &h);
          button->widget.requested_size = (ei_size(w,h));
          */
        if (button->text && button->text_font)
                hw_text_compute_size(button->text, button->text_font,
                                &button->widget.requested_size.width,
                                &button->widget.requested_size.height);
        else
                button->widget.requested_size = ei_size(100,20);
        button->img = NULL;
        button->img_rect = CALLOC_TYPE(ei_rect_t);
        assert(button->img_rect);

        if (button->img_rect) {
                ei_point_t p = {10,10};
                button->img_rect->top_left = p;

                ei_size_t s = {10,10};
                button->img_rect->size = s;
        }

        button->img_anchor = ei_anc_center;
        button->callback = NULL;
        button->user_param = NULL;
        button->clic = false;
}

void button_geomnotify(struct ei_widget_t* widget, ei_rect_t rect)
{
        ei_rect_t* content_rect;
        content_rect = malloc(sizeof(ei_rect_t));
        if (rect.size.width !=0 && rect.size.height != 0){
                widget->screen_location = rect;
                ei_button_t *button = (ei_button_t*)widget;
                // Gestion des bordures pour le content_rect
                int bw = button->border_width;
                *content_rect = rect;
                content_rect->top_left.x =  content_rect->top_left.x +
                        bw;

                content_rect->top_left.y =  content_rect->top_left.y +
                        bw;

                content_rect->size.width =  content_rect->size.width +
                        - 2*bw;
                content_rect->size.height =  content_rect->size.height +
                        -2*bw;
        }
        else{
                widget->screen_location = ei_rect_zero();
                content_rect = &widget->screen_location;
        }
        widget->content_rect = content_rect;
}
/**
 * \brief       Registers the "button" widget class in the program. This must be called only
 *              once before widgets of the class "button" can be created and configured with
 *              \ref ei_button_configure.
 */
void    ei_button_register_class()
{
        // Allocation
        button_table = CALLOC_TYPE(ei_widgetclass_t);
        assert(button_table);

        button_table->allocfunc= button_alloc;
        button_table->drawfunc = button_draw;
        button_table->releasefunc = button_release;
        button_table->setdefaultsfunc = button_setdefaults;
        button_table->geomnotifyfunc = button_geomnotify;
        strcpy(button_table->name, "button");
        button_table->next = NULL;
}


/********************************************************************************/
/*************************** toplevel **************/
// pointeur generique
void *toplevel_alloc()
{
        ei_toplevel_t *toplevel = CALLOC_TYPE(ei_toplevel_t);
        assert(toplevel);

        return toplevel;
}

void toplevel_release(ei_widget_t *widget)
{
        SAFE_FREE(widget);
}

void toplevel_draw(ei_widget_t *widget, ei_surface_t surface,
                ei_surface_t pick_surface, ei_rect_t *clipper)
{
        ei_toplevel_t *toplevel;
        toplevel = (ei_toplevel_t*)widget;
        assert(toplevel);
        assert(clipper);
        assert(pick_surface);
        assert(surface);

        if (surface){
                // lock de la surface
                hw_surface_lock(surface);

                ei_linked_point_t lp =
                        ei_rect_to_points(*toplevel->widget.content_rect);
                ei_draw_polygon(surface, &lp, toplevel->color, clipper);

					 ei_bar_draw(surface,toplevel,clipper);
                //unlock de la surface
                hw_surface_unlock(surface);
        }

        if (pick_surface){
                /* Dessin de la surface de picking */
                pick_surface_draw(pick_surface, widget, clipper);
        }
}

void toplevel_setdefaults(struct ei_widget_t* widget)
{
        // on commence par effectuer un recast
        ei_toplevel_t *toplevel;
        toplevel = (ei_toplevel_t*)widget;
        assert(toplevel);

        ei_size_t s = {100, 100};
        toplevel->widget.requested_size = s;
		  toplevel->bar_height=20;

		  toplevel->rel_btn_close=ei_relief_raised;

        ei_color_t c = {0x00, 0x00, 0x00, 0xFF};
        toplevel->color = c;

        toplevel->border_width = 4;
        toplevel->title = "Toplevel";
        toplevel->closable = true;
        toplevel->resizable = true;

        ei_size_t *ms = CALLOC_TYPE(ei_size_t);
        assert(ms);
        ms->width = 50;
        ms->height = 50;

        toplevel->min_size = ms;

        // Gestion du move, resize
        toplevel->move_pos = ei_point(0,0);
        toplevel->move = false;
        toplevel->resize = false;
        toplevel->resize_size = 10;
}

void toplevel_geomnotify(struct ei_widget_t* widget, ei_rect_t rect)
{
        ei_rect_t *content_rect;
        content_rect = malloc(sizeof(ei_rect_t));
		  //printf("width rect:%i\n",rect.size.width);
		  //printf("height rect:%i\n",rect.size.height);
        if (rect.size.width != 0 && rect.size.height !=0){
			  //printf("CAS IF \n");
                widget->screen_location = rect;
                // La screen_location est copiée tel quel
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
                // Gestion des bordures pour le content_rect
                int bw = toplevel->border_width;
                *content_rect = rect;
                content_rect->top_left =plus(rect.top_left,0,toplevel->bar_height);
					 /*
                content_rect->size.width =  content_rect->size.width +
                        - 2*bw;
                content_rect->size.height =  content_rect->size.height +
                        -2*bw;
					*/
        }
        else{
			  //printf("on est ds else\n");
                widget->screen_location = ei_rect_zero();
                content_rect = &widget->screen_location;
        }
        widget->content_rect = content_rect;
		  
}

/**
 * \brief       Registers the "toplevel" widget class in the program. This must be called only
 *              once before widgets of the class "toplevel" can be created and configured with
 *              \ref ei_toplevel_configure.
 */
void    ei_toplevel_register_class()
{
        // Allocation
        toplevel_table = CALLOC_TYPE(ei_widgetclass_t);
        assert(toplevel_table);

        toplevel_table->allocfunc= toplevel_alloc;
        toplevel_table->drawfunc = toplevel_draw;
        toplevel_table->releasefunc = toplevel_release;
        toplevel_table->setdefaultsfunc = toplevel_setdefaults;
        toplevel_table->geomnotifyfunc = toplevel_geomnotify;
        strcpy(toplevel_table->name, "toplevel");

        toplevel_table->next = NULL;
}
