/**
 *  @file       ei_entry.h
 *  @brief      Entry typing fields definition.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 15.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#ifndef EI_ENTRY_H
#define EI_ENTRY_H

#include "ei_types.h"
#include "ei_widgettypes.h"


/**
 * \brief       An entry field structure.
 */
typedef struct ei_entry_t {
        ei_widget_t widget;
        ei_bool_t top_entry;
        struct ei_entry_t* next_entry;
        char* txt;
        ei_font_t font;
        int border_width;
} ei_entry_t;


/**
 * \brief       Initializes an entry widget and all its descendants.
 *
 * @param       widget  The widget to initialize.
 */
void reinit_top_entry(ei_widget_t *widget);

/**
 * \brief       Finds the focused entry from a widget's hierarchy.
 *
 * @param       widget  The widget to search from.
 *
 * @return              The resulting widget.
 */
ei_widget_t* find_top_entry(ei_widget_t *widget);

/**
 * \brief       Draws an entry widget.
 *
 * @param       surface         The surface to draw into
 * @param       rectangle       The entry's rectangle
 * @param       entry           The entry
 * @param       clipper         The clipper
 */
void ei_entry_draw(ei_surface_t surface, ei_rect_t rectangle, ei_entry_t *entry, ei_rect_t* clipper);

/**
 * \brief       Groups several entries together (browsable using tabs).
 *
 * @param       tab_entry       The entry table
 * @param       nb_entry        The number of entries
 */
void link_entry(ei_widget_t *tab_entry[], int nb_entry);


#endif
