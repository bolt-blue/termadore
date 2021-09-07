/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef DRAWUTILS_H
#define DRAWUTILS_H

#include "screen.h"

struct setting {
    enum Shade shd;
    enum Colour col;
};

// TODO: Remove superfluous `draw_` prefix
void line(int, int, int, int);
void rect(int, int, int, int);
void ellipse(int, int, int, int);

#endif /* DRAWUTILS_H */
