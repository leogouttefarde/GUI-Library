
#include "ei_geometrymanager.h"
#include "ei_core.h"
#include "ei_common.h"
#include "ei_linkedlist.h"


static ei_widget_t *ei_root = NULL;
static ei_surface_t ei_root_surface = NULL;
static ei_surface_t ei_picking_surface = NULL;
static ei_linkedlist_t ei_update_rects;
static ei_rect_t *ei_cur_draw_rect = NULL;


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

void ei_init()
{
        ei_event_init();
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

ei_rect_t* ei_get_draw_rect()
{
        return ei_cur_draw_rect;
}

// Draw récursif selon la hiérarchie des widgets
void ei_draw_widget(ei_widget_t *widget){

        if (widget){

                // Le widget courant est a affiché en premier (il sera
                // derriere)
                if (widget->geom_params && widget->geom_params->manager
                                && widget->geom_params->manager->runfunc){
                        widget->geom_params->manager->runfunc(widget);
                }

                // Ses enfants seront devant lui et derriere ses freres
                ei_draw_widget(widget->children_head);

                // Les freres du widget courant sont enfin dessinés
                ei_draw_widget(widget->next_sibling);
        }
}

void ei_draw_rect(ei_rect_t *rect)
{
        ei_widget_t *root = ei_get_root();

        if (root && rect) {
                ei_cur_draw_rect = rect;

                ei_draw_widget(root);

                // Restore default
                ei_cur_draw_rect = NULL;
        }
}

void ei_draw_rects()
{
        ei_linked_elem_t *link = ei_update_rects.head;

        while (link) {
                ei_linked_rect_t *lrect = (ei_linked_rect_t*)link->elem;

                /* If valid linked rectangle, draw it */
                if (lrect)
                        ei_draw_rect(&lrect->rect);

                link = link->next;
        }
}

void ei_invalidate_reset()
{
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

        if (ei_is_rect_inter(rect1, rect2)) {

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
                int r2_right = x2 + w2 - 1;
                int r2_bottom = y2 + h2 - 1;

                int r1_left = x1;
                int r1_top = y1;
                // ATTENTION AU -1
                int r1_right = x1 + w1 - 1;
                int r1_bottom = y1 + h1 - 1;


                int left = MIN(r2_left, r1_left);
                int top = MIN(r2_top, r1_top);

                int right = MAX(r2_right, r1_right);
                int bottom = MAX(r2_bottom, r1_bottom);

                int width = right - left + 1;
                int height = bottom - top + 1;

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
        }

        return fuse;
}

void ei_invalidate_rect(ei_rect_t* rect)
{
        if (rect) {

                ei_rect_t new_rect = *rect;

                ei_linked_elem_t *link = ei_update_rects.head, *next = NULL;
                ei_bool_t add = true;
                ei_rect_t *fusion = NULL;

                /* Check current rectangles for optimization */
                while (link) {
                        ei_linked_rect_t *lrect = (ei_linked_rect_t*)link->elem;
                        next = link->next;

                        if (lrect) {

                                /* If duplicate found, do not add */
                                if (lrect->rect.top_left.x == rect->top_left.x
                                        && lrect->rect.top_left.y == rect->top_left.y
                                        && lrect->rect.size.width == rect->size.width
                                        && lrect->rect.size.height == rect->size.height)
                                        add = false;

                                /* Fuse with another if better */
                                else if ( (fusion = ei_smaller_fused(&lrect->rect, rect)) ) {
                                        ei_linkedlist_pop_link(&ei_update_rects, link, true);
                                        new_rect = *fusion;
                                        SAFE_FREE(fusion);
                                }
                        }

                        link = next;
                }


                /* Program a new rectangle for invalidation */
                if (add) {
                        ei_linked_rect_t *new_link = CALLOC_TYPE(ei_linked_rect_t);

                        if (new_link) {
                                new_link->rect = new_rect;
                                new_link->next = NULL;

                                /* Keep internal ei_linked_rect_t list valid */
                                ei_linked_elem_t *tail = ei_update_rects.tail;
                                if (tail) {
                                        ei_linked_rect_t *elem = (ei_linked_rect_t*)tail->elem;

                                        if (elem)
                                                elem->next = new_link;
                                }

                                ei_linkedlist_add(&ei_update_rects, new_link);
                        }
                }
        }
}

