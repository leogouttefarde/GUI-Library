#ifndef EI_RADIOBUTTON_H
#define EI_RADIOBUTTON_H

#include "ei_widgettypes.h"
#include <stdio.h>
#include <stdlib.h>
void free_rdbtn_lrec(ei_linked_rdbtn_rec_t *lrec);
void free_rdbtn_ltxt(ei_linked_rdbtn_txt_t *ltxt);
void ei_radiobutton_draw(ei_surface_t surface, ei_rect_t location,ei_radiobutton_t *radiobutton, ei_rect_t* clipper);
ei_linked_rdbtn_txt_t* rdbtn_txt_create(char* tab[]);
ei_linked_rdbtn_rec_t* rdbtn_rec_create(ei_radiobutton_t *radiobutton);

void aff_liste(ei_linked_rdbtn_rec_t *rec);
void modify_btn_rel(ei_radiobutton_t *radiobutton,int id);
#endif
