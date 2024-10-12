CC = gcc
CFLAGS = -Wall -Wextra -O2
SOURCES = part12_c.c part12_s.c
EXECUTABLES = part12_c part12_s


all: $(EXECUTABLES)

part12_c: part12_c.c
		$(CC) $(CFLAGS) -o part12_c part12_c.c

part12_s: part12_s.c
		$(CC) $(CFLAGS) -o part12_s part12_s.c

clear:
	rm -f $(EXECUTABLES)

.PHONY: all clear