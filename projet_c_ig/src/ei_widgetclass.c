/**
 *  @file       widgetclass.c
 *  @brief      Definition and registration of widget classes.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 02.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#include "ei_widgetclass_pv.h"
#include "ei_widgettypes.h"
#include "ei_utils.h"
#include "ei_button.h"
#include "ei_common.h"
#include "ei_utilities.h"
#include "ei_linkedlist.h"
#include "ei_radiobutton.h"
#include "ei_core.h"
#include "ei_tag.h"

/* widgetclass linked list */
static ei_linkedlist_t ei_class_list = { NULL, NULL };

/* Libération des classes */
void ei_widgetclass_free()
{
        ei_linkedlist_empty(&ei_class_list, EI_TRUE);
}

/* Enregistre une classe quelconque */
void ei_widgetclass_register    (ei_widgetclass_t* widgetclass)
{
        if (widgetclass) {
                ei_tag_create(widgetclass->name);
                ei_linkedlist_add(&ei_class_list, widgetclass);
        }
}

/* Renvoie true si le widget a une classe définie */
ei_bool_t ei_has_widgetclass(ei_widget_t *widget, ei_widgetclass_name_t name)
{
        return (widget && widget->wclass && !strcmp(widget->wclass->name, name));
}


/* Renvoie la structure décrivant une classe en fonction du nom */
ei_widgetclass_t* ei_widgetclass_from_name (ei_widgetclass_name_t name)
{
        ei_widgetclass_t *class = NULL;
        ei_bool_t found = EI_FALSE;
        ei_linked_elem_t *link = ei_class_list.head;

        while (link && !found) {
                class = (ei_widgetclass_t*)link->elem;

                if (class && !strncmp(class->name, name, sizeof(ei_widgetclass_name_t)))
                        found = EI_TRUE;

                link = link->next;
        }

        if (!found)
                class = NULL;

        return class;
}


/* Dessin de pick_surface */
void pick_surface_draw(ei_surface_t pick_surface, ei_widget_t *widget, ei_rect_t *clipper)
{
        hw_surface_lock(pick_surface);

        ei_draw_rectangle(pick_surface, widget->pick_color, &widget->screen_location, clipper);

        hw_surface_unlock(pick_surface);
}

/***** Frame *****/

/* Allocation */
void *frame_alloc()
{
        ei_frame_t *frame = CALLOC_TYPE(ei_frame_t);
        assert(frame);

        return frame;
}

/* Dessin */
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
                ei_frame_draw(surface,rec,frame,clipper);

                //unlock de la surface
                hw_surface_unlock(surface);
        }
        if (pick_surface){
                /* Dessin de la surface de picking */
                pick_surface_draw(pick_surface, widget, clipper);
        }
}

/* Release */
void frame_release(struct ei_widget_t* widget)
{
        if (widget) {
                ei_frame_t *frame = (ei_frame_t*)widget;

                SAFE_FREE(frame->text);
                SAFE_FREE(frame->img_rect);

                SAFE_FREE(frame);
        }
}

/* Paramètres par défaut */
void frame_setdefaults(struct ei_widget_t* widget)
{
        assert(widget);

        // On commence par effectuer un recast
        ei_frame_t *frame = (ei_frame_t*)widget;

        frame->border_width = 0;
        frame->img = NULL;
        frame->img_anchor = ei_anc_center;
        frame->img_rect = NULL;
        frame->relief = ei_relief_none;
        frame->text = NULL;
        frame->text_anchor = ei_anc_center;
        frame->text_font = ei_default_font;

        if (frame->text && frame->text_font)
                hw_text_compute_size(frame->text, frame->text_font,
                                &frame->widget.requested_size.width,
                                &frame->widget.requested_size.height);
        else
                frame->widget.requested_size = ei_size(100,100);
        frame->bg_color = ei_default_background_color;
}

