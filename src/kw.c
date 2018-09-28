#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <err.h>
#include <unistd.h>

#include "com.h"
#include "xcb.h"

static xcb_connection_t *conn;

static void usage(void);

int main(int argc, char **argv) {
    int parent = 0, arg;

    if (argc < 2)
        usage();

    while ((arg = getopt(argc, argv, "ph")) != -1) {
        switch(arg) {
            case 'p':
                parent = 1;
                break;
            default:
                usage();
        }
    }

    init_xcb(&conn);

    while (optind < argc) {
        if (parent) {
            /* kills the client whose WID belongs to */
            xcb_kill_client(conn, strtoul(argv[optind++], NULL, 16));
        } else {
            /* destroy the given window and all its children */
            xcb_destroy_window(conn, strtoul(argv[optind++], NULL, 16));
        }
    }

    xcb_aux_sync(conn);
    kill_xcb(&conn);

    return 0;
}

static void usage (void) {
    die("usage: will [-p] <wid> [wid...]\n");
}
