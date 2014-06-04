
#include "ei_button.h"


ei_linked_point_t* ei_button_arc(ei_point_t centre,int rayon,int angle_debut,int angle_fin,ei_linked_point_t* suivant) {
	ei_linked_point_t* Point_suivant=suivant;
	for (int theta=angle_fin; theta>=angle_debut; theta --) {
		ei_linked_point* Point=malloc(sizeof(ei_linked_point));
		Point->x=centre+rayon*cos(theta);
		Point->y=centre+rayon*sin(theta);
		Point->next=Point_suivant;
		Point_suivant=Point;
	}
	return Point;
}

/**
	*\brief Algo simple qui marche pour des traits horizontaux,verticaux ou diagonaux
	*@param queue La fin de la liste de points chainées qui sera reliées à suivant
	*@param tete La tete de la liste, point qui est renvoyé à la fin
	*@param suivant la liste à la laquelle on va chainer le trait que l'on crée
	*/
ei_linked_point_t* trait(ei_point_t queue, ei_point_t tete, ei_linked_point_t* suivant) {
	Point_suivant=suivant;
	int coeffx=0;
	int coeffy=0;
	int diffx=tete->x-queue->x;
	int diffy=tete->y-queue->y;
	int nb_iterations=(diffx+diffy)/2;
	if (queue->x!=tete->x) {
		coeffx=(diffx)/abs(diffx);
	}
	if (queue->y!=queue->y) {
		coeffy=(diffy)/abs(diffy);
	}
	for (int i=0; i<=nb_iterations; i++) {
		ei_linked_point* Point=malloc(sizeof(ei_linked_point));
		Point->x=queue->x+i*coeffx;
		Point->y=queue->y+i*coeffy;
		Point->next=Point_suivant;
		Point_suivant=Point;
	}
	return Point;
}

ei_linked_point_t* ei_button_rounded_frame(ei_rect_t rectangle, int rayon, part_rect_t partie) {
	ei_linked_point_t* Liste=NULL;
	int hauteur=rectangle->size->heigth-2*rayon;
	int longueur=rectangle->size->width-2*rayon;

	ei_point_t top_gauche={rectangle->x+rayon,rectangle->y};
	ei_point_t top_droit={rectangle->x+longueur-rayon,rectangle->y};
	ei_point_t gauche_top={rectangle->x,rectangle->y+rayon};
	ei_point_t gauche_bot={rectangle->x,rectangle->y+hauteur-rayon};
	ei_point_t droite_top={rectangle->x+longueur,rectangle->y+r};
	ei_point_t droite_bot={rectangle->x+longueur,rectangle->y+hauteur-r};
	ei_point_t bot_gauche={rectangle->x+rayon,rectangle->y+hauteur};
	ei_point_t bot_droite={rectangle->x+longueur-rayon,rectangle->y+hauteur};

	ei_point_t centre_tg={rectangle->x+rayon,rectangle->y+rayon};
	ei_point_t centre_bg={rectangle->x+rayon,rectangle->y+hauteur-rayon};
	ei_point_t centre_td={rectangle->x+longueur-rayon,rectangle->y+rayon};
	ei_point_t centre_bd={rectangle->x+longueur-rayon,rectangle->y+hauteur-rayon};

	if (partie==complet) {
		Liste=trait(top_gauche,top_droit,Liste);
		
		Liste=arc(centre_td,rayon,0,90,Liste);

		Liste=trait(droite_top,droite_bot,Liste);

		Liste=arc(centre_bd,rayon,-90,0,Liste);

		Liste=trait(bot_droite,boite_gauche,Liste);

		Liste=arc(centre_bg,rayon,-180,-90,Liste);

		Liste=trait(gauche_bot,gauche_top,Liste);

		Liste=arc(centre_tg,rayon,-270,-180,Liste);
	}
	else {
		int h = min(longueur/2,largeur/2);
		ei_point_t arrondi_bg={rectangle->x+r-r*cos(-3*Pi/4);rectangle->y+hauteur-r+r*sin(-3*Pi/4)};
		ei_point_ arrondi_td={rectangle->x+longueur-r+rcos(Pi/4),rectangle->y+r-r*sin(Pi/4)};
		ei_point_t pt_int_bg={rectangle->x+h,rectangle->y+hauteur-h};
		ei_point_t pt_int_td={rectangle->x+longeur-h,rectangle->y+h};

		Liste=trait(arrondi_bg,pt_int_bg,Liste);

		Liste=trait(pt_int_bg,pt_int_td,Liste);

		Liste=trait(pt_int_td,arrondi_td,Liste);

		if (partie==haute) {
			Liste=arc(centre_td,rayon,90,45,Liste);

			Liste=trait(top_droite,top_gauche,Liste);

			Liste=arc(centre_tg,180,90,Liste);

			Liste=trait(gauche_top,gauche_bot,Liste);
	
			Liste=arc(centre_bg,225,180,Liste);

		}
		else {
			Liste=arc(centre_td,rayon,0,45,Liste);

			Liste=trait(droite_top,droite_bot,Liste);

			Liste=arc(centre_tg,-90,0,Liste);

			Liste=trait(bot_droite,bot_gauche,Liste);
	
			Liste=arc(centre_bg,-135,-90,Liste);

		}

	return Liste;
}

void ei_button_draw(ei_rect_t rectangle,int rayon,int marge) {
	ei_linked_point_t* partie_haute=ei_button_rounded_frame(rectangle,rayon,1);
	ei_draw_polygon();
	ei_linked_point_t* partie_basse=ei_button_rounded_frame(rectangle,rayon,-1);
	ei_draw_polygon();

	ei_rectangle_t rectangle_interieur;
	rectangle_interieur->top_left={rectangle->x+marge,rectangle->y+marge};
	rectangle_interieur->size={rectangle->size->width-2*marge,rectangle->size->height-2*marge};
	ei_linked_point_t* rec_interieur=ei_button_rounded_frame(rectangl_interieur,rayon-marge);
	ei_draw_polygon();
}

	
