/**
 *  @file	widgetclass.h
 *  @brief	Definition and registration of widget classes.
 *
 *  \author 
 *  Created by François Bérard on 02.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

struct ei_widget_t;



/**
 * \brief	Returns the string of the name of a class.
 *
 * @param	name		The class name.
 *
 * @return			The string representing the name of the class.
 */
static inline char*	ei_widgetclass_stringname	(ei_widgetclass_name_t name);

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
        switch(name){
        case frame: return NULL; break;
        case default: return NULL; break;
        }
}


/**
 * \brief	Registers the "frame" widget class in the program. This must be called only
 *		once before widgets of the class "frame" can be created and configured with
 *		\ref ei_frame_configure.
 */
void	ei_frame_register_class (){
        ;
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

