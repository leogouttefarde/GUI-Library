#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	printf("Click !\n");
	return EI_TRUE;
}

/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_sym == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
	}
	
	return EI_FALSE;
}

int ei_main(int argc, char** argv)
{
	printf("la c'est bon qd meme\n");
	printf("la c'est bon qd meme\n");
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	button;
	ei_widget_t*	button_A;
	ei_widget_t*	button_B;
	ei_widget_t*	button_start;
	ei_widget_t*	button_select;

	ei_size_t	button_rond_size		= {20,20};
	int		button_A_x		= 180;
	int		button_A_y		= 210;
	int		button_B_x		= 160;
	int		button_B_y		= 215;

	ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
	char*		button_A_title		= "A";
	char*		button_B_title		= "B";
	char* image_name ="misc/klimt.jpg";

	ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
	int		button_corner_radius	= 10;
	ei_relief_t	button_relief		= ei_relief_raised;
	int		button_border_width	= 3;
	ei_callback_t	button_callback 	= button_press;
	printf("ici ok\n");
	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	ei_surface_t img=hw_image_load(image_name,ei_app_root_surface());
	printf("ok ici numero 0\n");

/* Create, configure and place the button on screen. */
	ei_size_t	button_size		= {200,200};
	int		button_x		= 150;
	int		button_y		= 200;
	ei_rect_t* rec=malloc(sizeof(ei_rect_t));
	rec->top_left.x=600;rec->top_left.y=600;
	rec->size.width=1000;rec->size.height=1000;
	printf("ok ici");
	button = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button, &button_size, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief,NULL, NULL, &button_text_color, NULL,
				 &img, &rec, NULL, &button_callback, NULL);
	assert(button!=NULL);
	ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Create, configure and place the button on screen. */
	//button_A = ei_widget_create("button", ei_app_root_widget());
	//button_B = ei_widget_create("button", ei_app_root_widget());

	//ei_button_configure(button_A, &button_rond_size, &button_color,&button_border_width, &button_corner_radius, &button_relief, &button_A_title, NULL, &button_text_color, NULL,NULL, NULL, NULL, &button_callback, NULL);
	//ei_button_configure	(button_B, &button_rond_size, &button_color,&button_border_width, &button_corner_radius,&button_relief, &button_B_title, NULL, &button_text_color, NULL,NULL, NULL, NULL, &button_callback, NULL);

	//ei_place(button_A, NULL, &button_A_x, &button_A_y, NULL, NULL, NULL, NULL, NULL, NULL );
	//ei_place(button_B, NULL, &button_B_x, &button_B_y, NULL, NULL, NULL, NULL, NULL, NULL );

		/* Hook the keypress callback to the event. */
	ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);

	/* Run the application's main loop. */
	printf("avant le app_run ok\n");
	ei_app_run();

        /* We just exited from the main loop. Terminate the application (cleanup). */
        ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
        ei_app_free();

        return (EXIT_SUCCESS);
}

