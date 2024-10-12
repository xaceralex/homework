CC = gcc
CFLAGS = -Wall -Wextra -O2
SOURCES = part1_c.c part1_s.c
EXECUTABLES = part1_c part1_s


all: $(EXECUTABLES)

part1_c: part1_c.c
		$(CC) $(CFLAGS) -o part1_c part1_c.c

part1_s: part1_s.c
		$(CC) $(CFLAGS) -o part1_s part1_s.c

clear:
	rm -f $(EXECUTABLES)

.PHONY: all clear