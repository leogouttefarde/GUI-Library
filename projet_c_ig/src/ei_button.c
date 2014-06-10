
#include "ei_button.h"
#include "ei_draw.h"
#include "ei_common.h"
#include "ei_shape.h"
#include "ei_utilities.h"

/* Frame draw */
void ei_frame_draw(ei_surface_t window, ei_rect_t rectangle, ei_frame_t * frame,
		ei_rect_t *clipper)
{
	//printf("debut dessin frame \n");
	ei_button_draw_loc(window, rectangle, frame->bg_color, frame->relief, 0, frame->border_width, clipper);
	
	ei_rect_t rectangle_red = reduction(rectangle, frame->border_width);
	//printf("fin dessin, debut texte\n");
	if (frame->text && frame->text_font && frame->text_anchor) {
		//printf("frame->text non null\n");
		ei_insert_text(window, rectangle_red, frame->text,
				frame->text_font, frame->text_color,
				frame->text_anchor);
	}
	else {
		if (frame->img) {
			if (frame->img_rect) aff_img(window,rectangle_red,frame->img,frame->img_rect,frame->img_anchor);
			//else printf("frame->img_rect=NULL\n");
		}
		//else printf("frame->img=NULL\n");
	}
}

void ei_button_draw(ei_surface_t window, ei_rect_t rectangle,
		ei_button_t * button, ei_rect_t *clipper)
{
	//printf("debut dessin \n");
	ei_button_draw_loc(window, rectangle, *button->color, button->relief,
			button->corner_radius, button->border_width, clipper);

	int marge = button->border_width + 2;   //2 pixels en plus pour la visibilité
	//peut-être mettre en fonction du rayon aussi pour que cela soit plus joli dans certain cas
	ei_rect_t rectangle_reduit=reduction(rectangle,marge);

	if (button->text) {
		ei_insert_text(window, rectangle_reduit, button->text,
				button->text_font, button->text_color,
				button->text_anchor);
	}
	else {
		if (button->img) {
			if (button->img_rect)
				aff_img(window, rectangle_reduit, button->img,
						button->img_rect, button->img_anchor);
			//else printf("button->img_rect=NULL\n");
		}
		//else printf("button->img=NULL\n");
	}
	//printf("fin dessin bouton\n");
}

void ei_toplevel_draw(ei_surface_t surface, ei_toplevel_t *toplevel, ei_rect_t *clipper) {
	ei_rect_t rec=toplevel->widget.screen_location;
	ei_linked_point_t lp;
//Dessin de la bordure
	ei_rect_t bord=rec;
	bord.top_left=plus(rec.top_left,0,toplevel->bar_height);
	bord.size.height=bord.size.height-toplevel->bar_height;
	lp=ei_rect_to_points(bord);
	ei_color_t bord_color=obscurcir(toplevel->color,0.3);
	ei_draw_polygon(surface,&lp,bord_color,clipper);
//Dessin du content_rect
   lp =ei_rect_to_points(*toplevel->widget.content_rect);
   ei_draw_polygon(surface, &lp, toplevel->color, clipper);

//Dessin de la barre	
	//printf("début ei_bar_draw\n");
	ei_rect_t bar=rec;
	bar.size.height=toplevel->bar_height;
	lp=ei_rect_to_points(bar);
	ei_draw_polygon(surface,&lp,toplevel->bar_color,clipper);
	//printf("barre déssinée, on dessine le bouton \n");
//Dessin du bouton close
	ei_color_t btn_c_color={0,0,0,255};
	int marge=toplevel->bar_height*0.25;
	ei_rect_t btn_c;
	btn_c.top_left=plus(rec.top_left,marge,marge);
	btn_c.size.width=toplevel->bar_height-2*marge;
	btn_c.size.height=toplevel->bar_height-2*marge;
	marge=0.2*btn_c.size.height;
	ei_button_draw_loc(surface,btn_c,btn_c_color,toplevel->rel_btn_close,0,marge,clipper);
	//printf("bouton déssiné\n");
//Dessin du bouton resize
	ei_color_t btn_r_color=eclaircir(toplevel->color,0.2);
	ei_rect_t btn_r;
	ei_size_t btn_r_s={toplevel->resize_size,toplevel->resize_size};
	btn_r.size=btn_r_s;
	int width=toplevel->widget.screen_location.size.width;
	int height=toplevel->widget.screen_location.size.height;
	btn_r.top_left=plus(rec.top_left,width-toplevel->resize_size-toplevel->border_width,height-toplevel->resize_size-toplevel->border_width);
	ei_button_draw_loc(surface,btn_r,btn_r_color,ei_relief_none,0,0,clipper);
//Affichage du titre
	ei_rect_t rec_txt;
	rec_txt.top_left=plus(rec.top_left,2*marge+btn_c.size.width,0);
	rec_txt.size.height=toplevel->bar_height;
	rec_txt.size.width=toplevel->widget.screen_location.size.width-2*(2*marge+btn_c.size.width);
	ei_insert_text(surface,rec_txt,toplevel->title,toplevel->title_font,toplevel->title_color,0);
}

