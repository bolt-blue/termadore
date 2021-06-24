#include "screen.c"

int main(int argc, char **argv)
{
    if (init_screen() != 0) {
        fprintf(stderr, "[Error] Failed to initialise screen");
        return 1;
    }

    // Solid fill
    //fill(MID);

    // Stripes
    int width = get_width();
    int height = get_height();
    enum Colour line_colour = MID;
    for (int i = 0; i < height; i++) {
        draw_line(0, i, width, line_colour);
        line_colour = line_colour == MID ? LGT : MID;
    }

    while (1) {
        // NOTE: This is where real work would be done, prior to rendering
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
