
#include "ei_button.h"
#include "ei_draw.h"
#include "ei_common.h"


ei_linked_point_t *ei_button_arc(ei_point_t centre, int rayon, int angle_tete,
		int angle_queue, ei_linked_point_t * suivant)
{
	ei_linked_point_t *Point_suivant = suivant;
	ei_linked_point_t *Point;
	for (float theta = angle_queue; theta >= angle_tete; theta = theta -0.5) {
		Point = CALLOC_TYPE(ei_linked_point_t);
		assert(Point != NULL);

		float theta_rad = theta * M_PI / 180.0;
		Point->point.x = centre.x + rayon * cos(theta_rad);
		Point->point.y = centre.y - rayon * sin(theta_rad);
		Point->next = Point_suivant;
		Point_suivant = Point;
	}
	return Point;
}

/**
 *\brief Algo simple qui marche pour des traits horizontaux,verticaux ou diagonaux
 *@param queue La fin de la liste de points chainées qui sera reliées à suivant
 *@param tete La tete de la liste, point qui est renvoyé à la fin
 *@param suivant la liste à la laquelle on va chainer le trait que l'on crée
 */

ei_linked_point_t *trait(ei_point_t queue, ei_point_t tete,
		ei_linked_point_t * suivant)
{
	ei_linked_point_t *Point_suivant = suivant;
	int coeffx = 0;
	int coeffy = 0;
	int diffx = tete.x - queue.x;
	int diffy = tete.y - queue.y;
	int nb_iterations = MAX(abs(diffx),abs(diffy));
	if (diffx != 0) {
		coeffx = (diffx) / abs(diffx);
	}
	if (diffy != 0) {
		coeffy = (diffy) / abs(diffy);
	}
	ei_linked_point_t *Point;
	for (int i = 0; i <= nb_iterations; i++) {
		Point = CALLOC_TYPE(ei_linked_point_t);
		assert(Point != NULL);

		Point->point.x = queue.x + i * coeffx;
		Point->point.y = queue.y + i * coeffy;
		Point->next = Point_suivant;
		Point_suivant = Point;
	}
	return Point;
}

