#include "drawutils.h"

#include <math.h>
#include <stdlib.h>

#define PI 3.14159265f

/* Math helpers */
/* TODO: separate header */
#define IS_EVEN(x) ~(x) & 1

// Prototypes
int line_clip(int *x0_out, int *y0_out, int *x1_out, int *y1_out,
              int xmin, int xmax, int ymin, int ymax);

/*
 * TODO:
 * - Write tests
 * - Better response to error-case(s)
 */

static struct Settings state = {
    .origin = {0, 0},		/* C99 */
    .rotate_angle = 0.0f,
    .scale_factor = 1.0f,
    .shd = BLK,
    .col = White,
};

void shade(enum Shade new)
{
    state.shd = new;
}

void colour(enum Colour new)
{
    state.col = new;
}

void translate(int x, int y)
{
    state.origin.x = x;
    state.origin.y = y;
}

void rotate(float angle)
{
    if (angle >= 360.0f)	/* Wrap angle */
	angle = fmod(angle, 360.0f);

    state.rotate_angle += angle;

    if (state.rotate_angle >= 360.0f) /* Wrap theta */
	state.rotate_angle -= 360.0f;
}

void scale(float factor)
{
    factor = fabs(factor);	/* Don't allow negative state.scale_factor*/
    state.scale_factor *= factor;
}

static void rotate_point(int *x, int *y)
{
    float rads = state.rotate_angle * PI / 180.0f;
    int tmp = *x;

    *x = round(*x * cos(rads) - *y * sin(rads));
    *y = round(tmp * sin(rads) + *y * cos(rads));
}

void line(int x1, int y1, int x2, int y2)
{
    rotate_point(&x1, &y1);
    rotate_point(&x2, &y2);

    x1 = state.origin.x + state.scale_factor * x1;
    x2 = state.origin.x + state.scale_factor * x2;
    y1 = state.origin.y + state.scale_factor * y1;
    y2 = state.origin.y + state.scale_factor * y2;

    /* if (!line_clip(&x1, &y1, &x2, &y2, 0, get_width(), 0, get_height())) */
    /*     return; */

    int dx = abs(x2 - x1);
    int dy = -abs(y2 - y1);
    int step_x = x1 < x2 ? 1 : -1;
    int step_y = y1 < y2 ? 1 : -1;
    int dxy = dx + dy;
    int error;

    while (1) {
        set_pixel(x1, y1, state.shd, state.col);
	if (x1 == x2 && y1 == y2)
	    break;

	error = 2 * dxy;

	if (error >= dy) {	/* error_xy + error_x > 0 */
	    dxy += dy;
	    x1 += step_x;
	}

	if (error <= dx) {	/* error_xy + error_y < 0 */
	    dxy += dx;
	    y1 += step_y;
	}
    }
}

void rect(int x, int y, int w, int h)
{
    line(x, y, x + w, y);
    line(x, y - h, x + w, y - h);
    line(x, y, x, y - h);
    line(x + w, y, x + w, y - h);
}

