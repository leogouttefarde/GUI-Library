/**
 *  @file       ei_widgettypes.c
 *  @brief      Manages the update rectangles
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 05.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#ifndef EI_WIDGETTYPES_H
#define EI_WIDGETTYPES_H

#include "ei_geometrymanager.h"
#include "ei_widget.h"



/**
 * @brief Parameters describing a frame
 */
typedef struct ei_frame_t {
        ei_widget_t widget;
        ei_color_t bg_color;
        int border_width; 
        ei_relief_t relief;
        char* text;
        ei_font_t text_font;
        ei_color_t text_color;
        ei_anchor_t text_anchor;
        ei_surface_t img;
        ei_rect_t* img_rect;
        ei_anchor_t img_anchor;
} ei_frame_t;


/**
 * @brief Parameters describing a button
 */
typedef struct ei_button_t {
        ei_widget_t widget;
        ei_color_t      *color;
        int             border_width;
        int             corner_radius;
        ei_relief_t     relief;
        char*           text;
        ei_font_t       text_font;
        ei_color_t      text_color;
        ei_anchor_t     text_anchor;
        ei_surface_t    img;
        ei_rect_t*      img_rect;
        ei_anchor_t     img_anchor;
        ei_callback_t   callback;
        void *  user_param;
} ei_button_t;

/**
 * @brief Parameters describing a toplevel
 */
typedef struct ei_toplevel_t {
        ei_widget_t widget;
        ei_color_t      color;

        //Barre de titre
        int bar_height;
        int             border_width;
        ei_color_t bar_color;

        //titre
        char*           title;
        ei_font_t       title_font;
        ei_color_t title_color;

        //bouton close
        ei_bool_t       closable;
        ei_relief_t rel_btn_close;

        //bouton resize
        ei_axis_set_t   resizable;
        int             resize_size;
        //
        ei_size_t*      min_size;

        // Pour le calcul du deplacement
        ei_point_t move_pos;

        // Pour detecter une demande de fermeture
        ei_bool_t   close;
} ei_toplevel_t;


typedef struct ei_linked_rdbtn_txt_t {
        char* txt;
        struct ei_linked_rdbtn_txt_t *next;
} ei_linked_rdbtn_txt_t;


typedef struct ei_linked_rdbtn_rec_t {
        ei_rect_t rec;
        ei_relief_t rel;
        struct ei_linked_rdbtn_rec_t *next;
        struct ei_linked_rdbtn_rec_t *prev;
} ei_linked_rdbtn_rec_t;


typedef struct ei_radiobutton_t {
        ei_widget_t widget;
        int nb_buttons;
        int nb_btn_pl;
        ei_color_t bg_color;
        ei_color_t btn_color;
        ei_font_t font;
        ei_color_t txt_color;
        char* txt_default;
        int bar_height;
        ei_color_t bar_color;
        ei_size_t btn_size;
        int border_width;
        int btn_bdw;
        ei_linked_rdbtn_txt_t *ltxt;
        int nb_radios;
        ei_linked_rdbtn_rec_t *lrec;
        void *  user_param;
} ei_radiobutton_t;


#endif
