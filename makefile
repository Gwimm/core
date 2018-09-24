CC ?= 	cc
LD = 	$(CC)

OPTFLAGS = -Os
CFLAGS  = $(OPTFLAGS) \
		  -std=c99 -pedantic -Wall
LDFLAGS = -lxcb -lxcb-util

SRCPREFIX ?= src
BUILDPREFIX ?= build
DOCPREFIX ?= doc

PREFIX = /usr
MANPREFIX = $(PREFIX)/share/man
BINPREFIX = $(PREFIX)/bin

# BINPREFIX = ~/bin/prog

MAN := $(wildcard $(DOCPREFIX)/*.1)

SRC := $(wildcard $(SRCPREFIX)/*.c)
OBJ := $(subst $(SRCPREFIX), $(BUILDPREFIX), $(SRC:.c=.o))

TARGETS := $(filter-out $(BUILDPREFIX)/com, $(OBJ:.o=))
DEPS := $(BUILDPREFIX)/com.o

all: prepare build
	
build: $(TARGETS)

$(TARGETS): $(OBJ)
	$(LD) -o $@ $(DEPS) $@.o $(LDFLAGS)

$(BUILDPREFIX)/%.o: $(SRCPREFIX)/%.c
	$(CC) -o $@ $(CFLAGS) -c $<

prepare:
	mkdir -p $(BUILDPREFIX)

clean:
	-rm -r $(BUILDPREFIX)

install:
	install -t $(DESTDIR)$(BINPREFIX) $(TARGETS)
	install -t $(DESTDIR)$(MANPREFIX) $(MAN)

uninstall:
	-rm $(DESTDIR)$(subst $(BUILDPREFIX), $(BINPREFIX), $(TARGETS))
	-rm $(DESTDIR)$(subst $(DOCPREFIX), $(MANPREFIX), $(MAN))
