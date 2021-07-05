#include "../screen.h"

#include <stdio.h>

int init_colours()
{
    enum Shade line_shade = BLK;
    enum Colour c[16] = {Black, Red, Green, Yellow, Blue, Magenta, Cyan, White,
                         BBlack, BRed, BGreen, BYellow, BBlue, BMagenta, BCyan, BWhite};
    int width = get_width();

    for (int i = 0; i < 16; i++) {
        draw_line(0, i, width, line_shade, c[i]);
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (init_screen() != 0) {
        fprintf(stderr, "[Error] Failed to initialise screen");
        return 1;
    }

    init_colours();

    while (1) {
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
