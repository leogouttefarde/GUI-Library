
#include "ei_radiobutton.h"
#include "ei_common.h"
#include "ei_core.h"
#include "ei_utilities.h"
#include "ei_widgetclass_pv.h"
#include "ei_button.h"

void free_rdbtn_ltxt(ei_linked_rdbtn_txt_t *ltxt)
{
        ei_linked_rdbtn_txt_t* Suivant;
        while (ltxt != NULL) {
                Suivant = ltxt->next;
                SAFE_FREE(ltxt);
                ltxt = Suivant;
        }
}

void free_rdbtn_lrec(ei_linked_rdbtn_rec_t *lrec)
{
        ei_linked_rdbtn_rec_t* Suivant;
        while (lrec != NULL) {
                Suivant = lrec->next;
                SAFE_FREE(lrec);
                lrec = Suivant;
        }
}

void ei_radiobutton_draw(ei_surface_t surface, ei_rect_t location,ei_radiobutton_t *radiobutton, ei_rect_t* clipper)
{
        /* Mise à jour*/
        radiobutton->lrec = place_rdbtn_rec(radiobutton);
        ei_rect_t *inter = NULL;

        /* Dessin du fond */
        ei_rect_t fond = location;
        int rel = radiobutton->border_width*0.5;
        ei_button_draw_loc(surface,fond,radiobutton->bg_color,ei_relief_raised,0,rel,clipper);

        /*Champ de texte*/
        ei_rect_t bar;
        bar.size.height = radiobutton->bar_height;
        bar.size.width = location.size.width-2*radiobutton->border_width;
        bar.top_left = plus(location.top_left,radiobutton->border_width,radiobutton->border_width);
        inter = ei_rect_intersection(clipper,&bar);
        if (inter) ei_fill(surface,&radiobutton->bar_color,inter);

        /*Dessin des boutons*/
        ei_linked_rdbtn_rec_t* lrec = radiobutton->lrec;
        ei_linked_rdbtn_txt_t* ltxt = radiobutton->ltxt;
        while (lrec!=NULL) {
                ei_button_draw_loc(surface,lrec->rec,radiobutton->btn_color,lrec->rel,0,radiobutton->btn_bdw,clipper);
                if (lrec->rel == ei_relief_sunken) {
                        if (ltxt && ltxt->txt != NULL) {
                                assert(ltxt->txt);
                                if (inter) ei_insert_text(surface,bar,ltxt->txt,radiobutton->font,radiobutton->txt_color,ei_anc_west,inter);
                        }
                        else
                                ei_insert_text(surface,bar,radiobutton->txt_default,radiobutton->font,radiobutton->txt_color,ei_anc_west,inter);
                }
                lrec = lrec->next;
                if (ltxt) {
                        ltxt = ltxt->next;
                }
                else ltxt = NULL;
        }

        SAFE_FREE(inter);
}

ei_linked_rdbtn_txt_t* rdbtn_txt_create(char* tab[],int taille)
{
        ei_linked_rdbtn_txt_t *suivant = NULL;
        ei_linked_rdbtn_txt_t* ltxt = NULL;
        for (int i=taille-1; i>=0; i--) {
                ltxt = CALLOC_TYPE(ei_linked_rdbtn_txt_t);
                ltxt->txt = tab[i];
                ltxt->next = suivant;
                suivant = ltxt;
        }
        return ltxt;    
}

/**
 *  @brief Updates the linked list of text of a radiobutton
 *
 *  @param tab The tab which contains the new names of radio
 *  @param radiobutton The principal widget
 *
 */
static void rdbtn_txt_maj(char** tab[],ei_radiobutton_t *radiobutton)
{
        ei_linked_rdbtn_txt_t* ltxt = radiobutton->ltxt;
        char* nv_txt[radiobutton->nb_radios];
        for (int i=0; i<=radiobutton->nb_radios-1; i++) {
                if (tab[i]) nv_txt[i] = *tab[i];
                else {
                        if (ltxt) nv_txt[i] = ltxt->txt;
                        else nv_txt[i] = NULL;
                }
                if (ltxt) ltxt = ltxt->next;
                else ltxt = NULL;
        }
        free_rdbtn_ltxt(radiobutton->ltxt);
        radiobutton->ltxt = rdbtn_txt_create(nv_txt,radiobutton->nb_radios);
}       

ei_linked_rdbtn_rec_t* rdbtn_rec_create(ei_radiobutton_t *radiobutton)
{
        ei_linked_rdbtn_rec_t* rdbtn = NULL;
        ei_linked_rdbtn_rec_t* suivant = NULL;
        for (int i=1; i<=radiobutton->nb_buttons; i++) {
                rdbtn = CALLOC_TYPE(ei_linked_rdbtn_rec_t);
                rdbtn->rel = ei_relief_raised;
                rdbtn->next = suivant;
                if (suivant) rdbtn->next->prev = rdbtn;
                suivant = rdbtn;
        }
        rdbtn->prev = NULL;
        return rdbtn;
}

