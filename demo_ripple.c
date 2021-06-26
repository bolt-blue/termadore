#include "screen.h"

#include <math.h>
//#include <string.h>
#include <stdio.h>

#define PI 3.14159265f

// TODO: Give this function a more clear name
void draw_circle(int x_center, int y_center, int x, int y, enum Shade shade)
{
    set_pixel_xy(x + x_center, y + y_center, shade);
    set_pixel_xy(-x + x_center, y + y_center, shade);
    set_pixel_xy(x + x_center, -y + y_center, shade);
    set_pixel_xy(-x + x_center, -y + y_center, shade);
    set_pixel_xy(y + x_center, x + y_center, shade);
    set_pixel_xy(-y + x_center, x + y_center, shade);
    set_pixel_xy(y + x_center, -x + y_center, shade);
    set_pixel_xy(-y + x_center, -x + y_center, shade);
}

#define PSZ 8
struct ring {
    int rad;
    enum Shade shd;
};
/*
 * Using Bresenham’s algorithm to draw circles.
 */
void update(struct ring *rings, int sz)
{
    // TODO:
    // - Add colour

    static float acc = 0;
    int move = 0;

    acc += 100 * get_dt();
    if (acc > 12) {
        acc = 0;
        move = 1;
    }

    static enum Shade pattern[PSZ] = {DRK, MID, CLR, LGT, MID, DRK, BLK, BLK};
    if (move) {
        enum Shade tmp = pattern[PSZ - 1];
        for (int i = PSZ - 1; i > 0; i--) {
            pattern[i] = pattern[i - 1];
        }
        pattern[0] = tmp;
    }

    const int x_center = get_width() / 2;
    const int y_center = get_height() / 2;

    for (int i = 0; i < sz; i++) {
        int r = rings[i].rad;// * sin(acc * (PI / 180));
        enum Shade shade = rings[i].shd;

        if (move) {
            rings[i].shd = pattern[i % PSZ];
        }

        int x = 0, y = r;
        int d = PI - 2 * r;
        draw_circle(x_center, y_center, x, y, shade);

        while (y >= x) {
            x++;

            if (d > 0) {
                y--;
                d += 4 * (x - y) + 10;
            } else {
                d += 4 * x + 6;
            }

            draw_circle(x_center, y_center, x, y, shade);
        }
    }

    //char scratch[16] = {0};
    //sprintf(scratch, "[%.6f] ", acc);
    //write_string(scratch, strlen(scratch), 0, 1);
}

int main(int argc, char **argv)
{
    if (init_screen() != 0) {
        fprintf(stderr, "[Error] Failed to initialise screen");
        return 1;
    }

    int w = get_width();
    int h = get_height();

    int biggest = (sqrt(w * w + h * h) + 0.5) / 2;
    int ring_count = biggest - 3;
    struct ring rings[ring_count];
    for (int i = 0; i < ring_count; i++) {
        rings[i].rad = i + 2;
        rings[i].shd = CLR;
    }

    while (1) {
        fill(CLR);
        update(rings, ring_count);
        render();

        if (!kbhit())
            continue;

        char key = fgetc(stdin);
        if (key == 'q')
            break;
    }

    cleanup();

    return 0;
}
