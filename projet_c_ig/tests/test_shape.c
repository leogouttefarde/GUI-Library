#include <stdlib.h>

#include "ei_types.h"
#include "ei_main.h"
#include "ei_draw.h"
#include "ei_event.h"
#include "hw_interface.h"

#include "ei_shape.h"
int ei_main(int argc, char** argv)
{
	ei_surface_t			main_window		= NULL;
	ei_size_t			main_window_size;
	ei_event_t			event;
	ei_color_t			red			= { 0xff, 0x00, 0x00, 0xff };
	ei_color_t			transp_blue		= { 0x00, 0x00, 0xff, 0x88 };

	// Init acces to hardware.
	hw_init();

	// Create the main window.
	main_window_size.width	= 640;
	main_window_size.height	= 480;
	main_window = hw_create_window(&main_window_size, EI_FALSE);

        // On definit un rectangle
        ei_rect_t rect = {{10,10}, {50,10}};

        // Conversion rectangle/liste de point
        ei_linked_point_t pts;
        ei_linked_point_t pts1;
        ei_linked_point_t pts2;
        ei_linked_point_t pts3;
        ei_point_t max;
        ei_point_t min;
        // test de ei_rect_to_points(rect);
        pts = ei_rect_to_points(rect);
        pts1 = ei_rect_to_points(rect);
        pts2 = ei_rect_to_points(rect);
        pts3 = ei_rect_to_points(rect);

        // Lock the surface for drawing, fill, draw polygon, unlock, update screen.
        hw_surface_lock(main_window);
        ei_fill(main_window, &red, NULL);
        ei_draw_polygon(main_window, &pts, transp_blue, NULL);
        // max, min
        max = ei_search_max(pts1);
        min = ei_search_min(pts1);
        pts1 = (ei_linked_point_t){max, NULL};
        ei_direct_append(&pts1, min);
        ei_direct_append(pts1.next,(ei_point_t){0,0});
        ei_draw_polygon(main_window, &pts1, red, NULL);
        //translation
        ei_translate(&pts2, (ei_point_t){10,30});
        ei_draw_polygon(main_window, &pts2, transp_blue, NULL);
        // append
        ei_direct_append(&pts3, (ei_point_t){0,00});
        //ei_draw_polygon(main_window, &pts3, transp_blue, NULL);
        // symetrie horizontale
        ei_sym_horiz(&pts3);
        ei_draw_polygon(main_window, &pts3, transp_blue, NULL);

        hw_surface_unlock(main_window);
        hw_surface_update_rects(main_window, NULL);



        // Wait for a key press.
        event.type = ei_ev_none;
        while (event.type != ei_ev_keydown)
                hw_event_wait_next(&event);

        // Free hardware resources.
        hw_quit();

        // Terminate program with no error code.
        return 0;
}
