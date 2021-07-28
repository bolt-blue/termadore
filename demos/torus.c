/* by camel-cdr - refactored by bolt-blue */
/* cc torus.c -DH=32 -DW=64 -lm */

#include "termadore.h"

#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

float X,Y,Z,l,t,x,y,z,A,S,C;

// NOTE X and Y here are shadowed
void r(float *X, float *Y)
{
    A = *X;
    S = sin(l);
    C = cos(l);

    *X = A * C - *Y * S;
    *Y = A * S + *Y * C;
}

int main(void)
{
    if (init_screen() != 0) {
        fprintf(stderr, "[Error] Failed to initialise screen");
        return 1;
    }

    int w = get_width();
    int h = get_height();
    int k;

    enum Shade palette[9] = {BLK, BLK, DRK, DRK, MID, MID, LGT, LGT, CLR};
    enum Colour colour = BRed;

    while (1) {
        fill(CLR, Unset);

        set_pen(0, 0);

        static float secs_elapsed = 0;
        secs_elapsed += get_dt();

        for (int i = 0; i < h; ++i, set_pen(0, i)) {
            for(int j=0; j < w; ++j, set_pixel_at_pen(palette[k=t], colour)) {
                X = (2. * j - w) / w;
                Y = (2. * i - h) / h;
                Z = 3;

                l = sqrt(X*X + Y*Y + Z);

                X /= l;
                Y /= l;
                Z /= l*l;

                for(t = k = 0; k < 50 && ~(l < .05 || (t += l) > 8 && (t = 8)); ++k) {
                    x = X * t;
                    y = Y * t;
                    z = Z * t - 5;

                    l = secs_elapsed;

                    r(&y, &z);

                    l /= 2;
                    r(&x, &y);

                    x = sqrt(x*x + z*z) - 2.2;
                    l = sqrt(x*x + y*y) - .4;
                }
            }
        }

        if (detect_resize()) {
            kill_window();
            init_window();
            w = get_width();
            h = get_height();
        }

        render();

        if (!kbhit())
            continue;

        char key = fgetc(stdin);
        if (key == 'q')
            break;

        switch (key) {
            case 'r': colour = BRed; break;
            case 'g': colour = BGreen; break;
            case 'b': colour = BBlue; break;
            case 'm': colour = BMagenta; break;
            case 'y': colour = BYellow; break;
            case 'c': colour = BCyan; break;
        }
    }

    cleanup();
    return 0;
}
