CC = gcc
CFLAGS = -Wall -Wextra -O2
SOURCES = part11_s.c
EXECUTABLES = part11_s


all: $(EXECUTABLES)

part11_s: part11_s.c
		$(CC) $(CFLAGS) -o part11_s part11_s.c

clear:
	rm -f $(EXECUTABLES)

.PHONY: all clear