/* by camel-cdr - refactored by bolt-blue */
/* cc torus.c -DH=32 -DW=64 -lm */
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#define W 64
#define H 32

float X,Y,Z,l,t,x,y,z,A,S,C;

// NOTE X and Y here are shadowed
void r(float *X, float *Y)
{
    A = *X;
    S = sin(l);
    C = cos(l);

    *X = A * C - *Y *S;
    *Y = A * S + *Y *C;
}

int main(void)
{
    int k;
    int sz = (W + 1) * H;
    char buf[sz];
    char *p;

    printf("\x1b[2J");

    while (1) {
        p = buf;
        for (int i = 0; i < H; ++i, *p++ = '\n') {
            for(int j=0; j < W; ++j, *p++ = "@$#=:,-. "[k=t]) {
                X = (2. * j - W) / W;
                Y = (2. * i - H) / H;
                Z = 3;

                l = sqrt(X*X + Y*Y + Z);

                X /= l;
                Y /= l;
                Z /= l*l;

                for(t = k = 0; k < 50 && ~(l < .05 || (t += l) > 8 && (t = 8)); ++k) {
                    x = X * t;
                    y = Y * t;
                    z = Z * t - 5;

                    l = 1. / CLOCKS_PER_SEC;
                    l *= clock();
                    r(&y, &z);

                    l /= 2;
                    r(&x, &y);

                    x = sqrt(x*x + z*z) - 2.2;
                    l = sqrt(x*x + y*y) - .4;
                }
            }
        }

        // Clear terminal buffer (?) and draw
        puts("\x1b[H");
        write(1, buf, sz);
    }

    return 0;
}
