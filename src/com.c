#include <stdio.h>
#include <stdlib.h>

#include "com.h"

void die(const char *errstr, ...) {
    va_list ap;

    va_start(ap, errstr);
    vfprintf(stderr, errstr, ap);
    va_end(ap);
    exit(1);
}

void version(void) {
    puts("split-"VERSION);
    exit(0);
}