/* See "Raster Algorithms for 2D Primitives" (Da Silva, 1989) */
/* Available at: https://cs.brown.edu/research/pubs/theses/masters/1989/dasilva.pdf  */
void ellipse(int x, int y, int w, int h)
{
    w *= state.scale_factor;
    h *= state.scale_factor;

    /* Require a >= b */
    float a, b;
    float angle_rads = state.rotate_angle;
    if (w >= h) {
        a = w / 2.0f;
        b = h / 2.0f;
    } else {
        angle_rads += 90.0f;
        b = w / 2.0f;
        a = h / 2.0f;
    }
    angle_rads *= PI / 180.0f;

    /* Foci */
    float a2 = a * a;
    float c = sqrt(a2 - b * b);
    float xf = c * cos(angle_rads);
    float yf = c * sin(angle_rads);
    float xf2 = xf * xf;
    float yf2 = yf * yf;

    /* Centre */
    x += IS_EVEN(w) ? w / 2 : w / 2 + 1; /* TODO: why not round()? */
    y -= IS_EVEN(h) ? h / 2 : h / 2 + 1;
    rotate_point(&x, &y);
    int cx = state.origin.x + x;
    int cy = state.origin.y + y;

    /* Coefficients of implicit equation F(x, y) = Ax2 + Bxy + Cy2 + D = 0 */
    float A = a2 - xf2;
    float B = -2 * xf * yf;
    float C = a2 - yf2;
    float D = a2 * (yf2 - A);

    float A2 = A + A;
    float B2 = B + B;
    float C2 = C + C;
    float B_2 = B / 2;

    /* Region boundaries */
    int XV, YV, YR, XH, XL;
    {
        /* 8-1, 4-5 */
        float k1 = -B / C2;
        float Xv = sqrt(-D / (A + B * k1 + C * k1 * k1));
        if (Xv < 0)
            Xv = -Xv;
        float Yv = k1 * Xv;

        /* 2-3 */
        float k = -B / A2;
        float Yh = sqrt(-D / (A * k * k + B * k + C));
        if (Yh < 0)
            Yh = -Yh;
        float Xh = k * Yh;

        /* 1-2 */
        k = (A2 - B) / (C2 - B);
        float Xr = sqrt(-D / (A + B * k + C * k * k));
        float Yr = k * Xr;
        if (Yr < k1 * Xr)
            Yr = -Yr;

        /* 3-4 */
        k = (-A2 - B) / (C2 + B);
        float Xl = sqrt(-D / (A + B * k + C * k * k));
        float Yl = k * Xl;
        if (Yl < k1 * Xl)
            Xl = -Xl;

        XV = round(Xv);
        YV = round(Yv);
        YR = round(Yr);
        XH = round(Xh);
        XL = round(Xl);
    }

    /* Init */
    x = XV;
    y = YV;
    float initx = x - 0.5f;
    float inity = y + 1;

    float diffN = C2 * inity + B * initx + C;
    float diffNW = diffN - A2 * initx - B * inity + A - B;
    float midpoint = A * initx * initx + B * initx * inity + C * inity * inity + D;

    float diffN_N = C2, diffNW_N = C2 - B, diffNW_NW = A2 - B2 + C2;
    float diffW_W = A2, diffNW_W = A2 - B, diffSW_SW = A2 + B2 + C2;
    float diffS_S = C2, diffSW_W = A2 + B, diffSW_S = C2 + B;

    float cross1 = B - A, cross2 = A - B + C, cross3 = A + B + C, cross4 = A + B;

    /* Region 1 */
    while (y < YR) {
        set_pixel(cx + x, cy + y, state.shd, state.col);
        set_pixel(cx - x, cy - y, state.shd, state.col);
        y++;

        if (midpoint < 0 || (diffN - diffNW) < cross1) {
            midpoint += diffN;
            diffN += diffN_N;
            diffNW += diffNW_N;
        } else {
            x--;
            midpoint += diffNW;
            diffN += diffNW_N;
            diffNW += diffNW_NW;
        }
    }

    /* Region 2 */
    float diffW = diffNW - diffN + A + B + B_2;
    diffNW += A - C;
    midpoint += (diffW - diffN + C) / 2 + (A + C) / 4 - A;

    while (x > XH) {
        set_pixel(cx + x, cy + y, state.shd, state.col);
        set_pixel(cx - x, cy - y, state.shd, state.col);
        x--;

        if (midpoint < 0 || (diffNW - diffW) < cross2) {
            y++;
            midpoint += diffNW;
            diffW += diffNW_W;
            diffNW += diffNW_NW;
        } else {
            midpoint += diffW;
            diffW += diffW_W;
            diffNW += diffNW_W;
        }
    }

    /* Region 3 */
    midpoint += diffW - diffNW + C2 - B;
    diffW += B;
    float diffSW = diffW - diffNW + diffW + C2 + C2 - B;

    while (x > XL) {
        set_pixel(cx + x, cy + y, state.shd, state.col);
        set_pixel(cx - x, cy - y, state.shd, state.col);
        x--;

        if (midpoint < 0 || (diffSW - diffW) > cross3) {
            midpoint += diffW;
            diffW += diffW_W;
            diffSW += diffSW_W;
        } else {
            y--;
            midpoint += diffSW;
            diffW += diffSW_W;
            diffSW += diffSW_SW;
        }
    }

    /* Region 4 */
    float diffS = diffSW - diffW - B;
    midpoint += diffS - diffSW / 2 + A - (A + C - B) / 4;
    diffSW += C - A;
    diffS += C - B_2;
    YV = -YV;

    while (y > YV) {
        set_pixel(cx + x, cy + y, state.shd, state.col);
        set_pixel(cx - x, cy - y, state.shd, state.col);
        y--;

        if (midpoint < 0 || (diffSW - diffS) < cross4) {
            x--;
            midpoint += diffSW;
            diffS += diffSW_S;
            diffSW += diffSW_SW;
        } else {
            midpoint += diffS;
            diffS += diffS_S;
            diffSW += diffSW_S;
        }
    }
}


