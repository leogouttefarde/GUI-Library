
#include "ei_debug.h"
#include "ei_common.h"
#include "ei_core.h"


void ei_print_color(const ei_color_t *color)
{
        if (color) {
                int red = color->red;
                int green = color->green;
                int blue = color->blue;
                int alpha = color->alpha;

                printf("red %d\t", red);
                printf("green %d\t", green);

                printf("blue %d\t", blue);
                printf("alpha %d\n", alpha);
        }
}


void ei_print_rect(const ei_rect_t *rect)
{
        if (rect) {
                int left = rect->top_left.x;
                int top = rect->top_left.y;

                int right = rect->size.width + left;
                int bottom = rect->size.height + top;

                int w = rect->size.width;
                int h = rect->size.height;


                printf("left %d\t", left);
                printf("right %d\t", right);

                printf("top %d\t", top);
                printf("bottom %d\t", bottom);

                printf("width %d\t", w);
                printf("height %d\n\n", h);
        }
}

void ei_print_rects(ei_linked_rect_t *lrect)
{
        while (lrect) {
                ei_print_rect(&lrect->rect);
                lrect = lrect->next;
        }
}

// Affiche une surface pixel par pixel
// On affiche seulement les pixels dont la couleur
// change par rapport au précédent
void debug_display_surface(ei_surface_t surface){
        hw_surface_lock(surface);
        uint8_t *addr = hw_surface_get_buffer(surface);
        ei_size_t size = hw_surface_get_size(surface);
        int x = 0;
        int y = 0; 
        ei_bool_t first = EI_TRUE; 
        uint32_t prec_id;
        uint32_t pick_id;

        int ir;
        int ig;
        int ib;
        int ia;
        hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);

        printf("********** Affichage de la surface **********\n");
        while (x< size.width && y < size.height){
                // on recupere les indices correspondants à l'encodage de la surface
                ei_color_t color;
                color.red = *(addr+ir*sizeof(uint8_t));
                color.green = *(addr+ig*sizeof(uint8_t));
                color.blue = *(addr+ib*sizeof(uint8_t));
                color.alpha = *(addr+ia*sizeof(uint8_t));
                // on génére le le code correspondant
                pick_id = ei_map_rgba(ei_get_picking_surface(), &color);

                // Affichage
                if (first) {
                        first = EI_FALSE;
                        prec_id = pick_id;
                        printf("Pixel : {%i, %i} Pick_id : %x \n", x,y,pick_id);
                }
                if (prec_id != pick_id){
                        printf("Pixel : {%i, %i} Pick_id : %x \n", x,y,pick_id);
                        prec_id = pick_id;
                }

                // Increment des coordonnées
                x++;
                if (x == size.width){
                        x=0;
                        y++;
                }
                addr = addr + 4*sizeof(uint8_t);
        }


        printf("********** Fin **********\n");
        hw_surface_unlock(surface);

}

void debug_display_picking_surface(){
        debug_display_surface(ei_get_picking_surface());
}


void debug_display_root_surface(){
        debug_display_surface(ei_get_root_surface());
}


