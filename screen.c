#include "screen.h"

#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ========================================================================== */

static struct Screen g_screen;

// TODO: Store these in a struct?
static struct timespec g_current_time;
static struct timespec g_last_time;
static int g_max_fps;
static float g_dt;

#define NB_ENABLE 1
#define NB_DISABLE 2
#define reset() printf("\033[2J")
#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define hidecur() printf("\033[?25l");
#define showcur() printf("\033[?25h");

/* ========================================================================== */

static void stdinblock(int state)
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

int kbhit(void)
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

/*
 * TODO:
 * - Update screen if terminal gets resized
 */

int init_screen(void)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    unsigned short buf_w = w.ws_col / 2;
    unsigned short buf_h = w.ws_row;

    g_screen.buffer = calloc(buf_h, buf_w * sizeof(pixel));
    if (!g_screen.buffer)
        return 1;

    // Terminal width is halved so we can have more square "pixels"
    g_screen.w = buf_w;
    g_screen.h = buf_h;

    stdinblock(NB_ENABLE);
    hidecur();

    clock_gettime(CLOCK_MONOTONIC, &g_last_time);
    // TODO: Allow to be set by caller
    g_max_fps = 30;

    return 0;
}

void cleanup(void)
{
    free(g_screen.buffer);
    clear();
    stdinblock(NB_DISABLE);
    showcur();
}

void render(void)
{
    gotoxy(0, 0);

    pixel *cur = g_screen.buffer;
    for (int i = 0; i < g_screen.h; i++, cur += g_screen.w) {
        fwrite(cur, g_screen.w * sizeof(pixel), 1, stdout);
        gotoxy(0, i);
    }

    // TODO: Do we really want this?
    fflush(stdout);

    clock_gettime(CLOCK_MONOTONIC, &g_current_time);

    const long int nspf = 1e9 / g_max_fps;
    struct timespec rqtp;
    rqtp.tv_sec = 0;

    g_dt = (g_current_time.tv_sec + 1e-9 * g_current_time.tv_nsec)
                - (g_last_time.tv_sec + 1e-9 * g_last_time.tv_nsec);

    // NOTE: [Debug]
    fprintf(stderr, "Last: %ld:%-12ld | Current: %ld:%-12ld | dt: %.9f",
            g_last_time.tv_sec, g_last_time.tv_nsec,
            g_current_time.tv_sec, g_current_time.tv_nsec,
            g_dt);

    g_last_time = g_current_time;

    // Limit framerate if necessary
    rqtp.tv_nsec = nspf - g_dt * 1e9;
    // NOTE: [Debug]
    fprintf(stderr, " | Sleeping for: %-9ld nanoseconds\n", rqtp.tv_nsec);
    clock_nanosleep(CLOCK_MONOTONIC, 0, &rqtp, NULL);
}

void fill(enum Shade px_type)
{
    pixel *cur = g_screen.buffer;
    for (size_t i = 0; i < g_screen.w * g_screen.h; i++) {
        memcpy(cur++, &px_type, sizeof(pixel));
    }
}

void draw_line(int x, int y, int len, enum Shade px_type)
{
    pixel *cur = g_screen.buffer + y * g_screen.w + x;
    for (size_t i = 0; i < len; i++) {
        memcpy(cur++, &px_type, sizeof(pixel));
    }
}

void set_pixel(int x, int y, enum Shade px_type)
{
    pixel *pos = g_screen.buffer + y * g_screen.w + x;

    if (pos < g_screen.buffer || pos >= g_screen.buffer + g_screen.w * g_screen.h)
        return;

    memcpy(pos, &px_type, sizeof(pixel));
}

int get_width()
{
    return g_screen.w;
}
int get_height()
{
    return g_screen.h;
}
float get_dt()
{
    return g_dt;
}
