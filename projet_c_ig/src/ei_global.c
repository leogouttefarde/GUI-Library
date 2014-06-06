#include "ei_global.h"
#include <stdio.h>

static ei_widget_t *ei_root = NULL;
static ei_surface_t ei_root_surface = NULL;
static ei_surface_t ei_picking_surface = NULL;

// Setters
// Au premier appel les 3 pointeurs sont alloués
void ei_set_root(ei_widget_t *root){
        ei_root = root;
}

void ei_set_root_surface(ei_surface_t root_surface){
        ei_root_surface = root_surface;
}

void ei_set_picking_surface(ei_surface_t picking_surface){
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
        return ei_root_surface;
}

ei_surface_t ei_get_picking_surface(){
        return ei_picking_surface;
}



void debug_display_picking_surface(){
        hw_surface_lock(ei_picking_surface);
        uint8_t *addr = hw_surface_get_buffer(ei_picking_surface);
        ei_size_t size = hw_surface_get_size(ei_picking_surface);
        int x = 0;
        int y = 0;  
        printf("********** Affichage de la surface de picking **********\n");
        for (; addr < addr + size.width*size.height; addr = addr + 4*sizeof(uint8_t)){
                // on recupere les indices correspondants à l'encodage de la surface
                ei_color_t *color;
                color = malloc(sizeof(ei_color_t));
                int ir;
                int ig;
                int ib;
                int ia;
                hw_surface_get_channel_indices(ei_picking_surface, &ir, &ig, &ib, &ia);
                color->red = *(addr+ir*sizeof(uint8_t));
                color->green = *(addr+ig*sizeof(uint8_t));
                color->blue = *(addr+ib*sizeof(uint8_t));
                color->alpha = *(addr+ia*sizeof(uint8_t));
                // on générele le code correspondant
                uint32_t pick_id = ei_map_rgba(ei_picking_surface, color);

                // Affichage
                printf("Pixel : {%i, %i} Pick_if : %u \n", x,y,pick_id);
                // Increment des coordonnées
                x++;
                if (x == size.width){
                        x=0;
                        y++;
                }



        }


        printf("********** Fin **********\n");
        hw_surface_unlock(ei_picking_surface);

}


