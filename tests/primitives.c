#include <stdio.h>

#include "screen.h"

// TODO: Add error handling

struct line {
    int x1; int y1;
    int x2; int y2;
};

/*
 * Rotate line l by r degrees
 * NOTE: Origin is considered as l.x1, l.y1
 */
struct line rotate_line(struct line l, float r)
{
    return l;
}

int main(int argc, char **argv)
{
    if (init_screen() != 0) {
        fprintf(stderr, "[Error] Failed to initialise screen");
        return 1;
    }

    fill(CLR, Unset);

    // Spinner
    struct line l = {.x1 = get_width() / 2, .y1 = get_height() / 2,
                     .x2 = get_width() / 2, .y2 = 0};
    //draw_line(l.x1, l.y1, l.x2, l.y2, DRK, Blue);

    draw_line(0, 0, 0, get_height(), BLK, Green);
    draw_line(1, 0, 1, get_height() - 1, BLK, Green);
    draw_line(get_width() - 1, 2, get_width() - 1, get_height(), BLK, Green);

    draw_line(2, 0, get_width(), 0, BLK, Blue);
    draw_line(2, 1, get_width() - 1, 1, BLK, Blue);
    draw_line(2, get_height() - 1, get_width() - 1, get_height() - 1, BLK, Blue);

    draw_line(5, 7, 7, 7, BLK, Magenta);

    draw_line(5, 5, get_width() - 5, get_height() - 5, BLK, Red);
    draw_line(get_width() - 6, 5, 4, get_height() - 5, BLK, Cyan);

    set_pixel(5, 5, BLK, Cyan);
    set_pixel(get_width() - 6, get_height() - 6, BLK, Cyan);
    set_pixel(5, get_height() - 6, BLK, Red);
    set_pixel(get_width() - 6, 5, BLK, Red);

    draw_rect(10, get_height() / 2 - 4, 18, 9, BLK, CLR, White, Unset);

    draw_elipse(10, get_height() / 2 - 4, 18, 9, BLK, CLR, BRed, Unset);
    draw_elipse(1, 1, get_width() - 1, get_height() - 1, BLK, CLR, BBlue, Unset);

    while (1) {
        // NOTE: This is where real work would be done, prior to rendering
        if (detect_resize()) {
            kill_window();
            init_window();
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
