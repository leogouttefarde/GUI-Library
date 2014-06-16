/**
 *  @file       ei_radiobutton.h
 *  @brief      Radiobutton functions.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 13.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 */

#ifndef EI_RADIOBUTTON_H
#define EI_RADIOBUTTON_H

#include "ei_widgettypes.h"


/**
 * @brief       Radiobutton linked string.
 */
typedef struct ei_linked_rdbtn_txt_t {
        char* txt;
        struct ei_linked_rdbtn_txt_t *next;
} ei_linked_rdbtn_txt_t;

/**
 * @brief       Radiobutton linked rectangle.
 */
typedef struct ei_linked_rdbtn_rec_t {
        ei_rect_t rec;
        ei_relief_t rel;
        struct ei_linked_rdbtn_rec_t *next;
        struct ei_linked_rdbtn_rec_t *prev;
} ei_linked_rdbtn_rec_t;

/**
 * @brief       A radiobutton.
 */
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


/**
 *  @brief free a linked list of the buttons of the radiobutton
 *
 *  @param The list to be freed
 *
 */
void free_rdbtn_lrec(ei_linked_rdbtn_rec_t *lrec);

/**
 *  @brief free a linked list of char*
 *
 *  @param The list to be freed
 *
 */
void free_rdbtn_ltxt(ei_linked_rdbtn_txt_t *ltxt);

/**
 *  @brief draw a radiobutton
 */
void ei_radiobutton_draw(ei_surface_t surface, ei_rect_t location,ei_radiobutton_t *radiobutton, ei_rect_t *clipper);

/**
 *  @brief Creates the linked list of text used by radiobutton
 *
 *  @param tab The array which contains the string name of the radios
 *  @param taille The size of the array
 *
 *  @return the head of the linked list
 */
ei_linked_rdbtn_txt_t* rdbtn_txt_create(char* tab[], int taille);

/**
 *  @brief Creates an empty linked list of radiobutton rectangle, relief is put to raised
 *
 *  @param radiobutton The principal widget
 *
 *  @return The head of the linked list of rectangles
 */
ei_linked_rdbtn_rec_t* rdbtn_rec_create(ei_radiobutton_t *radiobutton);

/**
 *  @brief Modify the position of the button of the widget radiobutton according to its screen location and the buttons' size
 *
 *  @param radiobutton The principal widget
 *
 *  @return The head of the linked list of the modified rectangles
 */
ei_linked_rdbtn_rec_t* place_rdbtn_rec(ei_radiobutton_t *radiobutton);

/**
 *  @brief Display the content of a linked list of rect
 *
 *  @param lrec The list to be displayed
 *
 */
void aff_liste(ei_linked_rdbtn_rec_t *rec);

/**
 * @brief Called in radiobuttion callback, it changes the id buttons relief to sunk and put the others to raised
 *
 * @param radiobutton The radiobutton to be modified
 * @param id The place in the list of the element to sunk
 *
 */
void modify_btn_rel(ei_radiobutton_t *radiobutton, int id);

/**
 * @brief Configure a radiobutton, the size is based on the size of the buttons, and the size of the text, it will always be able to display 25 characters, to reduce the radio, reduce the font and the size of the buttons
 *
 * @param nb_buttons The number of buttons
 * @param nb_btn_pl The number of buttons per line
 * @param btn_size The size of the buttons
 * @param bg_color The color of the background of the radio
 * @param txt_color The color in which the name of the radios will be displayed
 * @param btn_color The color of the buttons
 * @param nb_radios The number of the radios, which have to be equal to the size of the folowing array
 * @param tab An array which contains the names of the radios, its size can be lower or greater than nb_buttons
 * @param font The font to display the text
 */
void ei_radiobutton_configure (ei_widget_t *widget,
                int *nb_buttons,
                int *nb_btn_pl,
                ei_size_t *btn_size,
                const ei_color_t* bg_color,
                const ei_color_t* txt_color,
                const ei_color_t* btn_color,
                int *nb_radios,
                char **tab[],
                ei_font_t *font);


#endif
