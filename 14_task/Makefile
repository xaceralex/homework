CC = gcc
CFLAGS = -Wall -Werror

LDFLAGS_C = -lncursesw -lpthread
LDFLAGS_S = -lpthread

SOURCES = part1_sv_s.c part1_sv_c.c part1_px_s.c part1_px_c.c part2_s.c part2_c.c

EXECUTABLES = part1_sv_s part1_sv_c part1_px_s part1_px_c part2_s part2_c

all: $(EXECUTABLES)

%: %.c
		$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS_S)

part2_c: part2_c.c
		$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS_C)

part2_s: part2_s.c
		$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS_S)

clean:
		rm -f $(EXECUTABLES)