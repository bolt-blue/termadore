/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef DRAWUTILS_H
#define DRAWUTILS_H

#include "screen.h"

// TODO: Remove superfluous `draw_` prefix
void draw_line(int, int, int, int, enum Shade, enum Colour);
void draw_rect(int, int, int, int, enum Shade, enum Shade, enum Colour, enum Colour);
void draw_elipse(int, int, int, int, enum Shade, enum Shade, enum Colour, enum Colour);

#endif /* DRAWUTILS_H */
