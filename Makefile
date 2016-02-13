VERSION   := git-20151228

PREFIX    := /usr/local
MANPREFIX := $(PREFIX)/share/man

CC        ?= gcc
CFLAGS    += -std=c99 -Wall -pedantic
CPPFLAGS  += -I$(PREFIX)/include -D_XOPEN_SOURCE=700
LDFLAGS   += -L$(PREFIX)/lib
LIBS      := -lX11 -lImlib2 -lm

# optional dependencies:
# giflib: gif animations
	CPPFLAGS += -DHAVE_GIFLIB
	LIBS     += -lgif
# libexif: jpeg auto-orientation, exif thumbnails
	CPPFLAGS += -DHAVE_LIBEXIF
	LIBS     += -lexif


.PHONY: clean install uninstall

SRC := commands.c image.c main.c options.c thumbs.c util.c window.c
DEP := $(SRC:.c=.d)
OBJ := $(SRC:.c=.o)

all: config.h sxiv

$(OBJ): Makefile

-include $(DEP)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -DVERSION=\"$(VERSION)\" -MMD -MP -c -o $@ $<

median.o: median.cpp
	$(CC) --std=c++1y $(CPPFLAGS) -DVERSION=\"$(VERSION)\" -MMD -MP -c -o $@ $<

config.h:
	cp config.def.h $@

sxiv:	$(OBJ) median.o
	$(CC) $(LDFLAGS) -o $@ $(OBJ) median.o $(LIBS)

clean:
	rm -f $(OBJ) $(DEP) sxiv

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp sxiv $(DESTDIR)$(PREFIX)/bin/
	chmod 755 $(DESTDIR)$(PREFIX)/bin/sxiv
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s!PREFIX!$(PREFIX)!g; s!VERSION!$(VERSION)!g" sxiv.1 > $(DESTDIR)$(MANPREFIX)/man1/sxiv.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/sxiv.1
	mkdir -p $(DESTDIR)$(PREFIX)/share/sxiv/exec
	cp exec/* $(DESTDIR)$(PREFIX)/share/sxiv/exec/
	chmod 755 $(DESTDIR)$(PREFIX)/share/sxiv/exec/*

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/sxiv
	rm -f $(DESTDIR)$(MANPREFIX)/man1/sxiv.1
	rm -rf $(DESTDIR)$(PREFIX)/share/sxiv
