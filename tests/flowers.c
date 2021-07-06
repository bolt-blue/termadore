#include "../screen.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265f

int main(void)
{
    srand(time(NULL));
    int w, h, originX, originY;
    int up = 1;
    int n = 1;
    int d = 1;
    double r, a, step;
    double theta;
    double t_rad;
    double phi = 0.0;
    double p_rad;

    if (init_screen() != 0) {
        fprintf(stderr, "[Error] Failed to initialise screen");
        exit(EXIT_FAILURE);
    }

    w = get_width();
    h = get_height();
    originX = w / 2;
    originY = h / 2;
    a = w / 3.0;	/* Initial radius adjustment */
    step = w / 121.0;

    while (1) {
        fill(CLR, Unset);
        theta = 0.0;

        for (int i = 0; i < 1440; i++) {
            theta += i / 4.0;
            t_rad = theta * PI / 180.0;
            p_rad = phi * PI / 180.0;
            r = a * cos(((double) n / d) * (t_rad - p_rad));

            set_pixel(originX + r * cos(t_rad),
                      originY + r * sin(t_rad),
                      BLK, rand() % 16);
        }

        render();

        /* Rotation update */
        phi += 2.0;

        /* Radius update */
        a += up ? step : -step;

        if (a >= w / 2.0) {
            up = 0;
        } else if (a <= 0) {
            up = 1;
            n += 1;

            /* Don't ask */
            if (n > 7) {
                n = 1;
                d += 1;
            }

            if (n == d)
                n += 1;

            if (d > 7)
                d = 1;
        }

        if (detect_resize()) {
            kill_window();
            init_window();

            w = get_width();
            h = get_height();

            originX = w / 2;
            originY = h / 2;
            a = w / 3.0;
            step = w / 121.0;
        }

        if (!kbhit())
            continue;

        char key = fgetc(stdin);
        if (key == 'q')
            break;

    }

    cleanup();

    exit(EXIT_SUCCESS);
}
