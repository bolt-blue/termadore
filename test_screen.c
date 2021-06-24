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
    int width = getwidth();
    int height = getheight();
    enum Colour line_colour = MID;
    for (int i = 0; i < height; i++) {
        draw_line(0, i, width, line_colour);
        line_colour = line_colour == MID ? LGT : MID;
    }

    char key;
    while (1) {
        if (kbhit()) {
            key = fgetc(stdin);
            if (key == 'q')
                break;
        }
        draw();
    }

    cleanup();

    return 0;
}
