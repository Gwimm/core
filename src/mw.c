#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <err.h>

#include "com.h"
#include "xcb.h"

enum {
	ABSOLUTE = 0,
	RELATIVE = 1
};

static xcb_connection_t *conn;
static xcb_screen_t *scr;

static void usage(void);
static void move(xcb_window_t, int, int, int);

int main(int argc, char **argv) {
	int x, y, mode = RELATIVE;

	if (argc < 4)
		usage();

	init_xcb(&conn);
	get_screen(conn, &scr);

	if (argv[1][0] == '-' && argv[1][1] == 'a') {
		mode = ABSOLUTE;
		argv++;
	}

	x = atoi(*(++argv));
	y = atoi(*(++argv));

	while (*argv)
		move(strtoul(*argv++, NULL, 16), mode, x, y);

	xcb_aux_sync(conn);

	kill_xcb(&conn);

	return 0;
}

static void usage(void) {
	die("usage: mvw [-a] <x> <y> <win>\n");
}

static void move(xcb_window_t win, int mode, int x, int y) {
	uint32_t values[2];
	int real;
	xcb_get_geometry_reply_t *geom;

	if (!win || win == scr->root)
		return;

	geom = xcb_get_geometry_reply(conn, xcb_get_geometry(conn, win), NULL);
	if (!geom) {
		return;
    }

	if (mode == ABSOLUTE) {
		x -= geom->x + geom->width /2;
		y -= geom->y + geom->height/2;
	}
	values[0] = x ? geom->x + x : geom->x;
	values[1] = y ? geom->y + y : geom->y;

	if (x) {
		real = geom->width + (geom->border_width * 2);
		if (geom->x + x < 1)
			values[0] = 0;
		if (geom->x + x > scr->width_in_pixels - real)
			values[0] = scr->width_in_pixels - real;
	}

	if (y) {
		real = geom->height + (geom->border_width * 2);
		if (geom->y + y < 1)
			values[1] = 0;
		if (geom->y + y > scr->height_in_pixels - real)
			values[1] = scr->height_in_pixels - real;
	}

	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_X
			| XCB_CONFIG_WINDOW_Y, values);

	free(geom);
}
