/**
 *  @file       ei_tag.h
 *  @brief      Tag handling API.
 *              Tags possess a linked list of widgets linked to them,
 *              and are themselves managed by a linked list of all tags.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 14.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#ifndef EI_TAG_H
#define EI_TAG_H

#include "ei_common.h"
#include "ei_linkedlist.h"
#include "ei_widget.h"
#include "ei_event.h"


/**
 * \brief       Tag structure, containing its name and associated widgets.
 */
typedef struct ei_tag_widgets_t {
        ei_tag_t name;
        ei_linkedlist_t widgets;
} ei_tag_widgets_t;

/**
 * \brief       Tag parameters structure,
 *              used to communicate with list callback functions.
 */
typedef struct ei_tag_params_t {
        ei_widget_t *widget;
        ei_tag_t tag_name;
        ei_bool_t result;
} ei_tag_params_t;


/**
 * \brief       Creates a tag.
 *
 * @param       tag_name        The name of the tag to create.
 */
void ei_tag_create(ei_tag_t tag_name);

/**
 * \brief       Destroys a tag.
 *
 * @param       tag_name        The name of the tag to destroy.
 */
void ei_tag_destroy(ei_tag_t tag_name);

/**
 * \brief       Tags a widget with a tag.
 *
 * @param       widget          The widget to tag.
 * @param       tag_name        The tag to add to this widget.
 */
void ei_tag(ei_widget_t *widget, ei_tag_t tag_name);

/**
 * \brief       Untags a widget from a tag.
 *
 * @param       widget          The widget to untag.
 * @param       tag_name        The tag to remove from this widget.
 */
void ei_untag(ei_widget_t *widget, ei_tag_t tag_name);

/**
 * \brief       Indicates if a widget is tagged by a given tag.
 *
 * @param       widget          The widget.
 * @param       tag_name        The tag's name.
 *
 * @return              The resulting boolean.
 */
ei_bool_t ei_has_tag(ei_widget_t *widget, ei_tag_t tag_name);

/**
 * \brief       Frees all tags.
 */
void ei_tag_free();


#endif