void ei_button_draw_loc(ei_surface_t window, ei_rect_t rectangle,
		ei_color_t couleur, ei_relief_t relief, int rayon,
		int marge, ei_rect_t *clipper)
{
	ei_linked_point_t *Liste = NULL;

	if (relief == ei_relief_none) {
		Liste = ei_button_rounded_frame(rectangle, rayon, 0);
		ei_draw_polygon(window, Liste, couleur, NULL);
		free_lp(Liste);
	} else {
		ei_color_t couleur_eclairee, couleur_assombrie, couleur_haute,
					  couleur_basse;
		float coeff_couleur = 0.2;
		couleur_eclairee.red = MIN(couleur.red + coeff_couleur * 255, 255);
		couleur_eclairee.green = MIN(couleur.green + coeff_couleur * 255, 255);
		couleur_eclairee.blue = MIN(couleur.blue + coeff_couleur * 255, 255);
		couleur_eclairee.alpha = 255;

		couleur_assombrie.red = MAX(couleur.red - coeff_couleur * 255, 0);
		couleur_assombrie.green = MAX(couleur.green - coeff_couleur * 255, 0);
		couleur_assombrie.blue = MAX(couleur.blue - coeff_couleur * 255, 0);
		couleur_assombrie.alpha = 255;

		if (relief == ei_relief_raised) {
			couleur_haute = couleur_eclairee;
			couleur_basse = couleur_assombrie;
		} else {
			couleur_haute = couleur_assombrie;
			couleur_basse = couleur_eclairee;
		}

		ei_linked_point_t *partie_haute =
			ei_button_rounded_frame(rectangle, rayon, 1);
		ei_draw_polygon(window, partie_haute, couleur_haute, clipper);
		free_lp(partie_haute);

		ei_linked_point_t *partie_basse = ei_button_rounded_frame(rectangle, rayon, -1);
		ei_draw_polygon(window, partie_basse, couleur_basse, clipper);
		free_lp(partie_basse);

		ei_rect_t rectangle_interieur=reduction(rectangle,marge);
		if (rayon!=0) Liste=ei_button_rounded_frame(rectangle_interieur, rayon -marge, 0);
		else Liste=ei_button_rounded_frame(rectangle_interieur, 0, 0);
		ei_draw_polygon(window, Liste, couleur, clipper);
		free_lp(Liste);
	}
}

