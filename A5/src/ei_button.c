
#include "ei_button.h"


ei_linked_point_t* ei_button_arc(ei_point_t centre,int rayon,int angle_debut,int angle_fin) {
	ei_linked_point_t* Point_suivant=NULL;
	for (int theta=angle_fin; theta<=angle_debut; theta --) {
		ei_linked_point* Point=malloc(sizeof(ei_linked_point));
		Point->x=centre+rayon*cos(theta);
		Point->y=centre+rayon*sin(theta);
		Point->next=Point_suivant;
		Point_suivant=Point;
	}
}
