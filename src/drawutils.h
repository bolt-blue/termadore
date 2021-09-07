/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef DRAWUTILS_H
#define DRAWUTILS_H

#include "screen.h"

struct setting {
    enum Shade shd;
    enum Colour col;
};

// TODO: Remove superfluous `draw_` prefix
void draw_line(int, int, int, int);
void draw_rect(int, int, int, int);
void draw_elipse(int, int, int, int);

#endif /* DRAWUTILS_H */
