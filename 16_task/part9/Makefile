CC = gcc
CFLAGS = -Wall -Wextra -O2
SOURCES = part9_c.c part9_s.c
EXECUTABLES = part9_c part9_s


all: $(EXECUTABLES)

part9_c: part9_c.c
		$(CC) $(CFLAGS) -o part9_c part9_c.c

part9_s: part9_s.c
		$(CC) $(CFLAGS) -o part9_s part9_s.c

clear:
	rm -f $(EXECUTABLES)

.PHONY: all clear