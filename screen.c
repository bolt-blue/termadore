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
    uint64_t i;
    char code[8];
} unichar;

struct Screen {
    unsigned short w;
    unsigned short h;
    unichar *buffer;
} g_screen;

enum Colour {
    BLK = 0x8896e2008896e200,
    DRK = 0x9396e2009396e200,
    MID = 0x9296e2009296e200,
    LGT = 0x9196e2009196e200,
    CLR = 0x2000000020000000
};

#define NB_ENABLE 1
#define NB_DISABLE 2
#define reset() printf("\033[2J")
#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define hidecur() printf("\033[?25l");
#define showcur() printf("\033[?25h");

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

int kbhit()
{
    fd_set rfds;
    struct timeval tv;

    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);

    // Poll immediately
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    if (select(STDOUT_FILENO, &rfds, NULL, NULL, &tv) == -1)
        perror("select()");

    return FD_ISSET(STDIN_FILENO, &rfds);
}

/* ========================================================================== */

int init_screen(void)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    unsigned short buf_w = w.ws_col / 2;
    unsigned short buf_h = w.ws_row;

    g_screen.buffer = calloc(buf_h, buf_w * sizeof(unichar));
    if (!g_screen.buffer)
        return 1;

    // Terminal width is halved so we can have more square "pixels"
    g_screen.w = buf_w;
    g_screen.h = buf_h;

    stdinblock(NB_ENABLE);
    hidecur();

    return 0;
}

void cleanup()
{
    free(g_screen.buffer);
    clear();
    stdinblock(NB_DISABLE);
    showcur();
}

void draw(void)
{
    gotoxy(0, 0);

    unichar *cur = g_screen.buffer;
    for (int i = 0; i < g_screen.h; i++, cur += g_screen.w) {
        fwrite(cur, g_screen.w * sizeof(unichar), 1, stdout);
        gotoxy(0, i);
    }

    // NOTE: The following was first attempt to avoid mixing use of FILE * and file descriptors.
    // But it does not overwrite the screen as the above does.
    // TODO: Find a way to do the above but with filedes
    //write(STDOUT_FILENO, g_screen.buffer, g_screen.w * g_screen.h * sizeof(unichar));
}

void fill(enum Colour c)
{
    unichar *cur = g_screen.buffer;
    for (size_t i = 0; i < g_screen.w * g_screen.h; i++) {
        memcpy(cur++, &c, sizeof(unichar));
    }
}

void draw_line(int x, int y, int len, enum Colour c)
{
    unichar *cur = g_screen.buffer + y * g_screen.w + x;
    for (size_t i = 0; i < len; i++) {
        memcpy(cur++, &c, sizeof(unichar));
    }
}

int getwidth()
{
    return g_screen.w;
}
int getheight()
{
    return g_screen.h;
}
