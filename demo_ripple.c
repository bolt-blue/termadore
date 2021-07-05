#include "screen.h"

#include <stdlib.h>
#include <math.h>
//#include <string.h>
#include <stdio.h>

#define PI 3.14159265f

// TODO: Give this function a more clear name
void draw_circle(int x_center, int y_center, int x, int y, enum Shade shade, enum Colour colour)
{
    set_pixel(x + x_center, y + y_center, shade, colour);
    set_pixel(-x + x_center, y + y_center, shade, colour);
    set_pixel(x + x_center, -y + y_center, shade, colour);
    set_pixel(-x + x_center, -y + y_center, shade, colour);
    set_pixel(y + x_center, x + y_center, shade, colour);
    set_pixel(-y + x_center, x + y_center, shade, colour);
    set_pixel(y + x_center, -x + y_center, shade, colour);
    set_pixel(-y + x_center, -x + y_center, shade, colour);
}

#define PSZ 8
struct ring {
    int rad;
    enum Shade shd;
};

#define MIN_RING_RAD 2
int init_rings(int *count, struct ring **rings)
{
    int w = get_width();
    int h = get_height();

    int biggest = (sqrt(w * w + h * h) + 0.5) / 2;
    int new_count = biggest - MIN_RING_RAD;

    if (*rings) {
        // TODO: Copy any existing values
        // - careful about whether the bounds are shrinking or expanding
        struct ring *old_rings = *rings;
        int cpy_count = new_count >= *count ? *count : new_count;
        *count = new_count;
        *rings = malloc(*count * sizeof(struct ring));
        for (int i = 0; i < cpy_count; i++) {
            (*rings)[i] = old_rings[i];
        }
        for (int i = cpy_count; i < *count; i++) {
            (*rings)[i].rad = i + MIN_RING_RAD;
            (*rings)[i].shd = CLR;
        }
        free(old_rings);
    } else {
        *count = new_count;
        *rings = malloc(*count * sizeof(struct ring));
        for (int i = 0; i < *count; i++) {
            (*rings)[i].rad = i + MIN_RING_RAD;
            (*rings)[i].shd = CLR;
        }
    }

    return 0;
}

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
        int r = rings[i].rad;
        enum Shade shade = rings[i].shd;

        if (move) {
            rings[i].shd = pattern[i % PSZ];
        }

        int x = 0, y = r;
        int d = PI - 2 * r;
        draw_circle(x_center, y_center, x, y, shade, Blue);

        while (y >= x) {
            x++;

            if (d > 0) {
                y--;
                d += 4 * (x - y) + 10;
            } else {
                d += 4 * x + 6;
            }

            draw_circle(x_center, y_center, x, y, shade, Blue);
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

    int ring_count = 0;
    struct ring *rings = NULL;

    init_rings(&ring_count, &rings);

    while (1) {
        fill(CLR, Unset);
        update(rings, ring_count);

        if (detect_resize()) {
            kill_window();
            init_window();
            init_rings(&ring_count, &rings);
        }

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
