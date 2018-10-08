SHELL   ?= /bin/sh

CC      ?= gcc
CFLAGS  += -march=native -O2 -pipe
CFLAGS  += -std=c99 -Wpedantic -Wall -Wextra -I$(PREFIX)/include
CFLAGS  += -D_POSIX_C_SOURCE=200809L
LDFLAGS	:= -L$(PREFIX)/lib

PREFIX    ?= /usr/local
BINPREFIX ?= $(PREFIX)/bin

SRC := $(notdir $(wildcard *.c))
OUT := $(basename $(SRC))

comma  ?= ,
empty  ?=
space  ?= $(empty) $(empty)
BINLST ?= $(subst $(space),$(comma),$(OUT))

all: CFLAGS  += -Os
all: LDFLAGS += -s
all: $(OUT)

include Sourcedeps

debug: CFLAGS += -O0 -g3 -DDEBUG
debug: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LIBS)

install:
	mkdir -p "$(DESTDIR)$(BINPREFIX)"
	cp -p $(OUT) "$(DESTDIR)$(BINPREFIX)"

uninstall:
	rm -f "$(DESTDIR)$(BINPREFIX)"/{$(BINLIST)}

clean:
	rm -f $(OUT)

.PHONY: all debug clean install uninstall
