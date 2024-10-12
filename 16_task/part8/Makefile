CC = gcc
CFLAGS = -Wall -Wextra -O2
SOURCES = part8_c.c part8_s.c
EXECUTABLES = part8_c part8_s


all: $(EXECUTABLES)

part8_c: part8_c.c
		$(CC) $(CFLAGS) -o part8_c part8_c.c

part8_s: part8_s.c
		$(CC) $(CFLAGS) -o part8_s part8_s.c

clear:
	rm -f $(EXECUTABLES)

.PHONY: all clear