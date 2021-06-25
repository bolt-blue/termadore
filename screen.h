/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

typedef union pixel {
    uint64_t i;
    char code[8];
} pixel;

struct Screen {
    unsigned short w;
    unsigned short h;
    pixel *pen;
    pixel *buffer;
    pixel *end;
};

// TODO: Respect endian-ness
enum Shade {
    BLK = 0x8896e2008896e200,
    DRK = 0x9396e2009396e200,
    MID = 0x9296e2009296e200,
    LGT = 0x9196e2009196e200,
    CLR = 0x2000000020000000
};

int kbhit();
int init_screen();
void cleanup();
void render();
void fill(enum Shade);
void draw_line(int, int, int, enum Shade);
void set_pixel_at_pen(enum Shade);
void set_pixel_xy(int, int, enum Shade);
void set_pen(int, int);
int get_width();
int get_height();
float get_dt();

#endif /* SCREEN_H */
