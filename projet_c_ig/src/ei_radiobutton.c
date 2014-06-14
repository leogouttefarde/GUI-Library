#include "ei_radiobutton.h"
#include <assert.h>
#include "ei_common.h"
#include <math.h>
#include "ei_core.h"
#include "ei_draw.h"
#include "ei_shape.h"
#include "ei_button.h"
#include "ei_utilities.h"

void free_rdbtn_ltxt(ei_linked_rdbtn_txt_t *ltxt) {
	ei_linked_rdbtn_txt_t* Suivant;
	while (ltxt !=NULL) {
		Suivant=ltxt->next;
		SAFE_FREE(ltxt);
		ltxt=Suivant;
	}
}

void free_rdbtn_lrec(ei_linked_rdbtn_rec_t *lrec) {
	ei_linked_rdbtn_rec_t* Suivant;
	while (lrec !=NULL) {
		Suivant=lrec->next;
		SAFE_FREE(lrec);
		lrec=Suivant;
	}
}

void ei_radiobutton_draw(ei_surface_t surface, ei_rect_t location,ei_radiobutton_t *radiobutton, ei_rect_t* clipper) {
	/* Mise à jour*/
	radiobutton->lrec=place_rdbtn_rec(radiobutton);
	printf("place ok\n");
	aff_liste(radiobutton->lrec);
	ei_rect_t *inter;
	/* Dessin du fond */
	ei_rect_t fond=location;
	inter=ei_rect_intersection(clipper,&fond);
	if (inter) ei_fill(surface,&radiobutton->bg_color,inter);
	/*Champ de texte*/
	ei_rect_t bar;
	bar.size.height=radiobutton->bar_height;
	bar.size.width=location.size.width-2*radiobutton->border_width;
	bar.top_left=plus(location.top_left,radiobutton->border_width,radiobutton->border_width);
	inter=ei_rect_intersection(clipper,&bar);
	if (inter) ei_fill(surface,&radiobutton->bar_color,inter);
	/*Dessin des boutons*/
	ei_linked_rdbtn_rec_t* lrec=radiobutton->lrec;
	ei_linked_rdbtn_txt_t* ltxt=radiobutton->ltxt;
	while (lrec!=NULL) {
		ei_button_draw_loc(surface,lrec->rec,radiobutton->btn_color,lrec->rel,0,radiobutton->btn_bdw,clipper);
		if (lrec->rel==ei_relief_sunken) {
			if (ltxt&&ltxt->txt) {
				printf("on va afficher du txt\n");
				if (inter) ei_insert_text(surface,bar,ltxt->txt,radiobutton->font,radiobutton->txt_color,ei_anc_west,inter);
			}
			else 
				ei_insert_text(surface,bar,radiobutton->txt_default,radiobutton->font,radiobutton->txt_color,ei_anc_west,inter);
		}
		lrec=lrec->next;
		if (ltxt) ltxt=ltxt->next;
		else ltxt=NULL;
	}
}

ei_linked_rdbtn_txt_t* rdbtn_txt_create(char* tab[]) {
	int taille=sizeof(tab);
	assert(taille);
	ei_linked_rdbtn_txt_t *suivant=NULL;
	ei_linked_rdbtn_txt_t* ltxt;
	for (int i=taille-1; i>=0; i--) {
		ltxt=CALLOC_TYPE(ei_linked_rdbtn_txt_t);
		ltxt->txt=tab[i];
		ltxt->next=suivant;
		suivant=ltxt;
	}
	return(ltxt);	
}

ei_linked_rdbtn_rec_t* rdbtn_rec_create(ei_radiobutton_t *radiobutton) {
	ei_linked_rdbtn_rec_t* rdbtn;
	ei_linked_rdbtn_rec_t* suivant=NULL;
	ei_rect_t position;
	for (int i=1; i<=radiobutton->nb_buttons; i++) {
			rdbtn=CALLOC_TYPE(ei_linked_rdbtn_rec_t);
			//printf("scrnloc:pt/size{%i,%i},{%i,%i}\n",location.top_left.x,location.top_left.y,location.size.width,location.size.height);
			//printf("postion:pt/size{%i,%i},{%i,%i}\n",position.top_left.x,position.top_left.y,position.size.width,position.size.height);
			rdbtn->rec=position;
			rdbtn->rel=ei_relief_raised;
			rdbtn->next=suivant;
			if (suivant) rdbtn->next->prev=rdbtn;
			suivant=rdbtn;
	}
	rdbtn->prev=NULL;
	return(rdbtn);
}

ei_linked_rdbtn_rec_t* place_rdbtn_rec(ei_radiobutton_t *radiobutton) {
	ei_rect_t location=radiobutton->widget.screen_location;
	ei_linked_rdbtn_rec_t* rdbtn=radiobutton->lrec;
	while (1) {
		if (rdbtn->next==NULL) break;
		rdbtn=rdbtn->next;
	}
	ei_rect_t position;
	for (int j=(int)ceil((float)radiobutton->nb_buttons/4.);j>=1; j--) {
		for (int i=MIN(4,radiobutton->nb_buttons-4*(j-1));i>=1; i--) {
			position.top_left.y=location.top_left.y+radiobutton->bar_height+radiobutton->border_width+radiobutton->btn_size.height+(j-1)*2*radiobutton->btn_size.height;
			position.top_left.x=location.top_left.x+radiobutton->border_width+2*(i-1)*radiobutton->btn_size.width;
			position.size=radiobutton->btn_size;
			rdbtn->rec=position;
			if (rdbtn->prev) rdbtn=rdbtn->prev;
		}
	}
	return(rdbtn);
}

void aff_liste(ei_linked_rdbtn_rec_t *lrec) {
	ei_rect_t position;
	ei_linked_rdbtn_rec_t *lrec2=lrec;
	printf("Rectangles chainés next, top_left:  ");
	while (1) {
			position=lrec->rec;
			printf("{%i,%i}->",position.top_left.x,position.top_left.y);
			//printf("suivant%x\n",lrec->next);
			if (lrec->next==NULL) break;
			lrec=lrec->next;
		}
	printf("\n");
	printf("Rectangles chainés prev, top_left:  ");
	while (lrec!=NULL) {
			position=lrec->rec;
			printf("{%i,%i}->",position.top_left.x,position.top_left.y);
			//printf("suivant%x\n",lrec->next);
			lrec=lrec->prev;
		}
	printf("\n");
printf("Rectangles chainés, relief:  ");
	while (lrec2!=NULL) {
			position=lrec2->rec;
			printf("%i->",lrec2->rel);
			//printf("suivant%x\n",lrec->next);
			lrec2=lrec2->next;
		}
	printf("\n");

}

void modify_btn_rel(ei_radiobutton_t *radiobutton,int id) {
	ei_linked_rdbtn_rec_t *lrec=radiobutton->lrec;
	int indice=1;
	while (1) {
		if (indice!=id) {
			lrec->rel=ei_relief_raised;
			printf(" on raise  %i   ",indice);
		} else {
			lrec->rel=ei_relief_sunken;
			printf(" on sunk  %i   ",indice);
		}
		indice ++;
		if (lrec->next==NULL) break;
		lrec=lrec->next;
	}
	printf("\n");
	while (1) {
		if (lrec->prev==NULL) break;
		lrec=lrec->prev;
	}
	radiobutton->lrec=lrec;
}