/* Calcule du content_rect, de la screen_location */
void frame_geomnotify(struct ei_widget_t* widget, ei_rect_t rect)
{
        // On invalide l'ancienne position
        ei_invalidate_rect(&widget->screen_location);


        ei_rect_t* content_rect = widget->content_rect;

        if (rect.size.width !=0 && rect.size.height != 0){

                if (    (content_rect == &widget->screen_location)
                                || (content_rect == NULL))
                        content_rect = CALLOC_TYPE(ei_rect_t);


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
        } else{
                if (content_rect != &widget->screen_location)
                        SAFE_FREE(widget->content_rect);

                widget->screen_location = ei_rect_zero();
                content_rect = &widget->screen_location;
        }

        widget->content_rect = content_rect;

        // On invalide la nouvelle position
        ei_invalidate_rect(&widget->screen_location);
}

/* Enregistre la classe frame */
void    ei_frame_register_class ()
{
        // Declaration des fonctions liées à la classe frame
        ei_widgetclass_t *frame_class = NULL;

        // Allocation
        frame_class = CALLOC_TYPE(ei_widgetclass_t);
        assert(frame_class);

        frame_class->allocfunc= frame_alloc;
        frame_class->drawfunc = frame_draw;
        frame_class->releasefunc = frame_release;
        frame_class->setdefaultsfunc = frame_setdefaults;
        frame_class->geomnotifyfunc = frame_geomnotify;
        strcpy(frame_class->name, "frame");
        frame_class->next = NULL;

        ei_widgetclass_register(frame_class);
}

/***** Boutons *****/

/* Allocation */
void *button_alloc()
{
        ei_button_t *button = CALLOC_TYPE(ei_button_t);
        assert(button);

        return button;
}

/* Libération */
void button_release(struct ei_widget_t* widget)
{
        if (widget) {
                ei_button_t *button = (ei_button_t*)widget;

                SAFE_FREE(button->color);
                SAFE_FREE(button->text);
                SAFE_FREE(button->img_rect);

                SAFE_FREE(button);
        }
}

/* Dessin */
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
                ei_button_draw(surface,button->widget.screen_location,button,
                                clipper);
                //unlock de la surface
                hw_surface_unlock(surface);
        }
        if (pick_surface){
                /* Dessin de la surface de picking */
                pick_surface_draw(pick_surface, widget, clipper);
        }
}

/* Defaut */
void button_setdefaults(struct ei_widget_t* widget)
{
        assert(widget);

        // on commence par effectuer un recast
        ei_button_t *button;
        button = (ei_button_t*)widget;

        ei_color_t *color = CALLOC_TYPE(ei_color_t);
        assert(color);

        if (color != NULL) {
                color->red = 0xFF;
                color->green = 0xFF;
                color->blue = 0x00;
                color->alpha = 0xFF;
        }

        button->color = color;


        button->border_width = 2;
        button->corner_radius = k_default_button_corner_radius;
        button->relief = ei_relief_raised;
        button->text = NULL;
        button->text_font = ei_default_font;
        ei_color_t tc = {0xFF, 0xFF, 0xFF, 0xFF};
        button->text_color = tc;
        button->text_anchor = ei_anc_center;

        if (button->text && button->text_font)
                hw_text_compute_size(button->text, button->text_font,
                                &button->widget.requested_size.width,
                                &button->widget.requested_size.height);
        else
                button->widget.requested_size = ei_size(100,20);


        button->img = NULL;
        button->img_rect = NULL;

        if (button->img_rect) {
                ei_point_t p = {10,10};
                button->img_rect->top_left = p;

                ei_size_t s = {10,10};
                button->img_rect->size = s;
        }

        button->img_anchor = ei_anc_center;
        button->callback = NULL;
        button->user_param = NULL;
}

