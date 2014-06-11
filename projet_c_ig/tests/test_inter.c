
#include "ei_types.h"
#include "ei_main.h"
#include "ei_draw.h"
#include "ei_event.h"
#include "ei_common.h"
#include "hw_interface.h"




ei_rect_t* rect_intersection(const ei_rect_t *rect1, const ei_rect_t *rect2) 
{
        ei_rect_t *inter = NULL;
        // printf("calc inter entre\n\n");
        // print_rect(rect1);
        // printf("\net\n\n");
        // print_rect(rect2);

        if (rect1 && rect2) {
                int x1 = rect1->top_left.x;
                int y1 = rect1->top_left.y;

                int w1 = rect1->size.width;
                int h1 = rect1->size.height;


                int x2 = rect2->top_left.x;
                int y2 = rect2->top_left.y;

                int w2 = rect2->size.width;
                int h2 = rect2->size.height;



                int r2_left = x2;
                int r2_top = y2;
                int r2_right = x2 + w2;
                int r2_bottom = y2 + h2;

                int r1_left = x1;
                int r1_top = y1;
                int r1_right = x1 + w1;
                int r1_bottom = y1 + h1;


                bool is_intersection =  ( r2_left < r1_right 
                                        && r2_right > r1_left 
                                        && r2_top < r1_bottom 
                                        && r2_bottom > r1_top 
                                        );

                if(is_intersection)
                {
                        //printf("r1 coupe r2 !!!!!!!!!!!!!!!\n");
                        inter = CALLOC_TYPE(ei_rect_t);
                        assert(inter);

                        if (inter) {
                                int left = MAX(r1_left, r2_left);
                                int top = MAX(r1_top, r2_top);

                                inter->top_left.x = MAX(r1_left, r2_left);
                                inter->top_left.y = MAX(r1_top, r2_top);

                                inter->size.width = MIN( r1_right, r2_right) - left;
                                inter->size.height = MIN(r1_bottom, r2_bottom) - top;
                        }
                }
                else
                       printf("NO INTER\n");
        }
        // printf("\n\nRésultat\n\n");
        // print_rect(inter);

        return inter;
}



int ei_main(int argc, char** argv)
{
        ei_surface_t                    main_window             = NULL;
        ei_size_t                       main_window_size;
        ei_event_t                      event;
        int                             i;
        ei_linked_point_t               points[4];
        int                             coords[]                = { 20, 20, 620, 20, 20, 460, 620, 460 };
        ei_color_t                      red                     = { 0xff, 0x00, 0x00, 0xff };
        ei_color_t                      transp_blue             = { 0x00, 0x00, 0xff, 0x88 };

        // Init acces to hardware.
        hw_init();

        // Create the main window.
        main_window_size.width  = 640;
        main_window_size.height = 480;
        main_window = hw_create_window(&main_window_size, EI_FALSE);



        // Fill the main window in red, draw a transparent blue polygon:

        // Define the polygon vertices.
        for (i = 0; i < 4; i++) {
                points[i].point.x       = coords[i*2];
                points[i].point.y       = coords[i*2 + 1];
                if (i < 3)
                        points[i].next  = &(points[i+1]);
                else
                        points[i].next  = NULL;
        }

        // Lock the surface for drawing, fill, draw polygon, unlock, update screen.
        hw_surface_lock(main_window);

        ei_fill(main_window, &red, NULL);

        ei_color_t                      blue             = { 0x00, 0x00, 0xff, 0xff };
        ei_color_t                      green                     = { 0x00, 0xff, 0x00, 0xff };
        ei_color_t                      sss                     = { 0xff, 0xff, 0x00, 0xff };

        ei_rect_t clipper1 = {{20, 20}, {200, 200}};
        ei_fill(main_window, &blue, &clipper1);

        ei_rect_t clipper2 = {{150, 150}, {200, 200}};
        ei_fill(main_window, &green, &clipper2);

        ei_rect_t *clipper3 = rect_intersection(&clipper1, &clipper2);
        ei_fill(main_window, &sss, clipper3);



        ei_rect_t clipper11 = {{300, 300}, {200, 200}};
        ei_fill(main_window, &blue, &clipper11);

        clipper3 = rect_intersection(&clipper11, &clipper2);
        ei_fill(main_window, &sss, clipper3);



        ei_rect_t clipper111 = {{300, 20}, {200, 200}};
        ei_fill(main_window, &blue, &clipper111);

        clipper3 = rect_intersection(&clipper111, &clipper2);
        ei_fill(main_window, &sss, clipper3);



        ei_rect_t clipper1111 = {{20, 300}, {200, 200}};
        ei_fill(main_window, &blue, &clipper1111);

        clipper3 = rect_intersection(&clipper1111, &clipper2);
        ei_fill(main_window, &sss, clipper3);


        ei_rect_t clipper11115 = {{240, 240}, {50, 50}};
        ei_fill(main_window, &blue, &clipper11115);

        clipper3 = rect_intersection(&clipper11115, &clipper2);
        ei_fill(main_window, &sss, clipper3);


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
