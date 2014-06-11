
void pcol(ei_color_t *color)
{
        if (color) {
                int red = color->red;
                int green = color->green;
                int blue = color->blue;
                int alpha = color->alpha;

                // printf("rect %08lX\n\n", rect);

                // printf("left %d\t", left);
                // printf("right %d\n", right);

                // printf("top %d\t", top);
                // printf("bottom %d\n", bottom);

                // printf("width %d\t", w);
                // printf("height %d\n\n\n", h);

                printf("red %d\t", red);
                printf("green %d\t", green);

                printf("blue %d\t", blue);
                printf("alpha %d\n", alpha);
        }
        else
                printf("no col!\n");
}
