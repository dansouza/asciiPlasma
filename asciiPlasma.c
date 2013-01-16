// asciiPlasma is a C port of http://ioyu.com/io/javascript/asciiPlasma.asp
// mesmerizing eye-candy now in your console!
// Daniel <thehazard@gmail.com>

// gcc -Wall -g -O2 -o asciiPlasma asciiPlasma.c -lm -lncurses

// TODO: custom terminal dimensions as parameters (-w and -h)
// TODO: option to switch between PAL1 and PAL2 at runtime (-1 and -2)
// TODO: use colors/shades of gray on the palette (256 color terminals?)
// TODO: optional text message to be displayed in the middle ([-m <message>])
// TODO: handle terminal resizing gracefully
// TODO: redirect output to another virtual terminal (troll-mode!)

// based on ASCII Plasma (c) 2006 by Lee W. Fastenau
// http://www.reddit.com/r/programming/comments/16mmon/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <signal.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>
#include <ncurses.h>

char PAL1[] = " `.!:/;([icILYOPXBMBXPOYLIci[(;/:!.`";
char PAL2[] = " `.!:/;([icILYOPXBMBXPOYLIci[(;/:!.` .:;[cLOXMPLi;!           ";
#define PALETTE PAL2
#define PALETTE_LIMIT (sizeof(PALETTE)-2)

typedef struct {
	WINDOW *window;
	int width;
	int height;
	size_t framesize;
	char *frame;
} plasma_t;

plasma_t Plasma;

volatile double
now()
{
	struct timeval _now;
	double now;

	gettimeofday(&_now, NULL);
	now = ((_now.tv_sec*1000.0) + (_now.tv_usec/1000.0));
	return now;
}

int
plasma_init(plasma_t *plasma, WINDOW *win, int width, int height)
{
	if (width == 0 && height == 0) {
		if (win) {
			getmaxyx(win, height, width);
		} else {
			return 0;
		}
	}
	plasma->window = win;
	plasma->height = height;
	plasma->width = width;
	plasma->framesize = (plasma->width * plasma->height) + 1;
	plasma->frame = (char *) malloc(plasma->framesize);
	return ((plasma->frame != NULL) ? 1 : 0);
}

char *
plasma_draw(plasma_t *plasma)
{
	char *buffer;
	int x, y, offset = 0;
	double v, t;

	t = now();
	buffer = plasma->frame;
	for (y = 0; y < plasma->height; y++) {
		for (x = 0; x < plasma->width; x++) {
			v = sin(x*0.15 - t*0.008);
			v += sin((y-10) * 0.3 * (sin(t/700)-2) + t*0.008);
			v += sin(sqrt(pow(x/2-10,2) + pow(y-10,2)) * 0.4);
			v = (v/6) + 0.5;

			buffer[offset++] = PALETTE[(int) round(v * PALETTE_LIMIT)];
		}
	}
	buffer[offset] = 0;
	mvwprintw(plasma->window, 0, 0, "%s", buffer);
	return plasma->frame;
}

int
main(int argc, char **argv)
{
	WINDOW *mainwin;

	if ((mainwin = initscr()) == NULL) {
		fprintf(stderr, "error initialising ncurses.\n");
		exit(EXIT_FAILURE);
	}

	// NOTE: original size is 90x40
	if (!plasma_init(&Plasma, mainwin, 0, 0)) {
		fprintf(stderr, "error initialising the plasma instance.\n");
		exit(EXIT_FAILURE);
	}

	nodelay(stdscr, TRUE);
	while (getch() == ERR) {
		plasma_draw(&Plasma);
		refresh();
		usleep(1 * 1000);
	}
	endwin();
	exit(EXIT_SUCCESS);
}

