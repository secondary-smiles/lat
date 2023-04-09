NAME=main

SRCDIR=src
IDIR=include
ODIR=obj
BINDIR=build

CC=cc
CFLAGS=-I$(IDIR) -Wall -Wextra -pedantic
LIB=

_DEPS=
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=$(NAME).o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
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
