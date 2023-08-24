NAME:=lat

SRCDIR:=src
IDIR:=include
ODIR:=obj
BINDIR:=build

CC:=cc
# OMG SO FAST (see https://www.shlomifish.org/humour/by-others/funroll-loops/Gentoo-is-Rice.html)
# CFLAGS:=-I$(IDIR) -Wall -Wextra -pedantic -Ofast -faggressive-loop-optimizations -funroll-all-loops  -march=native
# For a stable experience
CFLAGS:=-I$(IDIR) -Wall -Wextra -pedantic -O2 -march=native

LIB:=

DEPS:=$($(IDIR)/%.h)

SRCS=$(shell find . -name "*.c")
BASENAME:=$(shell basename -a $(SRCS))

VPATH=$(dir $(SRCS))

OBJ=$(patsubst %.c,./$(ODIR)/%.o,$(BASENAME))

$(ODIR)/%.o: %.c $(DEPS)
	-@mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIB)

$(NAME): $(OBJ)
	-@mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LIB)

.PHONY: prep
prep:
	-@mkdir -p $(SRCDIR)
	-@mkdir -p $(IDIR)
	-@mkdir -p $(ODIR)
	-@mkdir -p $(BINDIR)

.PHONY: clean
clean:
	-rm -f $(ODIR)/*
	-rm -f $(BINDIR)/*