void ei_insert_text(ei_surface_t window, ei_rect_t clipper, char *text,
		ei_font_t font, ei_color_t color, ei_anchor_t anchor)
{
	//printf("j'affiche le texte \n");
	int width,height;
	hw_text_compute_size(text, font, &width, &height);
	int longueur = clipper.size.width;
	int hauteur = clipper.size.height;
	ei_point_t ancre;
	ei_point_t top_gauche = clipper.top_left;
	ei_point_t top_mid = { top_gauche.x + longueur / 2, top_gauche.y };
	ei_point_t centre = { top_gauche.x + longueur / 2, top_gauche.y + hauteur / 2 };
	ei_point_t top_droite = { top_gauche.x + longueur, top_gauche.y + hauteur * 0 };
	ei_point_t droite_mid = { top_gauche.x + longueur, top_gauche.y + hauteur / 2 };
	ei_point_t bot_droite = { top_gauche.x + longueur, top_gauche.y + hauteur };
	ei_point_t bot_mid = { top_gauche.x + longueur / 2, top_gauche.y + hauteur };
	ei_point_t bot_gauche = { top_gauche.x + longueur * 0, top_gauche.y + hauteur };
	ei_point_t gauche_mid = { top_gauche.x + longueur * 0, top_gauche.y + hauteur / 2 };
	switch (anchor) {
		case ei_anc_none:
		case ei_anc_center:
			ancre = plus(centre, -width / 2, -height / 2);
			break;
		case ei_anc_north:
			ancre = plus(top_mid, -width / 2, 0);
			break;
		case ei_anc_northeast:
			ancre = plus(top_droite, -width, 0);
			break;
		case ei_anc_east:
			ancre = plus(droite_mid, -width, 0);
			break;
		case ei_anc_southeast:
			ancre = plus(bot_droite, -width, -height);
			break;
		case ei_anc_south:
			ancre = plus(bot_mid, -width / 2, -height);
			break;
		case ei_anc_southwest:
			ancre = plus(bot_gauche, 0, -height);
			break;
		case ei_anc_west:
			ancre = plus(gauche_mid, 0, -height / 2);
			break;
		case ei_anc_northwest:
			ancre = top_gauche;
			break;
	}
	ei_draw_text(window, &ancre, text, font, &color, &clipper);
}

void aff_img(ei_surface_t window, ei_rect_t rectangle, ei_surface_t img,
		ei_rect_t * img_rect, ei_anchor_t img_anchor)
{
	int result;

	//printf("jaffiche l'image");
	assert(img_rect);

	hw_surface_lock(img);
	ei_rect_t nv_img_rect;
	if (img_rect) {
		printf("img_rect size{%i,%i}\n",img_rect->size.width,img_rect->size.height);
		nv_img_rect=*img_rect;
	} else {
		nv_img_rect=hw_surface_get_rect(img);
	}

	int width=rectangle.size.width;
	int height=rectangle.size.height;

	ei_point_t ancre;
	ei_point_t top_gauche = nv_img_rect.top_left;
	int longueur=nv_img_rect.size.width;
	int hauteur=nv_img_rect.size.height;
	ei_point_t top_mid = { top_gauche.x + longueur / 2, top_gauche.y };
	ei_point_t centre = { top_gauche.x + longueur / 2, top_gauche.y + hauteur / 2 };
	ei_point_t top_droite = { top_gauche.x + longueur, top_gauche.y + hauteur * 0 };
	ei_point_t droite_mid = { top_gauche.x + longueur, top_gauche.y + hauteur / 2 };
	ei_point_t bot_droite = { top_gauche.x + longueur, top_gauche.y + hauteur };
	ei_point_t bot_mid = { top_gauche.x + longueur / 2, top_gauche.y + hauteur };
	ei_point_t bot_gauche = { top_gauche.x + longueur * 0, top_gauche.y + hauteur };
	ei_point_t gauche_mid = { top_gauche.x + longueur * 0, top_gauche.y + hauteur / 2 };

	switch (img_anchor) {
		case ei_anc_none:
		case ei_anc_center:
			ancre = plus(centre, -width / 2, -height / 2);
			break;
		case ei_anc_north:
			ancre = plus(top_mid, -width / 2, 0);
			break;
		case ei_anc_northeast:
			ancre = plus(top_droite, -width, 0);
			break;
		case ei_anc_east:
			ancre = plus(droite_mid, -width, 0);
			break;
		case ei_anc_southeast:
			ancre = plus(bot_droite, -width, -height);
			break;
		case ei_anc_south:
			ancre = plus(bot_mid, -width / 2, -height);
			break;
		case ei_anc_southwest:
			ancre = plus(bot_gauche, 0, -height);
			break;
		case ei_anc_west:
			ancre = plus(gauche_mid, 0, -height / 2);
			break;
		case ei_anc_northwest:
			ancre = top_gauche;
			break;
	}
	
	ei_rect_t img_part = { ancre, rectangle.size };
	
	//printf("img_part size{%i,%i}\n",img_part.size.width,img_part.size.height);
	//printf("rectangle size{%i,%i}\n",rectangle.size.width,rectangle.size.height);
	result = ei_copy_surface(window, &rectangle, img, &img_part, 1);

	hw_surface_unlock(img);

	assert(!result);
	//printf("result %x\n", result);
}

