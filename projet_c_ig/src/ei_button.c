/**
 *  @file       ei_button.c
 *  @brief      Button functions
 *
 *  \author 
 *  Created by Antoine Delaite on 04.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#include "ei_button.h"
#include "ei_draw.h"
#include "ei_common.h"
#include "ei_shape.h"
#include "ei_utilities.h"
#include "ei_core.h"


/* Frame draw */
void ei_frame_draw(ei_surface_t window, ei_rect_t rectangle, ei_frame_t * frame,
                ei_rect_t * clipper)
{
        ei_button_draw_loc(window, rectangle, frame->bg_color, frame->relief, 0,
                        frame->border_width, clipper);

        ei_rect_t rectangle_red = reduction(rectangle, frame->border_width);


        if (frame->text && frame->text_font && frame->text_anchor)
                ei_insert_text(window, rectangle_red, frame->text,
                                frame->text_font, frame->text_color,
                                frame->text_anchor, clipper);

        else if (frame->img && frame->img_rect)
                print_image(window, rectangle_red, frame->img,
                                frame->img_rect, frame->img_anchor, clipper);
}

void ei_button_draw(ei_surface_t window, ei_rect_t rectangle,
                ei_button_t * button, ei_rect_t * clipper)
{
        if (button) {
                ei_button_draw_loc(window, rectangle, *button->color, button->relief,
                                button->corner_radius, button->border_width, clipper);

                /* 2 pixels en plus pour la visibilité */
                int marge = button->border_width + 2;

                ei_rect_t rectangle_reduit = reduction(rectangle, marge);

                if (button->text) {
                        ei_rect_t* inter = ei_rect_intersection(&rectangle_reduit,clipper);

                        if (inter)
                                ei_insert_text(window, rectangle_reduit, button->text,
                                                button->text_font, button->text_color,
                                                button->text_anchor, inter);

                        SAFE_FREE(inter);
                }

                else if (button->img && button->img_rect)
                        print_image(window, rectangle_reduit, button->img,
                                        button->img_rect, button->img_anchor, clipper);
        }
}

void ei_toplevel_draw(ei_surface_t surface, ei_toplevel_t * toplevel,
                ei_rect_t * clipper)
{
        /* Requiert : screenlocation.size = { 2*bw+content_rect largeur, bar + 2*bw +content_rect hauteur } */

        ei_rect_t rec = toplevel->widget.screen_location;
        ei_rect_t* inter;

        /* Dessin de la bordure */
        ei_rect_t bord = rec;
        bord.top_left = plus(rec.top_left, 0, toplevel->bar_height);
        bord.size.height = bord.size.height - toplevel->bar_height;

        ei_color_t bord_color = obscurcir(toplevel->color, 0.3);
        inter=ei_rect_intersection(clipper,&bord);
        if (inter) ei_fill(surface,&bord_color,inter);
        SAFE_FREE(inter);

        /* Dessin du content_rect */
        inter=ei_rect_intersection(clipper,toplevel->widget.content_rect);
        if (inter) ei_fill(surface,&toplevel->color,inter);
        SAFE_FREE(inter);

        /* Dessin de la barre */
        ei_rect_t bar = rec;
        bar.size.height = toplevel->bar_height;

        //Amélioration
        inter=ei_rect_intersection(clipper,&bar);
        if (inter) ei_fill(surface,&toplevel->bar_color,inter);
        SAFE_FREE(inter);


        int marge;
        ei_rect_t btn_c;

        /* Dessin du bouton close */
        if (toplevel->closable) {
                ei_color_t btn_c_color = { 0, 0, 0, 255 };
                marge = toplevel->bar_height * 0.25;
                btn_c.top_left = plus(rec.top_left, marge, marge);
                btn_c.size.width = toplevel->bar_height - 2 * marge;
                btn_c.size.height = toplevel->bar_height - 2 * marge;
                ei_button_draw_loc(surface, btn_c, btn_c_color,
                                toplevel->rel_btn_close, 0, marge, clipper);
        } else {
                marge = 0;
                btn_c.size.width = 0;
                btn_c.size.height = 0;
        }

        /* Dessin du bouton resize */
        if (toplevel->resizable) {
                ei_color_t btn_r_color = eclaircir(toplevel->color, 0.2);
                ei_rect_t btn_r;
                ei_size_t btn_r_s =
                { toplevel->resize_size, toplevel->resize_size };
                btn_r.size = btn_r_s;

                int width = rec.size.width;
                int height = rec.size.height;
                btn_r.top_left = plus(rec.top_left,
                                width - toplevel->resize_size - toplevel->border_width,
                                height - toplevel->resize_size -
                                toplevel->border_width);

                ei_button_draw_loc(surface, btn_r, btn_r_color, ei_relief_none,
                                0, 0, clipper);
        }
        if (toplevel->title) {
                /* Affichage du titre */
                ei_rect_t rec_txt;
                int marge_txt=marge + btn_c.size.width+5;
                rec_txt.top_left = plus(rec.top_left,marge_txt, 0);
                rec_txt.size.height = toplevel->bar_height;
                rec_txt.size.width = toplevel->widget.screen_location.size.width -
                        marge_txt;
                ei_rect_t rec_clip =rec_txt;
                rec_clip.size.width=rec_txt.size.width+marge +btn_c.size.width;
                inter =ei_rect_intersection(clipper,&rec_clip);
                if (inter) 
                        ei_insert_text(surface, rec_txt, toplevel->title,
                                        toplevel->title_font, toplevel->title_color, 1,
                                        inter);
                SAFE_FREE(inter);
        }
}

