#include <stdio.h>
#include <stdlib.h>

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

/*
 * ei_main --
 *
 *	Test l'imbrication de widgets
 */
int ei_main(int argc, char** argv)
{
        ei_size_t	screen_size		= {600, 600};
        ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

        ei_widget_t*	button;
        ei_size_t	button_size		= {100,50};
        int		button_x		= 10;
        int		button_y		= 10;
        ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
        char*		button_title		= "clic !";
        ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
        int		button_corner_radius	= 5;
        ei_relief_t	button_relief		= ei_relief_raised;
        int		button_border_width	= 2;
        ei_callback_t	button_callback 	= button_press;

        ei_widget_t*	button_2;
        ei_size_t	button_size_2		= {100,50};
        int		button_x_2		= 10;
        int		button_y_2		= 100;
        ei_color_t	button_color_2		= {0x88, 0x88, 0x88, 0xff};
        char*		button_title_2		= "clic 2 !";
        ei_color_t	button_text_color_2	= {0x00, 0x00, 0x00, 0xff};
        int		button_corner_radius_2	= 5;
        ei_relief_t	button_relief_2		= ei_relief_raised;
        int		button_border_width_2	= 2;
        ei_callback_t	button_callback_2 	= button_press;

        ei_widget_t*	frame;
        ei_size_t	frame_size		= {300,300};
        int		frame_x			= 100;
        int		frame_y			= 100;
        ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
        ei_relief_t	frame_relief		= ei_relief_raised;
        int		frame_border_width	= 6;

        ei_widget_t*	frame_2;
        ei_size_t	frame_size_2		= {300,150};
        int		frame_x_2		= 100;
        int		frame_y_2		= 0;
        ei_color_t	frame_color_2		= {0x88, 0x88, 0x88, 0xff};
        ei_relief_t	frame_relief_2		= ei_relief_sunken;
        int		frame_border_width_2	= 3;
        /* Create the application and change the color of the background. */
        ei_app_create(&screen_size, EI_FALSE);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        /* Create, configure and place the button on screen. */
        frame = ei_widget_create("frame", ei_app_root_widget());
        frame_2 = ei_widget_create("frame", frame);
        button = ei_widget_create("button", frame);
        button_2 = ei_widget_create("button", frame_2);

        ei_frame_configure(frame, &frame_size, &frame_color,
                        &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL);

        ei_frame_configure(frame_2, &frame_size_2, &frame_color_2,
                        &frame_border_width_2, &frame_relief_2, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL);

        ei_button_configure(button, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, 
                        &button_title, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);

        ei_button_configure(button_2, &button_size_2, &button_color_2,
                        &button_border_width_2, &button_corner_radius_2, &button_relief_2, 
                        &button_title_2, NULL, &button_text_color_2, NULL,
                        NULL, NULL, NULL, &button_callback_2, NULL);

        ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );
        ei_place(frame_2, NULL, &frame_x_2, &frame_y_2, NULL, NULL, NULL, NULL, NULL, NULL );
        ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );
        ei_place(button_2, NULL, &button_x_2, &button_y_2, NULL, NULL, NULL, NULL, NULL, NULL );

        /* Hook the keypress callback to the event. */
        ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);

        /* Run the application's main loop. */
        ei_app_run();

        /* We just exited from the main loop. Terminate the application (cleanup). */
        ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
        ei_app_free();

        return (EXIT_SUCCESS);
}
