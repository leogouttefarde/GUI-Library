#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_radiobutton.h"


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
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	radiobutton;
	int		button_x		= 50;
	int		button_y		= 50;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the button on screen. */
	radiobutton = ei_widget_create("radiobutton", ei_app_root_widget());
	ei_place(radiobutton, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Hook the keypress callback to the event. */
	ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);

	/* Run the application's main loop. */
	ei_app_run();

        /* We just exited from the main loop. Terminate the application (cleanup). */
        ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
        ei_app_free();

        return (EXIT_SUCCESS);
}

