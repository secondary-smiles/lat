NAME:=lat

SRCDIR:=src
IDIR:=include
ODIR:=obj
BINDIR:=build

CC:=cc
CFLAGS:=-I$(IDIR) -Wall -Wextra -pedantic -Ofast
LIB:=

DEPS:=$($(IDIR)/%.h)

SRCS=$(shell find . -name "*.c")
BASENAME:=$(shell basename -a $(SRCS))

VPATH=$(dir $(SRCS))

OBJ=$(patsubst %.c,./$(ODIR)/%.o,$(BASENAME))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIB)

$(NAME): $(OBJ)
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LIB)

.PHONY: prep
prep:
	-@mkdir -p $(SRCDIR)
	-@mkdir -p $(IDIR)
	-@mkdir -p $(ODIR)
	-@mkdir -p $(BINDIR)
	@echo $(BINDIR)/ >> .gitignore

.PHONY: clean
clean:
	-rm -f $(ODIR)/*
	-rm -f $(BINDIR)/*
