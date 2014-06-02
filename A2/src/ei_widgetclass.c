/**
 *  @file	widgetclass.h
 *  @brief	Definition and registration of widget classes.
 *
 *  \author 
 *  Created by François Bérard on 02.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#include "ei_widgetclass.h"



/**
 * \brief	Returns the string of the name of a class.
 *
 * @param	name		The class name.
 *
 * @return			The string representing the name of the class.
 */
static inline char*	ei_widgetclass_stringname	(ei_widget_tidgetclass_name_t name){
        return (char*)name;
}

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
        if (name == "frame"){
                // Les fonctions liées à la classe frame sont déja declarées
                return NULL;
        }

}


/**
 * \brief	Registers the "frame" widget class in the program. This must be called only
 *		once before widgets of the class "frame" can be created and configured with
 *		\ref ei_frame_configure.
 */
void	ei_frame_register_class (){
        // Declaration des fonctions liées à la classe frame

        // pointeur generique
        void *frame_alloc(){
                ;}
        void frame_release(struct ei_widget_t* widget){
                ;
        }
        void frame_draw(struct ei_widget_t* widget, ei_surface_t surface,
                        ei_surface_t pick_surface, ei_rect_t* clipper){
                ;
        }
        void frame_setdefaults(struct ei_widget_t* widget){
                ;
        }
        void frame_geomnotify(struct ei_widget_t* widget){
                ;
        }

        // Allocation
        ei_widgetclass_t *frame;
        frame->allocfunc= &frame_alloc;
        frame->drawfunc = &frame_draw;
        frame->releasefunc = &frame_release;
        frame->setdefaultsfunc = &frame_setdefaults;
        frame->geomnotifyfunc = &frame_geomnotify;
        frame->name = "frame";
        frame->next = NULL;

        //Definition du type
        typedef struct ei_frame_t {
                ei_widget_t widget;
                ei_relief_t relief;
                ei_font_t font;
                struct {bool txt; 
                        union{ char* txt;
                        uint32_t* img}
                } foreground;

        }
        /**
         * \brief	Registers the "button" widget class in the program. This must be called only
         *		once before widgets of the class "button" can be created and configured with
         *		\ref ei_button_configure.
         */
        void	ei_button_register_class(){
                ;
        }

        /**
         * \brief	Registers the "toplevel" widget class in the program. This must be called only
         *		once before widgets of the class "toplevel" can be created and configured with
         *		\ref ei_toplevel_configure.
         */
        void ei_toplevel_register_class	(){
                ;
        }


        /* Inline function definitions. */

        static inline char* ei_widgetclass_stringname (ei_widgetclass_name_t name)
        {
                return (char*)name;
        }

