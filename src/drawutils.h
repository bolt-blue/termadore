/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef DRAWUTILS_H
#define DRAWUTILS_H

#include "screen.h"

enum Rectmode {CENTRE, XY};

struct Point {
    int x;
    int y;
};

struct Settings {
    struct Point origin;
    float rotate_angle;
    float scale_factor;
    enum Rectmode mode;
    enum Shade shd;
    enum Colour col;
};

void shade(enum Shade);
void colour(enum Colour);
void mode(enum Rectmode);
void translate(int, int);
void rotate(float);
void scale(float);
void line(int, int, int, int);
void rect(int, int, int, int);
void ellipse(int, int, int, int);

#endif /* DRAWUTILS_H */
