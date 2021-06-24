#include "screen.c"

#include <math.h>

#define PI 3.14159265f

void draw_circle(int x_center, int y_center, int x, int y, enum Shade shade)
{
    set_pixel(x + x_center, y + y_center, shade);
    set_pixel(-x + x_center, y + y_center, shade);
    set_pixel(x + x_center, -y + y_center, shade);
    set_pixel(-x + x_center, -y + y_center, shade);
    set_pixel(y + x_center, x + y_center, shade);
    set_pixel(-y + x_center, x + y_center, shade);
    set_pixel(y + x_center, -x + y_center, shade);
    set_pixel(-y + x_center, -x + y_center, shade);
}

/*
 * Using Bresenham’s algorithm to draw circles.
 */
void update()
{
    // TODO:
    // - Convert to ripple effect
    // - Add colour

    enum Shade shade = MID;

    const int x_center = g_screen.w / 2;
    const int y_center = g_screen.h / 2;

    static float acc = 0;

    for (int i = 3; i < x_center; i += 3) {
        int r = i * sin(acc * (PI / 180));

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

    acc += 100 * get_dt();
    if (acc >= 360) acc = 0;
    //fprintf(stderr, "[%.6f] ", acc);
}

int main(int argc, char **argv)
{
    if (init_screen() != 0) {
        fprintf(stderr, "[Error] Failed to initialise screen");
        return 1;
    }

    while (1) {
        fill(CLR);
        update();
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
