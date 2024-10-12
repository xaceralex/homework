CC = gcc
CFLAGS = -Wall -Wextra -O2
SOURCES = part10_c_tcp.c part10_c_udp.c part10_s.c
EXECUTABLES = part10_c_tcp part10_c_udp part10_s


all: $(EXECUTABLES)

part10_c_tcp: part10_c_tcp.c
		$(CC) $(CFLAGS) -o part10_c_tcp part10_c_tcp.c

part10_c_udp: part10_c_udp.c
		$(CC) $(CFLAGS) -o part10_c_udp part10_c_udp.c

part10_s: part10_s.c
		$(CC) $(CFLAGS) -o part10_s part10_s.c

clear:
	rm -f $(EXECUTABLES)

.PHONY: all clear