CC = gcc
CFLAGS = -Wall -Wextra -O2
SOURCES = part4_c.c part4_s.c
EXECUTABLES = part4_c part4_s


all: $(EXECUTABLES)

part4_c: part4_c.c
		$(CC) $(CFLAGS) -o part4_c part4_c.c

part4_s: part4_s.c
		$(CC) $(CFLAGS) -o part4_s part4_s.c

clear:
	rm -f $(EXECUTABLES)

.PHONY: all clear