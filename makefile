include conf.mk

MAN := $(wildcard $(DOCPREFIX)/*.1)

SRC := $(wildcard $(SRCPREFIX)/*.c)
OBJ := $(subst $(SRCPREFIX), $(BUILDPREFIX), $(SRC:.c=.o))

TARGETS := $(filter-out $(BUILDPREFIX)/com $(BUILDPREFIX)/xcb, $(OBJ:.o=))
DEPS := $(filter-out $(TARGETS:=.o), $(OBJ))

all: prepare build strip
	
build: $(TARGETS)

strip:
	$(STRIP) $(TARGETS)

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