void ei_button_draw_loc(ei_surface_t window, ei_rect_t rectangle,
                ei_color_t couleur, ei_relief_t relief, int rayon,
                int marge, ei_rect_t * clipper)
{
        ei_linked_point_t *Liste = NULL;

        if (relief == ei_relief_none) {
                Liste = ei_button_rounded_frame(rectangle, rayon, 0);
                ei_draw_polygon(window, Liste, couleur, clipper);
                free_lp(Liste);
        } else {
                ei_color_t couleur_eclairee, couleur_assombrie, couleur_haute, couleur_basse;
                float coeff_couleur = 0.2;
                couleur_eclairee = eclaircir(couleur, coeff_couleur);
                couleur_assombrie = obscurcir(couleur, coeff_couleur);

                if (relief == ei_relief_raised) {
                        couleur_haute = couleur_eclairee;
                        couleur_basse = couleur_assombrie;
                } else {
                        couleur_haute = couleur_assombrie;
                        couleur_basse = couleur_eclairee;
                }

                ei_linked_point_t *partie_haute = ei_button_rounded_frame(rectangle, rayon, 1);
                ei_draw_polygon(window, partie_haute, couleur_haute, clipper);
                free_lp(partie_haute);

                ei_linked_point_t *partie_basse = ei_button_rounded_frame(rectangle, rayon, -1);
                ei_draw_polygon(window, partie_basse, couleur_basse, clipper);
                free_lp(partie_basse);

                ei_rect_t rectangle_interieur = reduction(rectangle, marge);


                if (rayon != 0)
                        Liste = ei_button_rounded_frame(rectangle_interieur, rayon - marge, 0);

                else
                        Liste = ei_button_rounded_frame(rectangle_interieur, 0, 0);


                ei_draw_polygon(window, Liste, couleur, clipper);
                free_lp(Liste);

        }
}

void ei_insert_text(ei_surface_t window, ei_rect_t rectangle, char *text,
                ei_font_t font, ei_color_t color, ei_anchor_t anchor,
                ei_rect_t * clipper)
{
        ei_point_t ancre;
        int width, height;

        hw_text_compute_size(text, font, &width, &height);
        if (clipper && anchor==ei_anc_center && width>rectangle.size.width)
                anchor_translation(&anchor);


        ancre = find_anchor(rectangle, width, height, anchor);

        ei_draw_text(window, &ancre, text, font, &color, clipper);
}

