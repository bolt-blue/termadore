#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

/* ========================================================================== */

typedef union unichar {
    uint32_t i;
    char code[4];
} unichar;

struct Screen {
    unsigned short w;
    unsigned short h;
    unichar *buffer;
} g_screen;

enum Colour {
    BLK = 0x8896e200,
    DRK = 0x9396e200,
    MID = 0x9296e200,
    LGT = 0x9196e200,
    CLR = 0x20000000
};

#define NB_ENABLE 1
#define NB_DISABLE 2
#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

/* ========================================================================== */

void stdinblock(int state)
{
    struct termios ttystate;

    tcgetattr(STDIN_FILENO, &ttystate);

    // Set canonical mode
    // - When enabled (default), reads will wait for 'Enter' keypress
    if (state == NB_ENABLE) {
        ttystate.c_lflag &= ~ICANON;
        // Read after single byte received
        ttystate.c_cc[VMIN] = 1;
    } else if (state == NB_DISABLE) {
        ttystate.c_lflag |= ICANON;
    }

    // Update terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

int init_screen(void)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    g_screen.buffer = calloc(w.ws_row, w.ws_col * sizeof(unichar));
    if (!g_screen.buffer)
        return 1;

    g_screen.w = w.ws_col;
    g_screen.h = w.ws_row;

    stdinblock(NB_ENABLE);

    return 0;
}

void cleanup()
{
    free(g_screen.buffer);
    clear();
    stdinblock(NB_DISABLE);
}

void draw(void)
{
    gotoxy(0, 0);
    fwrite(g_screen.buffer, g_screen.w * sizeof(unichar), g_screen.h, stdout);
}

void fill(enum Colour c)
{
    unichar *cur = g_screen.buffer;
    for (size_t i = 0; i < g_screen.w * g_screen.h; i++) {
        memcpy(cur++, &c, sizeof(unichar));
    }
}

int kbhit()
{
    fd_set rfds;
    struct timeval tv;

    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);

    // Poll about 120 times per second
    tv.tv_sec = 0;
    tv.tv_usec = 8192;

    if (select(STDOUT_FILENO, &rfds, NULL, NULL, &tv) == -1)
        perror("select()");

    return FD_ISSET(STDIN_FILENO, &rfds);
}
