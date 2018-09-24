#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <err.h>

#include "com.h"

static xcb_connection_t *conn;

static void usage(void);

int main(int argc, char **argv) {
	xcb_window_t win;

	if (argc != 2 || strncmp(argv[0], "-h", 2) == 0)
		usage(argv[0]);

	init_xcb(&conn);

	win = strtoul(argv[1], NULL, 16);

	if (win) {
		xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, win,
				XCB_CURRENT_TIME);

		xcb_aux_sync(conn);
	}

	kill_xcb(&conn);
	return 0;
}

static void usage(void) {
	die("usage: fw <wid>\n");
}
