CC ?= 	cc
LD = 	$(CC)
STRIP = strip

SRCPREFIX ?= src
BUILDPREFIX ?= build
DOCPREFIX ?= doc

PREFIX = /usr
MANPREFIX = $(PREFIX)/share/man
BINPREFIX = $(PREFIX)/bin

OPTFLAGS = -Os
CFLAGS  = $(OPTFLAGS)

LDLIBS = $(shell pkg-config --libs xcb-util)
LDFLAGS = $(LDLIBS)
