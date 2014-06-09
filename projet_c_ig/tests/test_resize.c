#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"

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

/* Fonction de redimensionnement
 * Conserve les proportions des fils */
void test_resize(ei_widget_t *widget, ei_size_t add_size){

        // Tous les ancrages sont nw
        // Car le redimensionnement vient du se
        ei_anchor_t anc = ei_anc_northwest;

        int w_w;
        int w_h;


        // Taille du widget
        w_w = widget->screen_location.size.width;
        w_h = widget->screen_location.size.height;
        w_w = w_w + add_size.width;
        w_h = w_h + add_size.height;

        ei_place(widget, &anc, &widget->screen_location.top_left.x, &widget->screen_location.top_left.y, &w_w, &w_h, NULL, NULL, NULL,
                        NULL);

        // Parcours des enfants et redessin
        ei_widget_t *current;
        current = widget->children_head;
        while(current){
                resize(current, add_size);
                current = current->next_sibling;
        }
}

/* Fonction de mouvement
 * Deplacement brut */
void test_move(ei_widget_t *widget, ei_size_t dist){


        ei_anchor_t anc = ei_anc_northwest;

        int x;
        int y;

        // Position top_left et taille du pere
        int p_x = widget->screen_location.top_left.x;
        int p_y = widget->screen_location.top_left.y;

        // Nouveau x absolu
        x = p_x + dist.width;
        y = p_y + dist.height;

        // On deplace le pere
        ei_place(widget, &anc, &x, &y, NULL, NULL, NULL,
                        NULL, NULL, NULL);

        // On deplace ses fils
        ei_widget_t *current;
        current = widget->children_head;

        // Parcours des fils et des freres
        while(current){
                move(current, dist);
                current = current->next_sibling;
        }
}
/*
 * ei_main --
 *
 *	Test l'imbrication de widgets
 */
int ei_main(int argc, char** argv)
{
        ei_size_t	screen_size		= {500, 500};
        ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

        ei_widget_t*	button;
        ei_size_t	button_size		= {50,50};
        int		button_x		= 0;
        int		button_y		= 0;
        ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
        char*		button_title		= "clic !";
        ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
        int		button_corner_radius	= 0;
        ei_relief_t	button_relief		= ei_relief_raised;
        int		button_border_width	= 1;
        ei_callback_t	button_callback 	= button_press;


        ei_widget_t*	frame;
        ei_size_t	frame_size		= {100,100};
        int		frame_x			= 0;
        int		frame_y			= 0;
        ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
        ei_relief_t	frame_relief		= ei_relief_raised;
        int		frame_border_width	= 1;

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


        ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );
        ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

        test_move(frame, ei_size(10,10));
        test_resize(frame, ei_size(10,50));

        /* Hook the keypress callback to the event. */
        ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);

        /* Run the application's main loop. */
        ei_app_run();

        /* We just exited from the main loop. Terminate the application (cleanup). */
        ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
        ei_app_free();

        return (EXIT_SUCCESS);
}
