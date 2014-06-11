
#include "ei_geometrymanager.h"
#include "ei_core.h"
#include "ei_common.h"
#include "ei_linkedlist.h"


static ei_widget_t *ei_root = NULL;
static ei_surface_t ei_root_surface = NULL;
static ei_surface_t ei_picking_surface = NULL;
static ei_linkedlist_t ei_invalid_widgets;
static ei_linkedlist_t ei_update_rects;


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
 * \brief       Returns the "root widget" of the application: a "frame" widget that encapsulate the
 *              root window.
 *
 * @return                      The root widget.
 */
ei_widget_t* ei_get_root(){
        return ei_root;
}

/**
 * \brief       Returns the surface of the root window.
 *
 * @return                      The surface of the root window.
 */
ei_surface_t ei_get_root_surface(){
        return ei_root_surface;
}

ei_surface_t ei_get_picking_surface(){
        return ei_picking_surface;
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
        bool first = true; 
        uint32_t prec_id;
        uint32_t pick_id;
        printf("********** Affichage de la surface **********\n");
        while (x< size.width && y < size.height){
                // on recupere les indices correspondants à l'encodage de la surface
                ei_color_t *color;
                color = CALLOC_TYPE(ei_color_t);
                int ir;
                int ig;
                int ib;
                int ia;
                hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
                color->red = *(addr+ir*sizeof(uint8_t));
                color->green = *(addr+ig*sizeof(uint8_t));
                color->blue = *(addr+ib*sizeof(uint8_t));
                color->alpha = *(addr+ia*sizeof(uint8_t));
                // on génére le le code correspondant
                pick_id = ei_map_rgba(ei_picking_surface, color);

                // Affichage
                if (first) {
                        first = false;
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
        debug_display_surface(ei_picking_surface);
}


void debug_display_root_surface(){
        debug_display_surface(ei_root_surface);
}

void ei_init()
{
        ei_event_init();
        ei_linkedlist_init(&ei_invalid_widgets);
        ei_linkedlist_init(&ei_update_rects);
}

ei_bool_t ei_is_widget_child(ei_widget_t *widget, ei_widget_t *child)
{
        if (widget) {
                ei_widget_t *cur = widget->children_head;

                while (cur) {
                        if (cur == widget)
                                return EI_TRUE;


                        if (cur->next_sibling)
                                cur = cur->next_sibling;
                        else
                                cur = cur->children_head;
                }
        }

        return EI_FALSE;
}

void ei_invalidate_widget(ei_widget_t *widget)
{
        ei_linked_elem_t *link = ei_invalid_widgets.head;
        ei_bool_t add = EI_TRUE;

        while (link && add) {
                if ((widget == link->elem) || ei_is_widget_child(link->elem, widget))
                        add = EI_FALSE;

                link = link->next;
        }

        if (add)
                ei_linkedlist_add(&ei_invalid_widgets, widget);
}

// Draw récursif selon la hiérarchie des widgets
void ei_draw_widget(ei_widget_t *widget, ei_bool_t sibling){

        if (widget){

                // Le widget courant est a affiché en premier (il sera
                // derriere)
                if (widget->geom_params && widget->geom_params->manager
                                && widget->geom_params->manager->runfunc){
                        widget->geom_params->manager->runfunc(widget);
                }

                // Ses enfants seront devant lui et derriere ses freres
                ei_draw_widget(widget->children_head, true);

                if (sibling) {
                        // Les freres du widget courant sont enfin dessinés
                        ei_draw_widget(widget->next_sibling, true);
                }
        }
}

/*
void ei_draw_widgets()
{
        ei_linked_elem_t *link = ei_invalid_widgets.head;
        ei_widget_t *widget = NULL;

        while (link) {
                ei_widget_t *widget = (ei_widget_t*)link->elem;

                ei_draw_widget(widget);

                link = link->next;
        }
}*/

void ei_draw_rect(ei_rect_t *rect)
{
        ei_widget_t *root = ei_get_root();

        if (root && rect) {
                root->content_rect = rect;

        //print_rect(rect);
                ei_draw_widget(root, false);

                // Restore default
                root->content_rect = &root->screen_location;
        }
}

void ei_draw_rects()
{
        // TODO : if this rect is hidden by another invalid one, do not add

        // TODO : Optimisation
        // Fusionner les rectangles dont l'intersection est trop grande


        ei_linked_elem_t *link = ei_update_rects.head;
        //printf("ei_draw_rects\n");

        while (link) {
                ei_linked_rect_t *lrect = (ei_linked_rect_t*)link->elem;

        //printf("lrect %x", lrect);
                if (lrect)
                        ei_draw_rect(&lrect->rect);

                link = link->next;
        }

        //printf("ei_draw_rects END\n");
}

void ei_invalidate_rects()
{
        // ei_linked_elem_t *link = ei_invalid_widgets.head;
        // ei_rect_t *clipper = NULL;
        // ei_widget_t *widget = NULL;

        // while (link) {
        //         widget = (ei_widget_t*)(link->elem);

        //         if (widget) {
        //                 clipper = &(widget->screen_location);
        //                 ei_invalidate_rect(clipper);
        //         }

        //         link = link->next;
        // }
}

void ei_invalidate_reset()
{
        ei_linkedlist_empty(&ei_invalid_widgets, false);
        ei_linkedlist_empty(&ei_update_rects, true);
}

ei_linked_rect_t* ei_get_update_rects()
{
        ei_linked_rect_t *result = NULL;
        ei_linked_elem_t *head = ei_update_rects.head;

        if (head)
                result = (ei_linked_rect_t*)head->elem;

        return result;
}

ei_rect_t* ei_smaller_fused(const ei_rect_t *rect1, const ei_rect_t *rect2)
{
        ei_rect_t *fuse = NULL;

        ei_rect_t *inter = rect_intersection(rect1, rect2);

        // Ne test que si inter, faster
        // if (is_inter()) {
        // if (inter) {
        //         SAFE_FREE(inter);

                int x1 = rect1->top_left.x;
                int y1 = rect1->top_left.y;

                int w1 = rect1->size.width;
                int h1 = rect1->size.height;


                int x2 = rect2->top_left.x;
                int y2 = rect2->top_left.y;

                int w2 = rect2->size.width;
                int h2 = rect2->size.height;

                int r2_left = x2;
                int r2_top = y2;
                int r2_right = x2 + w2;
                int r2_bottom = y2 + h2;

                int r1_left = x1;
                int r1_top = y1;
                int r1_right = x1 + w1;
                int r1_bottom = y1 + h1;


                int left = MIN(r2_left, r1_left);
                int top = MIN(r2_top, r1_top);

                int right = MAX(r2_right, r1_right);
                int bottom = MAX(r2_bottom, r1_bottom);

                int width = right - left;
                int height = bottom - top;

                long long rect1_area = rect1->size.width * rect1->size.height;
                long long rect2_area = rect2->size.width * rect2->size.height;

                long long current_area = rect1_area + rect2_area;

                long long fused_area = width * height;

                if (fused_area < current_area) {
                        fuse = CALLOC_TYPE(ei_rect_t);
                        assert(fuse);


                        fuse->top_left.x = left;
                        fuse->top_left.y = top;

                        fuse->size.width = width;
                        fuse->size.height = height;
                }
        //}

        return fuse;
}

void ei_invalidate_rect(ei_rect_t* rect)
{
        // TODO : if this rect is hidden by another invalid one, do not add

        // TODO : Optimisation
        // Fusionner les rectangles dont l'intersection est trop grande
        if (rect) {

                ei_rect_t new_rect = *rect;

                ei_linked_elem_t *link = ei_update_rects.head, *next = NULL;
                ei_bool_t add = true;

                while (link) {
                        ei_linked_rect_t *lrect = (ei_linked_rect_t*)link->elem;
                        next = link->next;

                        if (lrect) {
                                ei_rect_t *fused = NULL;

                                // Ne pas invalider deux fois le mm rectangle
                                if (lrect->rect.top_left.x == rect->top_left.x
                                        && lrect->rect.top_left.y == rect->top_left.y
                                        && lrect->rect.size.width == rect->size.width
                                        && lrect->rect.size.height == rect->size.height) {
                                        add = false;
                                        //printf("YOOOO\n");
                                        //exit(0);
                                }
                                else if (fused = ei_smaller_fused(&lrect->rect, rect)) {
                                        //printf("AAAAAAAAA\n");

                                        ei_linkedlist_pop_link(&ei_update_rects, link, true);

                                        new_rect = *fused;
                                }
                        }

                        link = next;
                }


                if (add) {
                        ei_linked_rect_t *link = CALLOC_TYPE(ei_linked_rect_t);
                        link->rect = new_rect;
                        link->next = NULL;

                        // Hack optimisation sujet !
                        ei_linked_elem_t *tail = ei_update_rects.tail;
                        if (tail) {
                                ei_linked_rect_t *elem = (ei_linked_rect_t*)tail->elem;

                                if (elem)
                                        elem->next = link;
                        }

                        ei_linkedlist_add(&ei_update_rects, link);
                }
        }
}

