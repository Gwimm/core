#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <err.h>
#include <getopt.h>

#include "com.h"

static xcb_connection_t *conn;

static void usage(void);

int main(int argc, char **argv) {
    xcb_window_t win;
    uint32_t values[1];
    char arg;

    if (argc != 3)
        usage();

    init_xcb(&conn);

    win = strtoul(argv[2], NULL, 16);
    if (!win)
        return 1;

    while ((arg = getopt(argc, argv, "rlih")) != -1) {
        switch(arg) {
            case 'r':
                values[0] = XCB_STACK_MODE_ABOVE;
                break;
            case 'l':
                values[0] = XCB_STACK_MODE_BELOW;
                break;
            case 'i':
                values[0] = XCB_STACK_MODE_OPPOSITE;
                break;
            default:
                usage();
                break;
        }
    }

    xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, values);

    xcb_aux_sync(conn);

    kill_xcb(&conn);

    return 0;
}

static void usage(void) {
    die("usage: sw -rli <wid>\n");
}
