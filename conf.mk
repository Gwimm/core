CC ?= 	cc
LD = 	$(CC)

SRCPREFIX ?= src
BUILDPREFIX ?= build
DOCPREFIX ?= doc

PREFIX = /usr
MANPREFIX = $(PREFIX)/share/man
BINPREFIX = $(PREFIX)/bin

VERSION = $(shell git describe --always --dirty)
OPTFLAGS = -Os
CFLAGS  = $(OPTFLAGS) \
		  -DVERSION=\"$(VERSION)\"

LDLIBS = $(shell pkg-config --libs xcb-util)
LDFLAGS = -s \
		  $(LDLIBS)