void print_image(ei_surface_t window, ei_rect_t rectangle, ei_surface_t img,
                ei_rect_t * img_rect, ei_anchor_t img_anchor, ei_rect_t * clipper)
{
        ei_rect_t new_img_rect;
        ei_rect_t real_img_rect = hw_surface_get_rect(img);

        if (img_rect) {

                new_img_rect = *img_rect;

                /* If img_rect->top_left out of image, relocate to origin */
                if (new_img_rect.top_left.x > real_img_rect.size.width
                                || new_img_rect.top_left.y > real_img_rect.size.height)
                        new_img_rect.top_left = ei_point(0, 0);

                if (new_img_rect.top_left.x + new_img_rect.size.width >
                                real_img_rect.size.width)
                        new_img_rect.size.width = real_img_rect.size.width - new_img_rect.top_left.x;

                if (new_img_rect.top_left.y + new_img_rect.size.height >
                                real_img_rect.size.height)
                        new_img_rect.size.height = real_img_rect.size.height - new_img_rect.top_left.y;

                /* Default image rectangle to full image */
        } else {
                new_img_rect = real_img_rect;
        }


        int w_dst = rectangle.size.width;
        int h_dst = rectangle.size.height;
        int w_ori = new_img_rect.size.width;
        int h_ori = new_img_rect.size.height;
        int cstraint_w = MIN(w_dst, w_ori);
        int cstraint_h = MIN(h_dst, h_ori);
        ei_size_t size = { cstraint_w, cstraint_h };

        ei_rect_t img_part;
        if (w_ori > w_dst || h_ori > h_dst) {
                ei_point_t ancre;
                ancre = find_anchor(new_img_rect, cstraint_w, cstraint_h, img_anchor);
                img_part.top_left = ancre;
                img_part.size = size;
        } else {
                img_part = new_img_rect;
        }

        ei_rect_t rec_dst;
        if (size.width == rectangle.size.width
                        && size.height == rectangle.size.height) {
                rec_dst = rectangle;
        } else {
                ei_point_t ancre_dst;
                ancre_dst = find_anchor(rectangle, cstraint_w, cstraint_h, img_anchor);
                rec_dst.top_left = ancre_dst;
                rec_dst.size = size;
        }

        ei_bool_t display = EI_TRUE;

        /* Clip image */
        if (clipper) {

                // On réduit les clippeurs de destination
                // pour ne pas déborder du clippeur brut
                ei_rect_t vor = rec_dst;
                ei_rect_t *inter = ei_rect_intersection(&rec_dst, clipper);
                if (inter) {
                        rec_dst = *inter;
                        SAFE_FREE(inter);

                        // On ajoute la réduction réalisée par l'intersection avec le clipper brut
                        // au clippeur de l'image
                        // img_part = img_part + (inter - rec_dst)
                        img_part.top_left.x+= rec_dst.top_left.x - vor.top_left.x;
                        img_part.top_left.y+= rec_dst.top_left.y - vor.top_left.y;
                        img_part.size.width+= rec_dst.size.width - vor.size.width;
                        img_part.size.height+= rec_dst.size.height - vor.size.height;
                }

                /* If no intersection, do not display */
                else
                        display = EI_FALSE;
        }

        img_part.size = rec_dst.size;


        if (display && (rec_dst.size.width > 0 && rec_dst.size.height > 0)) {
                hw_surface_lock(img);


#ifndef NDEBUG
                int result =
#endif

                        ei_copy_surface(window, &rec_dst, img, &img_part, 1);

#ifndef NDEBUG
                assert(result == 0);
#endif


                hw_surface_unlock(img);
        }
}
