
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

	ei_point_t tete_temp;
	ei_point_t queue_temp;
	ei_point_t centre_temp;

	if (partie==complet) {
		queue_temp={rectangle->x+rayon,rectangle->y};
		tete_temp={queue_temp->x+longueur,queue_temp->y};
		Liste=trait(queue_temp,tete_temp,Liste);

		centre_temp={tete_temp->x,tete_temp->y+rayon};
		Liste=arc(centre_temp,rayon,0,90,Liste);

		queue_temp={centre_temp->x+rayon,centre_temp->y};
		tete_temp={queue_temp->x,queue_temp->y+hauteur};
		Liste=trait(queue_temp,tete_temp,Liste);

		centre_temp={tete_temp->x-rayon,tete_temp->y};
		Liste=arc(centre_temp,rayon,-90,0,Liste);

		queue_temp={centre_temp->x,centre_temp->y+rayon};
		tete_temp={queue_temp->x-longueur,queue_temp->y};
		Liste=trait(queue_temp,tete_temp,Liste);

		centre_temp={tete_temp->x,tete_temp->y-rayon};
		Liste=arc(centre_temp,rayon,-180,-90,Liste);

		queue_temp={centre_temp->x-rayon,centre_temp->y};
		tete_temp={queue_temp->x,queue_temp->y-hauteur};
		Liste=trait(queue_temp,tete_temp,Liste);

		centre_temp={tete_temp->x+rayon,tete_temp->y};
		Liste=arc(centre_temp,rayon,-270,-180,Liste);
	}
	else {
		int h = min(longueur/2,largeur/2);
		tete_temp={rectangle->x+h,rectangle->y+h};
		queue_temp={rectangle->x+r-r*cos(-3*Pi/4);rectangle->y+hauteur-r+r*sin(-3*Pi/4)};
		Liste=trait(queue_temp,tete_temp,Liste);

		queue_temp=tete_temp;
		tete_temp={rectangle->x+longueur-h,rectangle->y+h};
		Liste=trait(queue_temp,tete_temp,Liste);

		queue_temp=tete_temp;
		tete_temp={rectangle->x+longueur-r+rcos(Pi/4),rectangle->y+r-r*sin(Pi/4)};
		Liste=trait(queue_temp,tete_temp,Liste);

		if (partie==haute) {
			Liste=arc(




	return Liste;
}








	
