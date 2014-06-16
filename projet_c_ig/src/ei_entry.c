#include "ei_entry.h"
#include "ei_widgetclass_pv.h"
#include "ei_utilities.h"
#include "ei_utils.h"
#include "ei_core.h"
#include "ei_button.h"

void reinit_top_entry(ei_widget_t *widget) {
	if (widget) {
		if (ei_has_widgetclass(widget, "entry")) {
			ei_entry_t *entry=(ei_entry_t*)widget;
			entry->top_entry=EI_FALSE;
			ei_invalidate_rect(&widget->screen_location);
		}
		reinit_top_entry(widget->children_head);
		reinit_top_entry(widget->next_sibling);
	}
}

ei_widget_t *find_top_entry(ei_widget_t *widget) {
	if (widget) {
		ei_widget_t * res = NULL;
		if (ei_has_widgetclass(widget, "entry")) {
			ei_entry_t *entry=(ei_entry_t*)widget;
			if (entry->top_entry) res = widget;
		}
		if (res) return res;
		res = find_top_entry(widget->children_head);
		if (res) return res;
		res = find_top_entry(widget->next_sibling);
		if (res) return res;
		return NULL;
	}
	else return NULL;
}

	

void ei_entry_draw(ei_surface_t surface, ei_rect_t rectangle, ei_entry_t *entry, ei_rect_t* clipper) {
	printf(" on draw la barre \n");
	/*dessin de la bordure*/
		ei_color_t red={0,0,255,200};
		ei_color_t black={0,0,0,200};
	if (entry->top_entry) ei_draw_rectangle(surface,&red,&rectangle,clipper);
	else ei_draw_rectangle(surface,&black,&rectangle,clipper);

	/*dessin de la barre de saisie*/ 
	ei_color_t blanc = {255,255,255,255};
	ei_rect_t rec_txt = reduction(rectangle,entry->border_width);
	ei_draw_rectangle(surface,&blanc,&rec_txt,clipper);

	/* Affichage du texte */
	ei_rect_t *inter=ei_rect_intersection(clipper,&rec_txt);
	if (entry->top_entry) {
		if (entry->txt) {
			printf(" Cas entrytxt nn null et vaut %s \n",entry->txt);
			ei_insert_text(surface, rec_txt, entry->txt, entry->font, black, ei_anc_west, inter);
		} else {
			printf("top mais entry->txt=NULL\n");
			ei_insert_text(surface, rec_txt, "|", entry->font, black, ei_anc_west, inter);
		}
	} else { 
		if (entry->txt) {
			printf("txt nn null mais pas top\n");
			ei_insert_text(surface, rec_txt, entry->txt, entry->font, black, ei_anc_west, inter);
		} else {
			printf("txt null et pas top\n");
		ei_insert_text(surface, rec_txt, "Veuillez ecrire ici", entry->font, black, ei_anc_west, inter);
		}
	}	
}

