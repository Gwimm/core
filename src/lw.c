#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <err.h>
#include <getopt.h>

#include "com.h"

static xcb_connection_t *conn;
static xcb_screen_t *scrn;

static void usage(void);
static int should_list(xcb_window_t, int);
static xcb_window_t focused_window(void);
static void list_windows(xcb_window_t, int);

enum {
    LIST_HIDDEN = 1 << 0,
    LIST_IGNORE = 1 << 1,
    LIST_ALL    = 1 << 2
};

int main(int argc, char *argv[]) {
    int listmask=0;
    char mode, arg;

    while ((arg = getopt(argc, argv, "auorch")) != -1) {
        switch(arg) {
            case 'a':
                listmask |= LIST_ALL;
                break;
            case 'u':
                listmask |= LIST_HIDDEN;
                break;
            case 'o':
                listmask |= LIST_IGNORE;
                break;
            case 'r':
            case 'c':
                mode=arg;
                break;
            default :
                usage();
        }
    }

    init_xcb(&conn);

    get_screen(conn, &scrn);

    switch(mode) {
        case 'r':
            printf("0x%08x\n", scrn->root);
            return 0;
        case 'c':
            printf("0x%08x\n", focused_window());
            return 0;
    }
    
    if(optind >= argc)
        list_windows(scrn->root, listmask);

    while(optind < argc)
		list_windows(strtoul(argv[optind++], NULL, 16), listmask);

    kill_xcb(&conn);

    return 0;
}

static void usage(void) {
    die("list windows\n"
        "lw [-auorc] [wid...]\n");
}

static int should_list(xcb_window_t w, int mask) {
    if ((mask & LIST_ALL) || (!mapped(conn, w) && mask & LIST_HIDDEN)
            || (ignore(conn, w) && mask & LIST_IGNORE) || (mapped(conn, w)
                && !ignore(conn, w) && mask == 0))
        return 1;

    return 0;
}

static void list_windows(xcb_window_t w, int listmask) {
    int i, wn;
    xcb_window_t *wc;

    wn = get_windows(conn, w, &wc);

    if (wc == NULL)
        errx(1, "0x%08x: unable to retrieve children", w);

    for (i=0; i<wn; i++) {
        if (should_list(wc[i], listmask))
            printf("0x%08x\n", wc[i]);
    }

    free(wc);
}

static xcb_window_t focused_window(void) {
	xcb_window_t w = 0;
	xcb_get_input_focus_cookie_t c;
	xcb_get_input_focus_reply_t *r;

	c = xcb_get_input_focus(conn);
	r = xcb_get_input_focus_reply(conn, c, NULL);
	if (r == NULL)
		errx(1, "xcb_get_input_focus");

	w = r->focus;
	free(r);

	if (w == XCB_NONE || w == XCB_INPUT_FOCUS_POINTER_ROOT)
		errx(1, "focus not set");

	return w;
}