/* Calcul screen_location, content_rect */
void button_geomnotify(struct ei_widget_t* widget, ei_rect_t rect)
{
        ei_invalidate_rect(&widget->screen_location);

        ei_rect_t* content_rect = widget->content_rect;


        if (rect.size.width !=0 && rect.size.height != 0) {

                if ((content_rect == &widget->screen_location)
                                || (content_rect == NULL))
                        content_rect = CALLOC_TYPE(ei_rect_t);


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
        } else {
                if (content_rect != &widget->screen_location)
                        SAFE_FREE(content_rect);

                widget->screen_location = ei_rect_zero();
                content_rect = &widget->screen_location;
        }

        widget->content_rect = content_rect;

        // On invalide la nouvelle position
        ei_invalidate_rect(&widget->screen_location);
}

/* Enregistre la classe bouton */
void    ei_button_register_class()
{
        ei_widgetclass_t *button_class = NULL;

        // Allocation
        button_class = CALLOC_TYPE(ei_widgetclass_t);
        assert(button_class);

        button_class->allocfunc= button_alloc;
        button_class->drawfunc = button_draw;
        button_class->releasefunc = button_release;
        button_class->setdefaultsfunc = button_setdefaults;
        button_class->geomnotifyfunc = button_geomnotify;
        strcpy(button_class->name, "button");
        button_class->next = NULL;

        ei_widgetclass_register(button_class);
}


/***** Toplevels *****/

/* Allocation */
void *toplevel_alloc()
{
        ei_toplevel_t *toplevel = CALLOC_TYPE(ei_toplevel_t);
        assert(toplevel);

        return toplevel;
}

/* Libération */
void toplevel_release(ei_widget_t *widget)
{
        if (widget) {
                ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;

                SAFE_FREE(toplevel->title);
                SAFE_FREE(toplevel->min_size);

                SAFE_FREE(toplevel);
        }
}

/* Dessin */
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

                ei_toplevel_draw(surface,toplevel,clipper);

                //unlock de la surface
                hw_surface_unlock(surface);
        }

        if (pick_surface){
                /* Dessin de la surface de picking */
                pick_surface_draw(pick_surface, widget, clipper);
        }
}

/* Defaut */
void toplevel_setdefaults(struct ei_widget_t* widget)
{
        // on commence par effectuer un recast
        ei_toplevel_t *toplevel;
        toplevel = (ei_toplevel_t*)widget;
        assert(toplevel);

        ei_size_t size = { 320, 240 };
        toplevel->widget.requested_size = size;

        toplevel->bar_height = 25;

        ei_color_t bar_color = { 255, 255, 255, 255 };
        toplevel->bar_color = bar_color;

        toplevel->rel_btn_close = ei_relief_raised;

        toplevel->color = ei_default_background_color;

        toplevel->border_width = 4;

        make_string_copy(&toplevel->title, "Toplevel");
        toplevel->title_font = ei_default_font;

        ei_color_t title_color = { 0, 0, 0, 255 };
        toplevel->title_color = title_color;


        ei_size_t *min_size = CALLOC_TYPE(ei_size_t);
        assert(min_size);

        min_size->width = 160;
        min_size->height = 120;

        toplevel->min_size = min_size;


        // Gestion du move, resize
        toplevel->move_pos = ei_point(0,0);
        toplevel->resize_size = 10;

        // Pas de demande de fermeture par defaut
        toplevel->closable = EI_TRUE;
        toplevel->resizable = ei_axis_both;
        toplevel->close = EI_FALSE;
}