/* ========================================================================== */


#define bool char
#define false 0
#define true 1

#ifndef CSCLIP
int line_clip(int *x0_out, int *y0_out, int *x1_out, int *y1_out,
              int xmin, int xmax, int ymin, int ymax)
{

    return false;
}
#else
typedef int outcode;

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// compute_ the bit code for a point (x, y) using the clip
// bounded diagonally by (xmin, ymin), and (xmax, ymax)

// ASSUME THAT xmax, xmin, ymax and ymin are global constants.

outcode compute_outcode(int x, int y, int xmin, int xmax, int ymin, int ymax)
{
    outcode code;
    code = INSIDE;          // initialised as being inside of [[clip window]]

    if (x < xmin)           // to the left of clip window
        code |= LEFT;
    else if (x > xmax)      // to the right of clip window
        code |= RIGHT;
    if (y < ymin)           // below the clip window
        code |= BOTTOM;
    else if (y > ymax)      // above the clip window
        code |= TOP;

    return code;
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
// diagonal from (xmin, ymin) to (xmax, ymax).
int line_clip(int *x0_out, int *y0_out, int *x1_out, int *y1_out,
              int xmin, int xmax, int ymin, int ymax)
{
    int x0, y0, x1, y1;
    x0 = *x0_out;
    y0 = *y0_out;
    x1 = *x1_out;
    y1 = *y1_out;
    // compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
    outcode outcode0 = compute_outcode(x0, y0, xmin, xmax, ymin, ymax);
    outcode outcode1 = compute_outcode(x1, y1, xmin, xmax, ymin, ymax);
    bool accept = false;

    while (true) {
        if (!(outcode0 | outcode1)) {
            // bitwise OR is 0: both points inside window; trivially accept and exit loop
            accept = true;
            break;
        } else if (outcode0 & outcode1) {
            // bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
            // or BOTTOM), so both must be outside window; exit loop (accept is false)
            break;
        } else {
            // failed both tests, so calculate the line segment to clip
            // from an outside point to an intersection with clip edge
            int x, y;

            // At least one endpoint is outside the clip rectangle; pick it.
            outcode outcode_out = outcode1 > outcode0 ? outcode1 : outcode0;

            // Now find the intersection point;
            // use formulas:
            //   slope = (y1 - y0) / (x1 - x0)
            //   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
            //   y = y0 + slope * (xm - x0), where xm is xmin or xmax
            // No need to worry about divide-by-zero because, in each case, the
            // outcode bit being tested guarantees the denominator is non-zero
            if (outcode_out & TOP) {           // point is above the clip window
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if (outcode_out & BOTTOM) { // point is below the clip window
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if (outcode_out & RIGHT) {  // point is to the right of clip window
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else if (outcode_out & LEFT) {   // point is to the left of clip window
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            // Now we move outside point to intersection point to clip
            // and get ready for next pass.
            if (outcode_out == outcode0) {
                x0 = x;
                y0 = y;
                outcode0 = compute_outcode(x0, y0, xmin, xmax, ymin, ymax);
            } else {
                x1 = x;
                y1 = y;
                outcode1 = compute_outcode(x1, y1, xmin, xmax, ymin, ymax);
            }
        }
    }

    *x0_out = x0;
    *y0_out = y0;
    *x1_out = x1;
    *y1_out = y1;

    return accept;
}
#endif // CSCLIP
