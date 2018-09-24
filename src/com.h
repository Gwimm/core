#ifndef COM_H
#define COM_H

void init_xcb(xcb_connection_t **);
void kill_xcb(xcb_connection_t **);

void get_screen(xcb_connection_t *, xcb_screen_t **);
int get_windows(xcb_connection_t *, xcb_window_t, xcb_window_t **);

int exists(xcb_connection_t *, xcb_window_t);
int mapped(xcb_connection_t *, xcb_window_t);
int ignore(xcb_connection_t *, xcb_window_t);

void die(const char *errstr, ...);

#endif // COM_H