/* Calculs screen_location, content_rect */
void toplevel_geomnotify(struct ei_widget_t* widget, ei_rect_t rect)
{
        // On invalide l'ancienne screen_location
        ei_invalidate_rect(&widget->screen_location);

        ei_rect_t screen_location = rect;

        //Gestion de la min_size
        ei_toplevel_t *toplevel = (ei_toplevel_t*)widget;
        if(toplevel->min_size){
                screen_location.size.width = MAX(toplevel->min_size->width,
                                screen_location.size.width);
                screen_location.size.height = MAX(toplevel->min_size->height,
                                screen_location.size.height);
        }

        // Le bouton de resize doit rester visible et les toplevel ne pas
        // devenir infiniment petite
        screen_location.size.width = MAX(toplevel->resize_size,
                        screen_location.size.width);
        screen_location.size.height = MAX(toplevel->resize_size,
                        screen_location.size.height);


        ei_rect_t* content_rect = widget->content_rect;

        // Calcul du content_rect en prenant en compte les bordures
        if (screen_location.size.width != 0 && screen_location.size.height !=0){

                if ((content_rect == &widget->screen_location)
                                || (content_rect == NULL))
                        content_rect = CALLOC_TYPE(ei_rect_t);


                widget->screen_location = screen_location;

                // Gestion des bordures pour le content_rect
                int bw = toplevel->border_width;
                *content_rect = screen_location;
                content_rect->top_left = plus(screen_location.top_left, bw , bw
                                + toplevel->bar_height);
                content_rect->size.height = widget->screen_location.size.height
                        - toplevel->bar_height - (2 * bw) - 1;
                content_rect->size.width =widget->screen_location.size.width
                        - (2 * bw) -1;
        } else {
                if (content_rect != &widget->screen_location)
                        SAFE_FREE(content_rect);

                widget->screen_location = ei_rect_zero();
                content_rect = &widget->screen_location;
        }

        widget->content_rect = content_rect;

        // On invalide la nouvelle position
        ei_invalidate_rect(&widget->screen_location);

        widget->content_rect = content_rect;
}





/* Enregistrement de la classe */
void    ei_toplevel_register_class()
{
        ei_widgetclass_t *toplevel_class = NULL;

        // Allocation
        toplevel_class = CALLOC_TYPE(ei_widgetclass_t);
        assert(toplevel_class);

        toplevel_class->allocfunc= toplevel_alloc;
        toplevel_class->drawfunc = toplevel_draw;
        toplevel_class->releasefunc = toplevel_release;
        toplevel_class->setdefaultsfunc = toplevel_setdefaults;
        toplevel_class->geomnotifyfunc = toplevel_geomnotify;
        strcpy(toplevel_class->name, "toplevel");

        toplevel_class->next = NULL;

        ei_widgetclass_register(toplevel_class);
}




/***** Radiobutton ******/
void *radiobutton_alloc()
{
        ei_radiobutton_t *radiobutton = CALLOC_TYPE(ei_radiobutton_t);
        assert(radiobutton);

        return radiobutton;
}

void radiobutton_release(struct ei_widget_t* widget)
{
        if (widget) {
                ei_radiobutton_t *radiobutton = (ei_radiobutton_t*)widget;
                free_rdbtn_ltxt(radiobutton->ltxt);
                free_rdbtn_lrec(radiobutton->lrec);
        }
}

void radiobutton_draw(struct ei_widget_t* widget, ei_surface_t surface,
                ei_surface_t pick_surface, ei_rect_t* clipper)
{
        ei_radiobutton_t *radiobutton = (ei_radiobutton_t*)widget;
        assert(radiobutton);
        assert(clipper);
        assert(pick_surface);
        assert(surface);

        if (surface){
                // lock de la surface
                hw_surface_lock(surface);
                //printf("début du draw\n");
                ei_radiobutton_draw(surface,radiobutton->widget.screen_location,radiobutton, clipper);
                //unlock de la surface
                hw_surface_unlock(surface);
        }

        if (pick_surface){
                /* Dessin de la surface de picking */
                pick_surface_draw(pick_surface, widget, clipper);
        }
}

