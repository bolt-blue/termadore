/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef DRAWUTILS_H
#define DRAWUTILS_H

#include "screen.h"

struct setting {
    enum Shade shd;
    enum Colour col;
};

void shade(enum Shade);
void colour(enum Colour);

struct point {
    int x;
    int y;
};

void translate(int, int);
void rotate(float);
void scale(float);

void line(int, int, int, int);
void rect(int, int, int, int);
void ellipse(int, int, int, int);

#endif /* DRAWUTILS_H */
