#include "screen.h"
#include "tdsignal.h"

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
#define reset_colour() fputs("\033[0m", stdout);
#define reset() fputs("\033[2J", stdout)
#define clear() fputs("\033[H\033[J", stdout)
#define hidecur() fputs("\033[?25l", stdout)
#define showcur() fputs("\033[?25h", stdout)
#define gotoxy(x,y) fprintf(stdout, "\033[%d;%dH", (y), (x))

#ifndef NDEBUG
char g_scratch[512];
#define DEBUG_PRINT(dbgx, dbgy, ...) \
    snprintf(g_scratch, 511, __VA_ARGS__); \
    write_string(g_scratch, strlen(g_scratch), (dbgx), (dbgy))
#else
#define DEBUG_PRINT(...)
#endif

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
 * - Error handling
 */

int init_window(void)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    // Terminal width is halved so we can have more square "pixels"
    unsigned short buf_w = w.ws_col / 2;
    unsigned short buf_h = w.ws_row;

    g_screen.buffer = calloc(buf_h, buf_w * sizeof(pixel));
    if (!g_screen.buffer)
        return 1;

    g_screen.w = buf_w;
    g_screen.h = buf_h;
    g_screen.pen = g_screen.buffer;
    g_screen.end = g_screen.buffer + buf_w * buf_h;

    // Register signal handler with the kernel
    signal(SIGINT, sig_handler);

    return 0;
}

int init_screen(void)
{
    init_window();

    stdinblock(NB_ENABLE);
    hidecur();

    clock_gettime(CLOCK_MONOTONIC, &g_last_time);
    // TODO: Allow to be set by caller
    g_max_fps = 30;

    return 0;
}

void kill_window(void)
{
    free(g_screen.buffer);
    clear();
}

void cleanup(void)
{
    kill_window();
    stdinblock(NB_DISABLE);
    reset_colour();
    // TODO: Make sure cursor was originally visible before setting it on
    showcur();
}

int detect_resize(void)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    unsigned short buf_w = w.ws_col / 2;
    unsigned short buf_h = w.ws_row;

    DEBUG_PRINT(1, 1, "W: %3d | H : %3d", buf_w, buf_h);

    if (buf_w != g_screen.w || buf_h != g_screen.h)
        return 1;

    return 0;
}

void render(void)
{
    clock_gettime(CLOCK_MONOTONIC, &g_current_time);

    // TODO: Avoid continually recalculating this value
    const long int nspf = 1e9 / g_max_fps;
    struct timespec rqtp;
    rqtp.tv_sec = 0;

    // TODO: Determine if our general approach to measuring delta time
    // and throttling frame rate is suitable
    g_dt = (g_current_time.tv_sec + 1e-9 * g_current_time.tv_nsec)
                - (g_last_time.tv_sec + 1e-9 * g_last_time.tv_nsec);

    // TODO: Use (and draw from) a separate buffer for debug text
    DEBUG_PRINT(0, g_screen.h - 2, "Last: %ld:%-12ld | Current: %ld:%-12ld | dt: %.9f",
                g_last_time.tv_sec, g_last_time.tv_nsec,
                g_current_time.tv_sec, g_current_time.tv_nsec,
                g_dt);

    g_last_time = g_current_time;

    // Limit framerate if necessary
    rqtp.tv_nsec = nspf - g_dt * 1e9;
    DEBUG_PRINT(0, g_screen.h - 1, "Sleeping for: %-9ld nanoseconds", rqtp.tv_nsec);

    pixel *line_cur = g_screen.buffer;
    for (int i = 0; i < g_screen.h; i++, line_cur += g_screen.w) {
        // TODO: Investigate this off-by-one issue
        gotoxy(0, i + 1);

        for (int j = 0; j < g_screen.w; j++) {
            enum Colour colour = line_cur[j].col;
            enum Shade shade = line_cur[j].shd;
            char col_code[14] = {0};

            if (colour != Unset) {
                snprintf(col_code, 14, "\033[38;5;%dm", colour);
            } else {
                // TODO: Maybe use a default instead of `Unset`
                // - can then avoid need for this branch
                snprintf(col_code, 8, "\033[0m");
            }

            fputs(col_code, stdout);
            fwrite(&shade, sizeof(shade), 1, stdout);
        }
    }

    clock_nanosleep(CLOCK_MONOTONIC, 0, &rqtp, NULL);
}

void fill(enum Shade px_type, enum Colour px_col)
{
    pixel *cur = g_screen.buffer;
    while (cur < g_screen.end) {
        cur->shd = px_type;
        cur->col = px_col;
        cur++;
    }
}

/*
 * NOTE: Currently this allows line to overflow on the right
 * and continue on the next line.
 * TODO: Better response to error-case(s)
 */
void draw_line(int x, int y, int len, enum Shade px_type, enum Colour px_col)
{
    if (x < 0 || y < 0 || x >= g_screen.w || y >= g_screen.h)
        return;

    pixel *cur = g_screen.buffer + y * g_screen.w + x;

    if (cur + len > g_screen.end)
        return;

    for (size_t i = 0; i < len; i++) {
        cur->shd = px_type;
        cur->col = px_col;
        cur++;
    }

    g_screen.pen = cur;
}

void set_pixel(int x, int y, enum Shade px_type, enum Colour px_col)
{
    if (x < 0 || y < 0 || x >= g_screen.w || y >= g_screen.h)
        return;

    int offset = y * g_screen.w + x;
    g_screen.buffer[offset].shd = px_type;
    g_screen.buffer[offset].col = px_col;
}

void set_pixel_at_pen(enum Shade px_type, enum Colour px_col)
{
    g_screen.pen->col = px_col;
    g_screen.pen->shd = px_type;
    g_screen.pen++;
    if (g_screen.pen >= g_screen.end)
        g_screen.pen = g_screen.buffer;
}

/*
 * Will leave pen untouched if x or y is out of bounds
 */
void set_pen(int x, int y)
{
    if (x < 0 || y < 0 || x >= g_screen.w || y >= g_screen.h)
        return;
    g_screen.pen = g_screen.buffer + y * g_screen.w + x;
}

void write_string(char *str, int len, int x, int y)
{
    // TODO: Find a nicer approach to this black magic
    // Will probably involve refactoring a few things

    char buf[len + (len % 2)];
    strncpy(buf, str, len);

    if (len % 2) {
        buf[len] = ' ';
    }

    // NOTE: We're going to be writing two chars per pixel
    int steps = len / 2.0f + 0.5;

    pixel *px = g_screen.buffer + y * g_screen.w + x;
    for (int i = 0; i < steps; i++, px++) {
        px->shd = 0;
        memcpy(&px->shd, buf + i*2, 2);
    }
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
