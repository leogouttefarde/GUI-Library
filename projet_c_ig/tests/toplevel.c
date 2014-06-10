
#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"

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

        ei_widget_t*	toplevel;
        ei_size_t	toplevel_size		= {200,200};
        float		toplevel_rel_x		= 0.05;
        float		toplevel_rel_y		= 0.05;
        ei_color_t	toplevel_color		= {0x88, 0x88, 0x88, 0xff};
        char*		toplevel_title		= "Titre";
        ei_bool_t	toplevel_closable                = EI_TRUE;
        ei_axis_set_t	toplevel_resizable               = ei_axis_both;
        int		toplevel_border_width	= 2;
        ei_size_t       *toplevel_min_size;
        toplevel_min_size = malloc(sizeof(ei_size_t));
        *toplevel_min_size = ei_size(50,50);

        ei_widget_t*	toplevel_2;
        ei_anchor_t     anc_2                           = ei_anc_southeast;
        ei_axis_set_t	toplevel_resizable_2               = ei_axis_none;
        float		toplevel_rel_x_2		= 0.95;
        float		toplevel_rel_y_2		= 0.95;

        ei_widget_t*	toplevel_3;
        ei_anchor_t     anc_3                           = ei_anc_southwest;
        ei_axis_set_t	toplevel_resizable_3               = ei_axis_y;
        float		toplevel_rel_x_3		= 0.05;
        float		toplevel_rel_y_3		= 0.95;

        /* Create the application and change the color of the background. */
        ei_app_create(&screen_size, EI_FALSE);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        /* Create, configure and place the toplevel on screen. */
        toplevel = ei_widget_create("toplevel", ei_app_root_widget());
        toplevel_2 = ei_widget_create("toplevel", ei_app_root_widget());
        toplevel_3 = ei_widget_create("toplevel", ei_app_root_widget());

        ei_toplevel_configure   (toplevel, &toplevel_size, &toplevel_color,
                        &toplevel_border_width, &toplevel_title, &toplevel_closable,
                        &toplevel_resizable, &toplevel_min_size);
        ei_toplevel_configure   (toplevel_2, &toplevel_size, &toplevel_color,
                        &toplevel_border_width, &toplevel_title, &toplevel_closable,
                        &toplevel_resizable_2, &toplevel_min_size);
        ei_toplevel_configure   (toplevel_3, &toplevel_size, &toplevel_color,
                        &toplevel_border_width, &toplevel_title, &toplevel_closable,
                        &toplevel_resizable_3, &toplevel_min_size);
        ei_place(toplevel, NULL, NULL, NULL, NULL, NULL, &toplevel_rel_x, &toplevel_rel_y, NULL, NULL );
        ei_place(toplevel_2, &anc_2, NULL, NULL, NULL, NULL, &toplevel_rel_x_2,&toplevel_rel_y_2, NULL, NULL );
        ei_place(toplevel_3, &anc_3, NULL, NULL, NULL, NULL, &toplevel_rel_x_3, &toplevel_rel_y_3, NULL, NULL );

        /* Hook the keypress callback to the event. */
        ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);

        /* Run the application's main loop. */
        ei_app_run();

        /* We just exited from the main loop. Terminate the application (cleanup). */
        ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
        ei_app_free();

        return (EXIT_SUCCESS);
}
