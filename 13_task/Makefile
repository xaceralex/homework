CC = gcc
CFLAGS = -Wall -Wextra -O2
SOURCES = part2_c.c part2_s.c
EXECUTABLES = part2_c part2_s


all: $(EXECUTABLES)

part2_c: part2_c.c
		$(CC) $(CFLAGS) -o part2_c part2_c.c

part2_s: part2_s.c
		$(CC) $(CFLAGS) -o part2_s part2_s.c

clear:
	rm -f $(EXECUTABLES)

.PHONY: all clears