ei_linked_point_t *ei_button_rounded_frame(ei_rect_t rectangle, int rayon,
		part_rect_t partie)
{
	ei_linked_point_t *Liste = NULL;
	int hauteur = rectangle.size.height;
	int longueur = rectangle.size.width;
	int xrec = rectangle.top_left.x;
	int yrec = rectangle.top_left.y;

	ei_point_t top_gauche = { xrec + rayon, yrec };
	ei_point_t top_droit = { xrec + longueur - rayon, yrec };
	ei_point_t gauche_top = { xrec, yrec + rayon };
	ei_point_t gauche_bot = { xrec, yrec + hauteur - rayon };
	ei_point_t droite_top = { xrec + longueur, yrec + rayon };
	ei_point_t droite_bot = { xrec + longueur, yrec + hauteur - rayon };
	ei_point_t bot_gauche = { xrec + rayon, yrec + hauteur };
	ei_point_t bot_droite = { xrec + longueur - rayon, yrec + hauteur };

	ei_point_t centre_tg = { xrec + rayon, yrec + rayon };
	ei_point_t centre_bg = { xrec + rayon, yrec + hauteur - rayon };
	ei_point_t centre_td = { xrec + longueur - rayon, yrec + rayon };
	ei_point_t centre_bd = { xrec + longueur - rayon, yrec + hauteur - rayon };

	if (partie == complet) {
		Liste = trait(top_gauche, top_droit, Liste);
		Liste = ei_button_arc(centre_td, rayon, 0, 90, Liste);
		Liste = trait(droite_top, droite_bot, Liste);
		Liste = ei_button_arc(centre_bd, rayon, -90, 0, Liste);
		Liste = trait(bot_droite, bot_gauche, Liste);
		Liste = ei_button_arc(centre_bg, rayon, -180, -90, Liste);
		Liste = trait(gauche_bot, gauche_top, Liste);
		Liste = ei_button_arc(centre_tg, rayon, -270, -180, Liste);
	} else {
		int h = MIN(longueur / 2, hauteur / 2);

		ei_point_t arrondi_bg = { xrec + rayon - rayon * cos(-3 * M_PI / 4),
			yrec + hauteur - rayon + rayon * sin(-3 * M_PI / 4) };

		ei_point_t arrondi_td = { xrec + longueur - rayon + rayon * cos(M_PI / 4),
			yrec + rayon - rayon * sin(M_PI / 4) };

		ei_point_t pt_int_bg = { xrec + h, yrec + hauteur - h };
		ei_point_t pt_int_td = { xrec + longueur - h, yrec + h };

		if (partie == haute) {
			Liste = ei_button_arc(centre_bg, rayon, 180, 225, Liste);
			Liste = trait(gauche_bot, gauche_top, Liste);
			Liste = ei_button_arc(centre_tg, rayon, 90, 180, Liste);
			Liste = trait(top_gauche, top_droit, Liste);
			Liste = ei_button_arc(centre_td, rayon, 45, 90, Liste);
			Liste = trait(arrondi_td, pt_int_td, Liste);
			Liste = trait(pt_int_td, pt_int_bg, Liste);
			Liste = trait(pt_int_bg, arrondi_bg, Liste);
		} else {
			Liste = trait(arrondi_bg, pt_int_bg, Liste);
			Liste = trait(pt_int_bg, pt_int_td, Liste);
			Liste = trait(pt_int_td, arrondi_td, Liste);
			Liste = ei_button_arc(centre_td, rayon, 0, 45, Liste);
			Liste = trait(droite_top, droite_bot, Liste);
			Liste = ei_button_arc(centre_bd, rayon, -90, 0, Liste);
			Liste = trait(bot_droite, bot_gauche, Liste);
			Liste = ei_button_arc(centre_bg, rayon, -135, -90, Liste);
		}
	}
	return Liste;
}

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
		ei_button_text(window, rectangle_red, frame->text,
				frame->text_font, frame->text_color,
				frame->text_anchor);
	}
	else {
		if (frame->img) {
			if (frame->img_rect) aff_img(window,rectangle_red,frame->img,frame->img_rect,frame->img_anchor);
			else printf("frame->img_rect=NULL\n");
		}
		else printf("frame->img=NULL\n");
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
		ei_button_text(window, rectangle_reduit, button->text,
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

/**
 *\brief Trace un bouton en relief (enfoncé ou relevé) ou plat
 *@param window surface sur laquelle on dessine le bouton
 *@param rectangle le rectangle que l'on transforme en bouton
 *@param couleur la couleur du bouton central
 *@param relief si raised =>relevé,none=>plat,sinon enfoncé
 *@param le rayon des angles
 *@param la marge
 *@param le clipper qui restreint le bouton
 */
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

void ei_button_text(ei_surface_t window, ei_rect_t clipper, char *text,
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

void free_lp(ei_linked_point_t * Liste)
{
	ei_linked_point_t *Suivant;
	while (Liste != NULL) {
		Suivant = Liste->next;
		SAFE_FREE(Liste);

		Liste = Suivant;
	}
}

ei_point_t plus(ei_point_t A, int abc, int ord)
{
	ei_point_t point = { A.x + abc, A.y + ord };
	return point;
}

void aff_img(ei_surface_t window, ei_rect_t rectangle, ei_surface_t img,
		ei_rect_t * img_rect, ei_anchor_t img_anchor)
{
	int result;

	//printf("jaffiche l'image");
	assert(img_rect);

	hw_surface_lock(img);

	//printf("rectangle.top_left.x %d\n", rectangle.top_left.x);
	//printf("rectangle.top_left.y %d\n", rectangle.top_left.y);

	//printf("rectangle.width.x %d\n", rectangle.size.width);
	//printf("rectangle.height.y %d\n", rectangle.size.height);

	//printf("img_rect->top_left.x %d\n", img_rect->top_left.x);
	//printf("img_rect->top_left.y %d\n", img_rect->top_left.y);

	//printf("img_rect->size.width %d\n", img_rect->size.width);
	//printf("img_rect->size.height %d\n", img_rect->size.height);

	// TODO : centrer l'image quand plus grande / petite
	// que le rectangle au lieu de tronquer
	ei_rect_t img_part = { img_rect->top_left, rectangle.size };

	result = ei_copy_surface(window, &rectangle, img, &img_part, 1);

	hw_surface_unlock(img);

	assert(!result);
	//printf("result %x\n", result);
}

ei_rect_t reduction(ei_rect_t rectangle, int marge)
{
	ei_rect_t rectangle_reduit;
	rectangle_reduit.top_left.x = rectangle.top_left.x + marge;
	rectangle_reduit.top_left.y = rectangle.top_left.y + marge;
	rectangle_reduit.size.width = rectangle.size.width - 2 * marge;
	rectangle_reduit.size.height = rectangle.size.height - 2 * marge;

	return rectangle_reduit;
}
