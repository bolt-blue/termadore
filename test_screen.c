#include "screen.c"

int main(int argc, char **argv)
{
    if (init_screen() != 0) {
        fprintf(stderr, "[Error] Failed to initialise screen");
        return 1;
    }

    fill(MID);

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
