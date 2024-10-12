CC = gcc
CFLAGS = -Wall -Wextra -O2
SOURCES = part7_c.c part7_s.c
EXECUTABLES = part7_c part7_s


all: $(EXECUTABLES)

part7_c: part7_c.c
		$(CC) $(CFLAGS) -o part7_c part7_c.c


part7_s: part7_s.c
		$(CC) $(CFLAGS) -o part7_s -lpthread part7_s.c

clear:
	rm -f $(EXECUTABLES)

.PHONY: all clear