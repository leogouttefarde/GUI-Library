
#include "ei_core.h"
#include "ei_linkedlist.h"
#include "ei_event_pv.h"
#include "ei_root.h"



static ei_linkedlist_t ei_update_rects;


#ifdef LEAK_TRACKER
/**
 * \brief       The current number of allocated (unfreed) blocks : the leak counter.
 *              Debugging feature only, leak tracker non included on release compilation mode.
 */
int ALLOCATION_COUNTER = 0;
#endif


void ei_init()
{
        ei_event_init();
        ei_linkedlist_init(&ei_update_rects);
}

ei_bool_t ei_is_rect_inter(const ei_rect_t *rect1, const ei_rect_t *rect2)
{
        ei_bool_t is_intersection = EI_FALSE;


        if (rect1 && rect2) {
                int x1, y1, x2, y2;
                int w1, h1, w2, h2;
                int rect1_right, rect1_bottom, rect2_right, rect2_bottom;

                x1 = rect1->top_left.x;
                y1 = rect1->top_left.y;

                w1 = rect1->size.width;
                h1 = rect1->size.height;


                x2 = rect2->top_left.x;
                y2 = rect2->top_left.y;

                w2 = rect2->size.width;
                h2 = rect2->size.height;



                rect2_right = x2 + w2 - 1;
                rect2_bottom = y2 + h2 - 1;

                rect1_right = x1 + w1 - 1;
                rect1_bottom = y1 + h1 - 1;


                if ( (x2 < rect1_right)
                                && (rect2_right > x1)
                                && (y2 < rect1_bottom)
                                && (rect2_bottom > y1) )
                        is_intersection = EI_TRUE;
        }

        return is_intersection;
}

ei_rect_t* ei_rect_intersection(const ei_rect_t *rect1, const ei_rect_t *rect2)
{
        ei_rect_t *inter = NULL;


        if (ei_is_rect_inter(rect1, rect2)) {
                int x1, y1, x2, y2;
                int w1, h1, w2, h2;
                int r1_right, r1_bottom, r2_right, r2_bottom;


                x1 = rect1->top_left.x;
                y1 = rect1->top_left.y;

                w1 = rect1->size.width;
                h1 = rect1->size.height;


                x2 = rect2->top_left.x;
                y2 = rect2->top_left.y;

                w2 = rect2->size.width;
                h2 = rect2->size.height;



                r2_right = x2 + w2 - 1;
                r2_bottom = y2 + h2 - 1;

                r1_right = x1 + w1 - 1;
                r1_bottom = y1 + h1 - 1;


                inter = CALLOC_TYPE(ei_rect_t);
                assert(inter);

                if (inter) {
                        int left, top;

                        left = MAX(x1, x2);
                        top = MAX(y1, y2);

                        inter->top_left.x = MAX(x1, x2);
                        inter->top_left.y = MAX(y1, y2);

                        inter->size.width = MIN(r1_right, r2_right) - left + 1;
                        inter->size.height = MIN(r1_bottom, r2_bottom) - top + 1;
                }
        }

        return inter;
}

void ei_draw_widget(ei_widget_t *widget, ei_rect_t *draw_rect)
{
        if (widget){
                /* On calcule le real_clipper du widget */

                if (draw_rect) {
                        ei_rect_t *clipper = NULL;
                        ei_rect_t *real_clipper = NULL;
                        if (widget->parent){
                                // Clipper lié au widget = content_rect parent
                                // INTER screen_location widget
                                clipper = ei_rect_intersection(widget->parent->content_rect, 
                                                &widget->screen_location);

                                // Clipper optimisé = rectangle a mettre a jour
                                // INTER clipper widget
                                //if (widget->parent->content_rect != root->content_rect)
                                real_clipper = ei_rect_intersection(clipper, draw_rect);
                                SAFE_FREE(clipper);
                        }
                        // Pour le root
                        else{
                                clipper = &widget->screen_location;
                                if (clipper) {
                                        real_clipper = ei_rect_intersection(clipper, draw_rect);
                                }
                        }
                        // Si le real_clipper est non vide
                        if (real_clipper) {
                                // Dessin du widget dans le real_clipper
                                widget->wclass->drawfunc(widget, ei_get_root_surface(), ei_get_picking_surface(), real_clipper);
                                SAFE_FREE(real_clipper);
                        }
                }

                // Ses enfants seront devant lui et derriere ses freres
                ei_draw_widget(widget->children_head, draw_rect);

                // Les freres du widget courant sont enfin dessinés
                ei_draw_widget(widget->next_sibling, draw_rect);
        }
}

ei_bool_t ei_draw_rect(ei_linked_elem_t *link, void *user_param)
{
        ei_linked_rect_t *lrect = (ei_linked_rect_t*)link->elem;
        ei_widget_t *root = ei_get_root();

        /* If valid linked rectangle, draw it */
        if (lrect && root) {
                ei_draw_widget(root, &lrect->rect);
        }

        return EI_FALSE;
}

void ei_draw_rects()
{
        ei_linkedlist_applyfunc(&ei_update_rects, ei_draw_rect, NULL);
}

void ei_invalidate_reset()
{
        ei_linkedlist_empty(&ei_update_rects, EI_TRUE);
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

                if (fused_area <= current_area) {
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

void ei_invalidate_rect(ei_rect_t* invalid_rect)
{
        if (invalid_rect) {
                ei_rect_t *rect = NULL;
                ei_rect_t temp;

                /* On commence par intersecter le rectangle avec le root_widget */
                temp =  hw_surface_get_rect(ei_get_root_surface());
                rect = ei_rect_intersection(invalid_rect, &temp);

                if (rect) {
                        /* On ajoute le rectangle */
                        ei_rect_t new_rect = *rect;

                        ei_linked_elem_t *link = ei_update_rects.head, *next = NULL;
                        ei_bool_t add = EI_TRUE;
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
                                                add = EI_FALSE;

                                        /* Fuse with another if better */
                                        else if ( (fusion = ei_smaller_fused(&lrect->rect, rect)) ) {
                                                ei_linkedlist_pop_link(&ei_update_rects, link, EI_TRUE);
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

                        SAFE_FREE(rect);
                }
        }
}

