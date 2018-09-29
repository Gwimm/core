include conf.mk

MAN := $(wildcard $(DOCPREFIX)/*.1)

SRC := $(wildcard $(SRCPREFIX)/*.c)
OBJ := $(subst $(SRCPREFIX), $(BUILDPREFIX), $(SRC:.c=.o))

TARGETS := $(filter-out $(BUILDPREFIX)/com $(BUILDPREFIX)/xcb, $(OBJ:.o=))
DEPS := $(filter-out $(TARGETS:=.o), $(OBJ))

.PHONY: all clean

all: prepare build
	
build: $(TARGETS)

$(TARGETS): $(OBJ)
	@printf "[34mLD[0m :: $@\n"
	$(LD) -o $@ $(DEPS) $(@:=.o) $(LDFLAGS)

$(BUILDPREFIX)/%.o: $(SRCPREFIX)/%.c
	@printf "[32mCC[0m :: $@\n"
	$(CC) -o $@ $(CFLAGS) -c $<

prepare:
	@printf "[32creating build directory[0m\n"
	-mkdir $(BUILDPREFIX)

clean:
	@printf "[32mcleaning, master uwu[0m\n"
	-rm -r $(BUILDPREFIX)

install:
	install -m 755 -D \
		$(subst $(BUILDPREFIX), $(BINPREFIX), $(TARGETS)) $(DESTDIR)$(BINPREFIX)
	install -m 655 -D \
		$(subst $(DOCPREFIX), $(MANPREFIX), $(MAN)) $(DESTDIR)$(MANPREFIX)

uninstall:
	-rm \
		$(DESTDIR)$(subst $(BUILDPREFIX), $(BINPREFIX), $(TARGETS))
	-rm \
		$(DESTDIR)$(subst $(DOCPREFIX), $(MANPREFIX), $(MAN))
