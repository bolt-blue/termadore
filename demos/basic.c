#include <stdio.h>

#include "termadore.h"

// TODO: Add error handling
int init_stripes()
{
    int width = get_width();
    int height = get_height();
    shade(MID);
    colour(Unset);
    enum Shade line_shade = MID;
    enum Colour col = Unset;

    for (int i = 0; i < height; i++) {
        line(0, i, width, i);
        line_shade = line_shade == MID ? LGT : MID;
        shade(line_shade);
    }

    set_pen(get_width() - 3, get_height() - 1);
    set_pixel_at_pen(DRK, col);
    set_pixel_at_pen(LGT, col);
    set_pixel_at_pen(DRK, col);
    set_pixel_at_pen(LGT, col);
    set_pixel_at_pen(DRK, col);
    set_pixel_at_pen(LGT, col);

    set_pen(0,0);
    set_pixel_at_pen(CLR, col);
    set_pen(get_width() - 1, get_height() - 1);
    set_pixel_at_pen(CLR, col);

    return 0;
}

int main(int argc, char **argv)
{
    if (init_screen() != 0) {
        fprintf(stderr, "[Error] Failed to initialise screen");
        return 1;
    }

    // Stripes
    init_stripes();

    while (1) {
        // NOTE: This is where real work would be done, prior to rendering
        if (detect_resize()) {
            kill_window();
            init_window();
            init_stripes();
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
