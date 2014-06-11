#include "ei_utilities.h"
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
	ei_point_t tl=rectangle.top_left;
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
/*debug
	if (partie==1) { 
		printf("rayon:%i,top_gauche:{%i,%i}\n",rayon,top_gauche.x,top_gauche.y);
		printf("rec d'origine tl:{%i,%i}\n",xrec,yrec);	
	}
*/
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
		int h;
		ei_point_t arrondi_bg = { xrec + rayon - rayon * cos(-3 * M_PI / 4),
			yrec + hauteur - rayon + rayon * sin(-3 * M_PI / 4) };

		ei_point_t arrondi_td = { xrec + longueur - rayon + rayon * cos(M_PI / 4),
			yrec + rayon - rayon * sin(M_PI / 4) };
		ei_point_t pt_int_bg;
		ei_point_t pt_int_td;
		if (longueur>hauteur) {
			h=hauteur/2;
			//pt_int_bg = { xrec + hauteur/2, yrec + hauteur/2};
			pt_int_bg=plus(tl,h,h);
			//pt_int_td = { xrec + longueur - hauteur/2, yrec + hauteur/2 };
			pt_int_td=plus(tl,longueur-h,h);
		} else {
			h=longueur/2;
			//pt_int_bg = { xrec + longueur/2, yrec + hauteur - longueur/2};
			pt_int_bg=plus(tl,h,hauteur-h);
			//pt_int_td = { xrec + longueur/2, yrec + longueur/2 };
			pt_int_td=plus(tl,h,h);
		}
		//printf("pt_int_bg {%i,%i}\n",pt_int_bg.x,pt_int_bg.y);
		//printf("pt_int_td {%i,%i}\n",pt_int_td.x,pt_int_td.y);

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

ei_rect_t reduction(ei_rect_t rectangle, int marge)
{
	ei_rect_t rectangle_reduit;
	rectangle_reduit.top_left.x = rectangle.top_left.x + marge;
	rectangle_reduit.top_left.y = rectangle.top_left.y + marge;
	rectangle_reduit.size.width = rectangle.size.width - 2 * marge;
	rectangle_reduit.size.height = rectangle.size.height - 2 * marge;

	return rectangle_reduit;
}

ei_color_t eclaircir(ei_color_t couleur, float coeff_couleur) {
		ei_color_t couleur_eclairee;
		couleur_eclairee.red = MIN(couleur.red + coeff_couleur * 255, 255);
		couleur_eclairee.green = MIN(couleur.green + coeff_couleur * 255, 255);
		couleur_eclairee.blue = MIN(couleur.blue + coeff_couleur * 255, 255);
		couleur_eclairee.alpha = 255;
		return couleur_eclairee;
}
ei_color_t obscurcir(ei_color_t couleur, float coeff_couleur) {
		ei_color_t couleur_assombrie;
		couleur_assombrie.red = MAX(couleur.red - coeff_couleur * 255, 0);
		couleur_assombrie.green = MAX(couleur.green - coeff_couleur * 255, 0);
		couleur_assombrie.blue = MAX(couleur.blue - coeff_couleur * 255, 0);
		couleur_assombrie.alpha = 255;
		return couleur_assombrie;
}
/*BRIEF
 *trouve l'ancre dans un rectangle à partir de deux contrainte et de l'ancre
*/
ei_point_t find_anchor(ei_rect_t rectangle,int width,int height, ei_anchor_t position) {
	ei_point_t ancre;
	ei_point_t top_gauche = rectangle.top_left;
	int longueur=rectangle.size.width;
	int hauteur=rectangle.size.height;
	ei_point_t top_mid = { top_gauche.x + longueur / 2, top_gauche.y };
	ei_point_t centre = { top_gauche.x + longueur / 2, top_gauche.y + hauteur / 2 };
	ei_point_t top_droite = { top_gauche.x + longueur, top_gauche.y + hauteur * 0 };
	ei_point_t droite_mid = { top_gauche.x + longueur, top_gauche.y + hauteur / 2 };
	ei_point_t bot_droite = { top_gauche.x + longueur, top_gauche.y + hauteur };
	ei_point_t bot_mid = { top_gauche.x + longueur / 2, top_gauche.y + hauteur };
	ei_point_t bot_gauche = { top_gauche.x + longueur * 0, top_gauche.y + hauteur };
	ei_point_t gauche_mid = { top_gauche.x + longueur * 0, top_gauche.y + hauteur / 2 };
	switch (position) {
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
			ancre = plus(droite_mid, -width, -height/2);
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

	return(ancre);
}