void radiobutton_setdefaults(struct ei_widget_t* widget)
{
        assert(widget);

        // on commence par effectuer un recast
        ei_radiobutton_t *radiobutton;
        radiobutton = (ei_radiobutton_t*)widget;

        ei_color_t bg_color={0x88,0x88,0x88,255};
        radiobutton->bg_color = bg_color;

        ei_color_t btn_color={0,0,0,255};
        radiobutton->btn_color=btn_color;

        ei_color_t txt_color={0,0,0,255};
        radiobutton->txt_color=txt_color;

        ei_color_t bar_color={255,255,255,255};
        radiobutton->bar_color=bar_color;

        ei_size_t btn_size ={25,25};
        radiobutton->btn_size=btn_size;
        radiobutton->btn_bdw=3;

        int border_width=6;
        radiobutton->border_width=border_width;

        int nb_buttons=7;
        radiobutton->nb_buttons=nb_buttons;
        int nb_radios=5;
        radiobutton->nb_radios=nb_radios;
        char* tab_chaine[radiobutton->nb_radios];
        for (int i=0; i<=nb_radios-1;i++) {
                tab_chaine[i]=NULL;
        }
        tab_chaine[0]="Breizh libra";
        tab_chaine[1]="France Bleu Menhir";
        tab_chaine[2]="Carnac blues";
        tab_chaine[3]="Chouchen vibes";
        char* txt_default="No FM selected";
        radiobutton->txt_default=txt_default;
        ei_linked_rdbtn_txt_t *ltxt=rdbtn_txt_create(tab_chaine,nb_radios);

        radiobutton->ltxt=ltxt;
        radiobutton->font=ei_default_font;
        radiobutton->lrec=rdbtn_rec_create(radiobutton);

        int nb_btn_pl=5;
        radiobutton->nb_btn_pl=nb_btn_pl;

        int nb_lignes=(int)ceil((float)radiobutton->nb_buttons/(float)nb_btn_pl);
        int nb_col=MIN(radiobutton->nb_buttons,nb_btn_pl);
        ei_size_t s;
        int h;
        int w;
        hw_text_compute_size("motdevingtcinqlettresssss",radiobutton->font,&w,&h);
        radiobutton->bar_height=h+6;
        s.width=MAX((2*nb_col-1)*btn_size.width+2*border_width,w);
        s.height=radiobutton->bar_height+(nb_lignes+2)*radiobutton->border_width+nb_lignes*radiobutton->btn_size.height;
        //printf("ceil..%i\n",nb_lignes);
        radiobutton->widget.requested_size=s;
        //button->user_param = NULL;
}

void radiobutton_geomnotify(struct ei_widget_t* widget, ei_rect_t rect)
{
        ei_invalidate_rect(&widget->screen_location);
        ei_rect_t* content_rect = NULL;

        if (    widget->content_rect
                        && (widget->content_rect != &widget->screen_location))
                content_rect = widget->content_rect;

        else
                content_rect = CALLOC_TYPE(ei_rect_t);

        if (content_rect != NULL) {
                if (rect.size.width !=0 && rect.size.height != 0){
                        widget->screen_location = rect;
                        ei_radiobutton_t *radiobutton = (ei_radiobutton_t*)widget;
                        // Gestion des bordures pour le content_rect

                        int bw = radiobutton->border_width;
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
        ei_invalidate_rect(&widget->screen_location);
}

void    ei_radiobutton_register_class()
{
        ei_widgetclass_t *radiobutton_class = NULL;

        // Allocation
        radiobutton_class = CALLOC_TYPE(ei_widgetclass_t);
        assert(radiobutton_class);

        radiobutton_class->allocfunc= radiobutton_alloc;
        radiobutton_class->drawfunc = radiobutton_draw;
        radiobutton_class->releasefunc = radiobutton_release;
        radiobutton_class->setdefaultsfunc = radiobutton_setdefaults;
        radiobutton_class->geomnotifyfunc = radiobutton_geomnotify;
        strcpy(radiobutton_class->name, "radiobutton");
        radiobutton_class->next = NULL;

        ei_widgetclass_register(radiobutton_class);
}
