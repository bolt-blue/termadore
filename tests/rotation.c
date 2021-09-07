#include <stdio.h>

#include "termadore.h"

int main(int argc, char **argv)
{
    if (init_screen() != 0) {
	fprintf(stderr, "[Error] Failed to initialise screen");
	return 1;
    }

    int w = get_width();
    int h = get_height();

    while (1) {
	if (detect_resize()) {
	    kill_window();
	    init_window();
	}

	fill(CLR, Unset);

	/* Upper left lines */
	translate(w / 4, h / 4);
	shade(MID);
	colour(Red);
	line(0, 0, 10, 0);

	/* Bottom right rects */
	translate(w - w / 4,
		  h - h / 4);
	shade(BLK);
	colour(Green);
	rect(0, 0, 10, 10);

	/* Upper right ellipses */
	/* Doesn't work*/
	translate(w - w / 4, h / 4);
	shade(LGT);
	colour(Cyan);

	scale(2.0f);
	ellipse(0, 0, 5, 10);
	scale(1 / 2.0f);

	/* Bottom left rects */
	translate(w / 4, h - h / 4);
	shade(BLK);
	colour(Magenta);

	scale(2);
	rotate(180);
	rect(0, 0, 10, 10);
	rotate(180);
	scale(0.5);

	render();
	rotate(3);

	if (!kbhit())
	    continue;

	char key = fgetc(stdin);
	if (key == 'q')
	    break;
    }

    cleanup();

    return 0;
}
