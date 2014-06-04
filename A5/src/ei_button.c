
#include "ei_button.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define Pi 3.14

ei_linked_point_t* ei_button_arc(ei_point_t centre,int rayon,int angle_debut,int angle_fin,ei_linked_point_t* suivant) {
	ei_linked_point_t* Point_suivant=suivant;
	ei_linked_point_t* Point;
	for (int theta=angle_fin; theta>=angle_debut; theta --) {
		Point=malloc(sizeof(ei_linked_point_t));
		float theta_rad=theta*Pi/180.0;
		Point->point.x=centre.x+rayon*cos(theta_rad);
		Point->point.y=centre.y+rayon*sin(theta_rad);
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
	ei_linked_point_t* Point_suivant=suivant;
	int coeffx=0;
	int coeffy=0;
	int diffx=tete.x-queue.x;
	int diffy=tete.y-queue.y;
	int nb_iterations=(diffx+diffy)/2;
	if (diffx!=0) {
		coeffx=(diffx)/abs(diffx);
	}
	if (diffy!=0) {
		coeffy=(diffy)/abs(diffy);
	}
	ei_linked_point_t* Point;
	printf("coeffx:%i,coeffy:%i\n",coeffx,coeffy);
	for (int i=0; i<=nb_iterations; i++) {
		Point=malloc(sizeof(ei_linked_point_t));
		Point->point.x=queue.x+i*coeffx;
		Point->point.y=queue.y+i*coeffy;
		Point->next=Point_suivant;
		Point_suivant=Point;
	}
	return Point;
}
/*
ei_linked_point_t* ei_button_rounded_frame(ei_rect_t rectangle, int rayon, part_rect_t partie) {
	ei_linked_point_t* Liste=NULL;
	int hauteur=rectangle.size.heigth-2*rayon;
	int longueur=rectangle.size.width-2*rayon;
	int xrec=rectangle.top_left.x;
	int yrec=rectangle.top_left.y;

	ei_point_t top_gauche={xrec+rayon,yrec};
	ei_point_t top_droit={xrec+longueur-rayon,yrec};
	ei_point_t gauche_top={xrec,yrec+rayon};
	ei_point_t gauche_bot={xrec,yrec+hauteur-rayon};
	ei_point_t droite_top={xrec+longueur,yrec+r};
	ei_point_t droite_bot={xrec+longueur,yrec+hauteur-r};
	ei_point_t bot_gauche={xrec+rayon,yrec+hauteur};
	ei_point_t bot_droite={xrec+longueur-rayon,yrec+hauteur};

	ei_point_t centre_tg={xrec+rayon,yrec+rayon};
	ei_point_t centre_bg={xrec+rayon,yrec+hauteur-rayon};
	ei_point_t centre_td={xrec+longueur-rayon,yrec+rayon};
	ei_point_t centre_bd={xrec+longueur-rayon,yrec+hauteur-rayon};

	if (partie==complet) {
		Liste=trait(top_gauche,top_droit,Liste);
		
		Liste=ei_button_arc(centre_td,rayon,0,90,Liste);

		Liste=trait(droite_top,droite_bot,Liste);

		Liste=ei_button_arc(centre_bd,rayon,-90,0,Liste);

		Liste=trait(bot_droite,boite_gauche,Liste);

		Liste=ei_button_arc(centre_bg,rayon,-180,-90,Liste);

		Liste=trait(gauche_bot,gauche_top,Liste);

		Liste=ei_button_arc(centre_tg,rayon,-270,-180,Liste);
	}
	else {
		int h = min(longueur/2,largeur/2);
		ei_point_t arrondi_bg={xrec+r-r*cos(-3*Pi/4);yrec+hauteur-r+r*sin(-3*Pi/4)};
		ei_point_t arrondi_td={xrec+longueur-r+rcos(Pi/4),yrec+r-r*sin(Pi/4)};
		ei_point_t pt_int_bg={xrec+h,yrec+hauteur-h};
		ei_point_t pt_int_td={xrec+longeur-h,yrec+h};

		Liste=trait(arrondi_bg,pt_int_bg,Liste);

		Liste=trait(pt_int_bg,pt_int_td,Liste);

		Liste=trait(pt_int_td,arrondi_td,Liste);

		if (partie==haute) {
			Liste=ei_button_arc(centre_td,rayon,90,45,Liste);

			Liste=trait(top_droite,top_gauche,Liste);

			Liste=ei_button_arc(centre_tg,180,90,Liste);

			Liste=trait(gauche_top,gauche_bot,Liste);
	
			Liste=ei_button_arc(centre_bg,225,180,Liste);

		}
		else {
			Liste=ei_button_arc(centre_td,rayon,0,45,Liste);

			Liste=trait(droite_top,droite_bot,Liste);

			Liste=ei_button_arc(centre_tg,-90,0,Liste);

			Liste=trait(bot_droite,bot_gauche,Liste);
	
			Liste=ei_button_arc(centre_bg,-135,-90,Liste);

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
	ei_linked_point_t* rec_interieur=ei_button_rounded_frame(rectangle_interieur,rayon-marge,0);
	ei_draw_polygon();
}
*/
