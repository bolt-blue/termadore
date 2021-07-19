/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

// TODO: Respect endian-ness
enum Shade {
    BLK = 0x8896e2008896e200,
    DRK = 0x9396e2009396e200,
    MID = 0x9296e2009296e200,
    LGT = 0x9196e2009196e200,
    CLR = 0x2000000020000000
};

enum Colour {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    BBlack,
    BRed,
    BGreen,
    BYellow,
    BBlue,
    BMagenta,
    BCyan,
    BWhite,
    Unset = -1
};

typedef struct pixel {
    enum Shade shd;
    enum Colour col;
} pixel;

struct Screen {
    unsigned short w;
    unsigned short h;
    pixel *pen;
    pixel *buffer;
    pixel *end;
};

int kbhit();
int init_window(void);
int init_screen();
void kill_window(void);
void cleanup();
int detect_resize(void);
void render();
void fill(enum Shade, enum Colour);
void draw_line(int, int, int, enum Shade, enum Colour);
void set_pixel(int, int, enum Shade, enum Colour);
void set_pixel_at_pen(enum Shade, enum Colour);
void set_pen(int, int);
void write_string(char *, int, int, int);
int get_width();
int get_height();
float get_dt();

#endif /* SCREEN_H */
