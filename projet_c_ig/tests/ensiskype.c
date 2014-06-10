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

        // Fenetre
        ei_widget_t*	toplevel;
        ei_size_t	toplevel_size		= {500,500};
        float		toplevel_rel_x		= 0.05;
        float		toplevel_rel_y		= 0.05;
        ei_color_t	toplevel_color		= {0x88, 0x88, 0x88, 0xff};
        char*		toplevel_title		= "Skype";
        ei_bool_t	toplevel_closable                = EI_TRUE;
        ei_axis_set_t	toplevel_resizable               = ei_axis_both;
        int		toplevel_border_width	= 2;
        ei_size_t       *toplevel_min_size;
        toplevel_min_size = malloc(sizeof(ei_size_t));
        *toplevel_min_size = ei_size(50,50);

        // Liste de contact - frame
        ei_widget_t*	contacts;
        ei_anchor_t     contacts_anc                           = ei_anc_northwest;
        ei_size_t	contacts_size	            = {100,100};
        int		contacts_x		    = 0;
        int		contacts_y		    = 0;
        int             contacts_w                  = 100;
        float           contacts_rel_h              = 1.0;
        ei_color_t	contacts_color		    = {0x66, 0xFF, 0x33, 0xff};
        ei_relief_t	contacts_relief		    = ei_relief_raised;
        int		contacts_border_width	    = 2;
        char*           contacts_text               = "Vous avez 2 amis";
        ei_font_t       contacts_text_font          = ei_default_font;
        ei_anchor_t     contacts_text_anchor        = ei_anc_southwest;
        ei_color_t      contacts_text_color         = {0x00, 0x22, 0x00, 0xff};

        // Zone de texte - frame
        ei_widget_t*	zone;
        ei_anchor_t     zone_anc                = ei_anc_southeast;
        ei_size_t	zone_size		= {100,100};
        float		zone_rel_x		= 1.0;
        float           zone_rel_y              = 1.0;
        float           zone_rel_w              = 4./5.;
        //int             zone_h                  =   50;
        ei_color_t	zone_color		= {0xFF, 0xFF, 0xFF, 0xff};
        ei_relief_t	zone_relief		= ei_relief_sunken;
        int		zone_border_width	= 0;
        char*           zone_text               = "Tapez votre texte ici";
        ei_font_t       zone_text_font          = ei_default_font;
        ei_anchor_t     zone_text_anchor        = ei_anc_center;
        ei_color_t      zone_text_color         = {0x00, 0x22, 0x00, 0xff};

        // Zone de conversation - toplevel
        ei_widget_t*	conversation;
        ei_anchor_t      conversation_anc                = ei_anc_northeast;
        ei_size_t	conversation_size		= {350,350};
        float		conversation_rel_x		= 1.0;
        float		conversation_rel_y		= 0.0;
        ei_color_t	conversation_color		= {0x88, 0x88, 0x88, 0xff};
        char*		conversation_title		= "Conversation";
        ei_bool_t	conversation_closable                = EI_TRUE;
        ei_axis_set_t	conversation_resizable               = ei_axis_x;
        int		conversation_border_width	= 5;
        ei_size_t       *conversation_min_size;
        conversation_min_size = malloc(sizeof(ei_size_t));
        *conversation_min_size = ei_size(50,50);

        /* Create the application and change the color of the background. */
        ei_app_create(&screen_size, EI_FALSE);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        /* Create, configure and place the toplevel on screen. */
        toplevel = ei_widget_create("toplevel", ei_app_root_widget());
        contacts = ei_widget_create("frame", toplevel);
        zone = ei_widget_create("frame", toplevel);
        conversation = ei_widget_create("toplevel", toplevel);

        //toplevel
        ei_toplevel_configure   (toplevel, &toplevel_size, &toplevel_color,
                        &toplevel_border_width, &toplevel_title, &toplevel_closable,
                        &toplevel_resizable, &toplevel_min_size);
        //conversation
        ei_toplevel_configure   (conversation, 
                        &conversation_size, &conversation_color, 
                        &conversation_border_width, 
                        &conversation_title, 
                        &conversation_closable,
                        &conversation_resizable, 
                        &conversation_min_size);

        //zone de texte
        ei_frame_configure(zone, &zone_size, 
                        &zone_color, &zone_border_width, 
                        &zone_relief, &zone_text, 
                        &zone_text_font, &zone_text_color, 
                        &zone_text_anchor, NULL, NULL, NULL);

        //contacts
        ei_frame_configure(contacts, &contacts_size, 
                        &contacts_color, &contacts_border_width, 
                        &contacts_relief, &contacts_text, 
                        &contacts_text_font, &contacts_text_color, 
                        &contacts_text_anchor, NULL, NULL, NULL);

        /* PLACEMENT */
        ei_place(toplevel, NULL, NULL, NULL, NULL, NULL, &toplevel_rel_x, &toplevel_rel_y, NULL, NULL);
        ei_place(conversation, &conversation_anc, NULL, NULL, NULL, NULL, &conversation_rel_x, &conversation_rel_y, NULL, NULL);
        ei_place(zone, &zone_anc, NULL, NULL, NULL, NULL, &zone_rel_x,&zone_rel_y, &zone_rel_w, NULL);
        ei_place(contacts, &contacts_anc, &contacts_x, &contacts_y, &contacts_w, NULL, NULL, NULL, NULL, &contacts_rel_h);

        /* Hook the keypress callback to the event. */
        ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);

        /* Run the application's main loop. */
        ei_app_run();

        /* We just exited from the main loop. Terminate the application (cleanup). */
        ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
        ei_app_free();

        return (EXIT_SUCCESS);
}
