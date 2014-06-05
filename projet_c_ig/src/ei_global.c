#include "ei_global.h"


static ei_widget_t *ei_root;
static ei_surface_t ei_root_surface;
static ei_surface_t ei_picking_surface;


void ei_set_root(ei_widget_t *root){
        ei_root = root;
}

void ei_set_root_surface(ei_surface_t root_surface){
        ei_root_surface = root_surface;
}

void ei_set_picking_surface(ei_surface_t picking_surface){
        ei_picking_surface = picking_surface;
}


ei_widget_t* ei_get_root(){
        return ei_root;
}
ei_surface_t ei_get_root_surface(){
        return ei_root_surface;
}

ei_surface_t ei_get_picking_surface(){
        return ei_picking_surface;
}
