#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <err.h>
#include <getopt.h>

#include "com.h"

enum {
    MAP     = 1 << 0,
    UNMAP   = 1 << 1,
    TOGGLE  = 1 << 2
};

static xcb_connection_t *conn;

static void usage(void);

int main(int argc, char *argv[]) {
    int mapflag = 0;
    char arg;
    xcb_window_t w = 0;

    while ((arg = getopt(argc, argv, "muth")) != -1) {
        switch(arg) {
            case 'm':
                mapflag = MAP;
                break;
            case 'u':
                mapflag = UNMAP;
                break;
            case 't':
                mapflag = TOGGLE;
                break;
            case 'h':
            default:
            usage();
        }
    }

    if (argc < 1 || mapflag == 0)
        usage();

    init_xcb(&conn);

    while (optind < argc) {
        w = strtoul(argv[optind++], NULL, 16);

        switch (mapflag) {
            case MAP:
                xcb_map_window(conn, w);
                break;
            case UNMAP:
                xcb_unmap_window(conn, w);
                break;
            case TOGGLE:
                if (mapped(conn, w))
                    xcb_unmap_window(conn, w);
                else
                    xcb_map_window(conn, w);
                break;
        }
    }

    xcb_aux_sync(conn);

    kill_xcb(&conn);

    return 0;
}

static void usage(void) {
    die("(un-)hide windows\n"
        "usage: hw [-h] [-mut <wid> [wid..]]\n");
}
