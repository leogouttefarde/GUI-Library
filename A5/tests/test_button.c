#include <stdlib.h>

#include "ei_types.h"
#include "ei_main.h"
#include "ei_draw.h"
#include "hw_interface.h"
#include "ei_button.h"
#include "ei_event.h"

int ei_main(int argc, char** argv) {

	ei_surface_t			main_window		= NULL;
	ei_size_t			main_window_size;
	int				i;
	ei_event_t			event;
	ei_linked_point_t		points[4];
	int				coords[]		= { 20, 20, 620, 20, 20, 460, 620, 460 };
	ei_color_t			red			= { 0xff, 0x00, 0x00, 0xff };
	ei_color_t			transp_blue		= { 0x00, 0x00, 0xff, 0x88 };

	ei_point_t centre;
	ei_point_t queue;
	ei_point_t P4;
	ei_point_t tete;
	ei_linked_point_t* Liste;
	ei_linked_point_t* Liste2;
	ei_linked_point_t* Liste3;

	// Init acces to hardware.
	hw_init();

	// Create the main window.
	main_window_size.width	= 640;
	main_window_size.height	= 480;
	main_window = hw_create_window(&main_window_size, EI_FALSE);
	hw_surface_lock(main_window);

	switch (argv[1][0]) {

	case '1':
		centre.x=200;centre.y=200;
		Liste=ei_button_arc(centre,50,0,360,NULL);
		queue.x=200;queue.y=0;
		tete.x=0;tete.y=200;
		P4.x=200;P4.y=0;
		Liste2=trait(queue,tete,NULL);
		Liste2=trait(tete,centre,Liste2);
		Liste2=trait(centre,P4,Liste2);
		Liste3=trait(P4,queue,Liste2);

		ei_draw_polygon(main_window, Liste, transp_blue, NULL);
		ei_draw_polygon(main_window, Liste2, red, NULL);
		ei_draw_polygon(main_window, Liste3, red, NULL);
		break;

	case '2':
		queue.x=0;queue.y=0;
		centre.x=200;centre.y=200;
		P4.x=200;P4.y=300;
		Liste=trait(queue,centre,NULL);
		Liste=trait(centre,P4,Liste);
		ei_draw_polygon(main_window, Liste, transp_blue, NULL);
		break;
	default:	;
	}

	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);
	event.type = ei_ev_none;
	while (event.type != ei_ev_keydown)
		hw_event_wait_next(&event);

	// Free hardware resources.
	hw_quit();

	return(0);
}
