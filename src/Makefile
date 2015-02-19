CC = gcc
LD = gcc
CFLAGS = -Wall -W -Werror
LDFLAGS =
EXE = kiwi
HEADERS = $(wildcard *.h)
CFILES = $(wildcard *.c)
OBJS = $(CFILES: .c=.o)

.PHONY: all clean mrproper

all: $(EXE)

$(EXE): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

Makefile.dep : $(CFILES) $(HEADERS)
	$(CC) -MM $(CFILES) > $@

clean :
	$(RM) $(OBJS) Makefile.dep

mrproper : clean
	$(RM) $(EXE)

include Makefile.dep
