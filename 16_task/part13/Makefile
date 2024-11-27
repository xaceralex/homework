CC=gcc
CFLAGS=-Wall -Wextra

all: part13_c part13_s

part13_c: part13_c.o
		$(CC) $(CFLAGS) -o part13_c part13_c.o

part13_s: part13_s.o
		$(CC) $(CFLAGS) -o part13_s part13_s.o

part13_c.o: part13_c.c
		$(CC) $(CFLAGS) -c part13_c.c

part13_s.o: part13_s.c
		$(CC) $(CFLAGS) -c part13_s.c

clean:
		rm -f *.o part13_c part13_s