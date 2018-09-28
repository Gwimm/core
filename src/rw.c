#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <err.h>
#include <unistd.h>

#include "com.h"
#include "xcb.h"

enum {
	ABSOLUTE = 0,
	RELATIVE = 1
};

static xcb_connection_t *conn;
static xcb_screen_t *scr;

static void resize(xcb_window_t, int, int, int);

int main(int argc, char *argv[]) {
	int arg, x, y, mode = RELATIVE;

	if (argc < 4)
        die("usage: rw [-a] <x> <y> [wid..]\n");

    while ((arg = getopt(argc, argv, "avh")) != -1) {
        switch(arg) {
            case 'a':
		        mode = ABSOLUTE;
                break;
            case 'v':
                version();
            default :
                die("usage: rw [-a] <x> <y> <wid> [wid..]\n");
        }
    }

	init_xcb(&conn);
	get_screen(conn, &scr);

	x = atoi(argv[++optind]);
	y = atoi(argv[++optind]);

	while (optind < argc)
		resize(strtoul(argv[optind++], NULL, 16), mode, x, y);

	xcb_aux_sync(conn);

	kill_xcb(&conn);

	return 0;
}

static void resize(xcb_window_t w, int mode, int x, int y) {
	uint32_t val[3];
	uint32_t mask = XCB_CONFIG_WINDOW_WIDTH
        | XCB_CONFIG_WINDOW_HEIGHT
        | XCB_CONFIG_WINDOW_STACK_MODE;
	xcb_get_geometry_cookie_t c;
	xcb_get_geometry_reply_t *r;

	c = xcb_get_geometry(conn, w);
	r = xcb_get_geometry_reply(conn, c, NULL);

	if (r == NULL)
		return;

	if (mode == ABSOLUTE) {
		x -= r->x + r->width;
		y -= r->y + r->height;
	}

	if ((r->x + r->width + 2*r->border_width + x) > scr->width_in_pixels)
		x = scr->width_in_pixels - (
				r->x + r->width + (2*r->border_width));

	if ((r->y + r->height + 2*r->border_width + y) > scr->height_in_pixels)
		y = scr->height_in_pixels - (
				r->y + r->height + (2*r->border_width));

	val[0] = r->width  + x;
	val[1] = r->height + y;
	val[2] = XCB_STACK_MODE_ABOVE;

	xcb_configure_window(conn, w, mask, val);

	free(r);
}
