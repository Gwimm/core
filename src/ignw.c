#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <err.h>
#include <getopt.h>

#include "util.h"

char *argv0;
static xcb_connection_t *conn;

static void usage(char *);
static void set_override(xcb_window_t, int);

int main(int argc, char **argv) {
    int setflag = 0;
    char arg;

    while ((arg = getopt(argc, argv, "sr")) != -1) {
        switch(arg) {
            case 's':
                setflag = 1;
                break;
            case 'r':
                setflag = 0;
                break;
            default:
                usage(argv0);
        }
    }

    init_xcb(&conn);

    while (*argv)
        set_override(strtoul(*argv++, NULL, 16), setflag);

    xcb_aux_sync(conn);

    kill_xcb(&conn);

    return 0;
}

static void usage(char *name) {
    fprintf(stderr, "usage: %s [-sr] <wid> [wid..]\n", name);
    exit(1);
}

static void set_override(xcb_window_t w, int or) {
    uint32_t mask = XCB_CW_OVERRIDE_REDIRECT;
    uint32_t val[] = { or };

    xcb_change_window_attributes(conn, w, mask, val);
}
