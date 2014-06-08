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

        ei_anchor_t c = ei_anc_center;
        ei_anchor_t nw = ei_anc_northwest;
        ei_anchor_t n = ei_anc_north;
        ei_anchor_t ne = ei_anc_northeast;
        ei_anchor_t e = ei_anc_east;
        ei_anchor_t se = ei_anc_southeast;
        ei_anchor_t s = ei_anc_south;
        ei_anchor_t sw = ei_anc_southwest;
        ei_anchor_t w = ei_anc_west;

        ei_widget_t*	button_c;
        ei_widget_t*	button_nw;
        ei_widget_t*	button_n;
        ei_widget_t*	button_ne;
        ei_widget_t*	button_e;
        ei_widget_t*	button_se;
        ei_widget_t*	button_s;
        ei_widget_t*	button_sw;
        ei_widget_t*	button_w;
        ei_size_t	button_size		= {100,50};
        float	        button_rel_width	= 0.2;
        float           button_rel_height       = 0.2;
        float		button_rel_x		= 0.3;
        float           button_rel_y		= 0.5;
        float		button_rel_x_2		= 0.7;
        float           button_rel_y_2		= 0.5;
        ei_color_t	button_color		= {0xFF, 0xAA, 0x22, 0xff};
        char*		button_title_c		= "C";
        char*		button_title_nw		= "NW";
        char*		button_title_n		= "N";
        char*		button_title_ne		= "NE";
        char*		button_title_e		= "E";
        char*		button_title_se		= "SE";
        char*		button_title_s		= "S";
        char*		button_title_sw		= "SW";
        char*		button_title_w		= "W";
        ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
        int		button_corner_radius	= 3;
        ei_relief_t	button_relief		= ei_relief_raised;
        int		button_border_width	= 2;
        ei_callback_t	button_callback 	= button_press;


        ei_widget_t*	frame;
        ei_size_t	frame_size		= {300,300};
        float           frame_rel_width         = 0.80;
        float           frame_rel_height        = 0.80;
        float		frame_rel_x		= 0.05;
        float		frame_rel_y		= 0.05;
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
        button_c = ei_widget_create("button", frame);
        button_nw = ei_widget_create("button", frame);
        button_n = ei_widget_create("button", frame);
        button_ne = ei_widget_create("button", frame);
        button_e = ei_widget_create("button", frame);
        button_se = ei_widget_create("button", frame);
        button_s = ei_widget_create("button", frame);
        button_sw = ei_widget_create("button", frame);
        button_w = ei_widget_create("button", frame);

        ei_frame_configure(frame, &frame_size, &frame_color,
                        &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL);


        ei_button_configure(button_c, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, 
                        &button_title_c, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(button_nw, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, 
                        &button_title_nw, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(button_n, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, 
                        &button_title_n, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(button_ne, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, 
                        &button_title_ne, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(button_e, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, 
                        &button_title_e, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(button_se, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, 
                        &button_title_se, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);

        ei_button_configure(button_s, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, 
                        &button_title_s, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(button_sw, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, 
                        &button_title_sw, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(button_w, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, 
                        &button_title_w, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);

        ei_place(frame, &c, &frame_x, &frame_y, NULL, NULL, &frame_rel_x, &frame_rel_y, &frame_rel_width, &frame_rel_height);
        ei_place(button_c, &c, NULL, NULL, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_width, &button_rel_height);

        ei_place(button_nw, &nw, NULL, NULL, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_width, &button_rel_height);
        ei_place(button_n, &n, NULL, NULL, NULL, NULL, &button_rel_x_2, &button_rel_y_2, &button_rel_width, &button_rel_height);
        ei_place(button_ne, &ne, NULL, NULL, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_width, &button_rel_height);
        ei_place(button_e, &e, NULL, NULL, NULL, NULL, &button_rel_x_2, &button_rel_y_2, &button_rel_width, &button_rel_height);
        ei_place(button_se, &se, NULL, NULL, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_width, &button_rel_height);
        ei_place(button_s, &s, NULL, NULL, NULL, NULL, &button_rel_x_2, &button_rel_y_2, &button_rel_width, &button_rel_height);
        ei_place(button_sw, &sw, NULL, NULL, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_width, &button_rel_height);
        ei_place(button_w, &w, NULL, NULL, NULL, NULL, &button_rel_x_2, &button_rel_y_2, &button_rel_width, &button_rel_height);

        /* Hook the keypress callback to the event. */
        ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);

        /* Run the application's main loop. */
        ei_app_run();

        /* We just exited from the main loop. Terminate the application (cleanup). */
        ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
        ei_app_free();

        return (EXIT_SUCCESS);
}
