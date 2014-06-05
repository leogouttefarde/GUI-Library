#include "ei_global.h"

static bool is_init;
static ei_widget_t *ei_root = NULL;
static ei_surface_t *ei_root_surface = NULL;// = malloc(sizeof(ei_surface_t));
static ei_surface_t *ei_picking_surface = NULL;// = malloc(sizeof(ei_surface_t));

void init(){
        ei_root = malloc(sizeof(ei_widget_t));
        ei_root_surface = malloc(sizeof(ei_surface_t));
        ei_picking_surface = malloc(sizeof(ei_surface_t));
}

// Setters
// Au premier appel les 3 pointeurs sont alloués
void ei_set_root(ei_widget_t *root){
        if (!is_init)
                init();        
        ei_root = root;
}

void ei_set_root_surface(ei_surface_t root_surface){
        if (!is_init)
                init();
        ei_root_surface = root_surface;
}

void ei_set_picking_surface(ei_surface_t picking_surface){
        if (!is_init)
                init();
        ei_picking_surface = picking_surface;
}


/**
 * \brief	Returns the "root widget" of the application: a "frame" widget that encapsulate the
 *		root window.
 *
 * @return 			The root widget.
 */
ei_widget_t* ei_get_root(){
        return ei_root;
}

/**
 * \brief	Returns the surface of the root window.
 *
 * @return 			The surface of the root window.
 */
ei_surface_t ei_get_root_surface(){
        return *ei_root_surface;
}

ei_surface_t ei_get_picking_surface(){
        return *ei_picking_surface;
}


ei_surface_t* ei_get_root_surface_pointer(){
        return ei_root_surface;
}

ei_surface_t* ei_get_picking_surface_pointer(){
        return ei_picking_surface;
}