ei_linked_rdbtn_rec_t* place_rdbtn_rec(ei_radiobutton_t *radiobutton)
{
        ei_rect_t location = radiobutton->widget.screen_location;
        ei_linked_rdbtn_rec_t* rdbtn = radiobutton->lrec;
        int nb_btn_pl = radiobutton->nb_btn_pl;

        while (rdbtn) {
                if (rdbtn->next == NULL) break;
                rdbtn = rdbtn->next;
        }

        ei_rect_t position;
        int n = MIN(radiobutton->nb_buttons,nb_btn_pl);
        int ecart;
        ecart = (location.size.width-2*radiobutton->border_width-n*radiobutton->btn_size.width)/(n+1);
        for (int j=(int)ceil((float)radiobutton->nb_buttons/(float)nb_btn_pl);j>=1; j--) {
                for (int i=MIN(nb_btn_pl,radiobutton->nb_buttons-nb_btn_pl*(j-1));i>=1; i--) {
                        position.top_left.y = location.top_left.y+radiobutton->bar_height+(j+1)*radiobutton->border_width+(j-1)*radiobutton->btn_size.height;
                        position.top_left.x = location.top_left.x+radiobutton->border_width+(i-1)*radiobutton->btn_size.width+i*ecart;
                        position.size = radiobutton->btn_size;
                        rdbtn->rec = position;
                        if (rdbtn->prev) rdbtn = rdbtn->prev;
                }
        }
        return rdbtn;
}

void aff_liste(ei_linked_rdbtn_rec_t *lrec)
{
        ei_rect_t position;
        ei_linked_rdbtn_rec_t *lrec2 = lrec;
        printf("Rectangles chainés next, top_left:  ");
        while (lrec) {
                position = lrec->rec;
                printf("{%i,%i}->",position.top_left.x,position.top_left.y);

                if (lrec->next==NULL) break;
                lrec = lrec->next;
        }
        printf("\n");
        printf("Rectangles chainés prev, top_left:  ");
        while (lrec) {
                position = lrec->rec;
                printf("{%i,%i}->",position.top_left.x,position.top_left.y);
                lrec = lrec->prev;
        }
        printf("\n");
        printf("Rectangles chainés, relief:  ");
        while (lrec2) {
                position = lrec2->rec;
                printf("%i->",lrec2->rel);
                lrec2 = lrec2->next;
        }
        printf("\n");

}

void modify_btn_rel(ei_radiobutton_t *radiobutton, int id)
{
        ei_linked_rdbtn_rec_t *lrec = radiobutton->lrec;
        int indice = 1;

        while (lrec) {
                if (indice != id)
                        lrec->rel = ei_relief_raised;
                else
                        lrec->rel = ei_relief_sunken;

                indice ++;

                if (lrec->next == NULL) break;
                lrec = lrec->next;
        }

        while (lrec) {
                if (lrec->prev == NULL) break;
                lrec = lrec->prev;
        }

        radiobutton->lrec = lrec;
}

void ei_radiobutton_configure (ei_widget_t *widget,
                int *nb_buttons,
                int *nb_btn_pl,
                ei_size_t *btn_size,
                const ei_color_t *bg_color,
                const ei_color_t *txt_color,
                const ei_color_t *btn_color,
                int *nb_radios,
                char **tab[],
                ei_font_t *font)
{
        if (ei_has_widgetclass(widget,"radiobutton")) {
                ei_radiobutton_t *radiobutton = (ei_radiobutton_t*)widget;
                ei_size_t s = radiobutton->widget.requested_size;
                if (nb_radios) radiobutton->nb_radios = *nb_radios;
                else if (tab) printf("Please, renseign the size of the array");
                if (tab) rdbtn_txt_maj(tab,radiobutton);
                if (nb_buttons) {
                        radiobutton->nb_buttons = *nb_buttons;
                        free_rdbtn_lrec(radiobutton->lrec);
                        radiobutton->lrec = rdbtn_rec_create(radiobutton);
                }
                if (nb_btn_pl) radiobutton->nb_btn_pl = *nb_btn_pl;
                if (bg_color) radiobutton->bg_color = *bg_color;
                if (txt_color) radiobutton->txt_color = *txt_color;
                if (btn_color) radiobutton->btn_color = *btn_color;
                if (btn_size) radiobutton->btn_size = *btn_size;
                int h,w;
                if (font) {
                        radiobutton->font = *font;
                        hw_text_compute_size("motdevingtcinqlettresssss",radiobutton->font,&w,&h);
                        radiobutton->bar_height = h+6;
                }
                else hw_text_compute_size("motdevingtcinqlettresssss",ei_default_font,&w,&h);
                int nb_btn_pl = radiobutton->nb_btn_pl;
                int nb_lignes = (int)ceil((float)radiobutton->nb_buttons/(float)nb_btn_pl);
                int nb_col = MIN(radiobutton->nb_buttons,nb_btn_pl);
                s.width = MAX((2*nb_col-1)*radiobutton->btn_size.width+2*radiobutton->border_width,w);
                s.height = radiobutton->bar_height+(nb_lignes+2)*radiobutton->border_width+nb_lignes*radiobutton->btn_size.height;
                radiobutton->widget.requested_size = s;
        }
}       

