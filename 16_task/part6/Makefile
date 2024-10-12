CC = gcc
CFLAGS = -Wall -Wextra -O2
SOURCES = part6_c.c part6_s.c
EXECUTABLES = part6_c part6_s


all: $(EXECUTABLES)

part6_c: part6_c.c
		$(CC) $(CFLAGS) -o part6_c part6_c.c

part6_s: part6_s.c
		$(CC) $(CFLAGS) -o part6_s part6_s.c

clear:
	rm -f $(EXECUTABLES)

.PHONY: all clear