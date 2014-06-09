#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

// Test du palcer avec coordonees relatives

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
        float	        button_rel_width	= 0.20;
        float           button_rel_height       = 0.10;
        float		button_rel_x		= 0.0;
        float           button_rel_y		= 0.20;
        ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
        char*		button_title		= "clic !";
        ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
        int		button_corner_radius	= 5;
        ei_relief_t	button_relief		= ei_relief_raised;
        int		button_border_width	= 2;
        ei_callback_t	button_callback 	= button_press;


        ei_widget_t*	frame;
        ei_size_t	frame_size		= {300,300};
        float           frame_rel_width         = 0.50;
        float           frame_rel_height        = 0.50;
        float		frame_rel_x		= 0.0;
        float		frame_rel_y		= 0.10;
        int		frame_x			= 100;
        int		frame_y			= 100;
        ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
        ei_relief_t	frame_relief		= ei_relief_raised;
        int		frame_border_width	= 6;

        /* Create the application and change the color of the background. */
        ei_app_create(&screen_size, EI_FALSE);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        /* Create, configure and place the button on screen. */
        frame = ei_widget_create("frame", ei_app_root_widget());
        button = ei_widget_create("button", frame);

        ei_frame_configure(frame, &frame_size, &frame_color,
                        &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL);


        ei_button_configure(button, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, 
                        &button_title, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);


        ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, &frame_rel_x, &frame_rel_y, &frame_rel_width, &frame_rel_height);
        ei_place(button, NULL, NULL, NULL, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_width, &button_rel_height);

        /* Hook the keypress callback to the event. */
        ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);

        /* Run the application's main loop. */
        ei_app_run();

        /* We just exited from the main loop. Terminate the application (cleanup). */
        ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
        ei_app_free();

        return (EXIT_SUCCESS);
}
