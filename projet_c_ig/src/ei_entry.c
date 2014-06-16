
#include "ei_entry.h"
#include "ei_widgetclass_pv.h"
#include "ei_utilities.h"
#include "ei_utils.h"
#include "ei_core.h"
#include "ei_button.h"


void reinit_top_entry(ei_widget_t *widget)
{
        if (widget) {
                if (ei_has_widgetclass(widget, "entry")) {
                        ei_entry_t *entry = (ei_entry_t*)widget;
                        entry->top_entry = EI_FALSE;
                        ei_invalidate_rect(&widget->screen_location);
                }

                reinit_top_entry(widget->children_head);
                reinit_top_entry(widget->next_sibling);
        }
}

ei_widget_t *find_top_entry(ei_widget_t *widget)
{
        ei_widget_t *res = NULL;

        if (widget) {

                if (ei_has_widgetclass(widget, "entry")) {
                        ei_entry_t *entry = (ei_entry_t*)widget;

                        if (entry->top_entry)
                                res = widget;
                }

                if (res == NULL) {
                        res = find_top_entry(widget->children_head);

                        if (res == NULL)
                                res = find_top_entry(widget->next_sibling);
                }
        }

        return res;
}

void ei_entry_draw(ei_surface_t surface, ei_rect_t rectangle, ei_entry_t *entry, ei_rect_t* clipper)
{
        /* Dessin de la bordure */
        ei_color_t red = { 0, 0, 255, 200 };
        ei_color_t black = { 0, 0,0, 200 };

        if (entry->top_entry)
                ei_draw_rectangle(surface, &red, &rectangle, clipper);
        else
                ei_draw_rectangle(surface, &black, &rectangle, clipper);

        /* Dessin de la barre de saisie */ 
        ei_color_t blanc = { 255, 255, 255, 255 };
        ei_rect_t rec_txt = reduction(rectangle,entry->border_width);
        ei_draw_rectangle(surface, &blanc, &rec_txt, clipper);

        /* Affichage du texte */
        ei_rect_t *inter = ei_rect_intersection(clipper, &rec_txt);

        char *text = entry->txt;
        ei_bool_t is_alloc = EI_FALSE;

        if (text) {
                const uint32_t size = strlen(text);

                if (size > 0) {
                        is_alloc = EI_TRUE;
                        text = CALLOC(1, size + 2);

                        strcpy(text, entry->txt);
                        text[ size ] = '|';
                        text[ size + 1 ] = '\0';
                }
        }

        if (entry->top_entry) {
                if (entry->txt) {
                        ei_insert_text(surface, rec_txt, text, entry->font, black, ei_anc_west, inter);
                } else {
                        ei_insert_text(surface, rec_txt, "|", entry->font, black, ei_anc_west, inter);
                }
        } else { 
                if (entry->txt) {
                        ei_insert_text(surface, rec_txt, text, entry->font, black, ei_anc_west, inter);
                } else {
                        ei_insert_text(surface, rec_txt, "Veuillez ecrire ici", entry->font, black, ei_anc_west, inter);
                }
        }

        if (is_alloc)
                SAFE_FREE(text);

        SAFE_FREE(inter);
}

void link_entry(ei_widget_t *tab_entry[], int nb_entry)
{
        ei_entry_t *entry = NULL;
        ei_entry_t *entry_next = NULL;

        for (int i = 0; i <= nb_entry - 1; i++) {

                if (ei_has_widgetclass(tab_entry[i], "entry")) {

                        entry = (ei_entry_t*)tab_entry[i];

                        if (i != nb_entry - 1) {
                                entry_next = (ei_entry_t*)tab_entry[i + 1];

                                if (ei_has_widgetclass(tab_entry[i + 1], "entry"))
                                        entry->next_entry = entry_next;
                        }
                        else
                                entry->next_entry = (ei_entry_t*)tab_entry[0];
                }
        }
}

