CC = gcc
CFLAGS = -Wall -g
TARGETS = part3_c part3_s
SRCS = part3_c.c part3_s.c part3_lib.c
OBJS = $(SRCS:.c=.o)

all: $(TARGETS)

part3_c: part3_c.o part3_lib.o
	$(CC) $(CFLAGS) -o $@ $^

part3_s: part3_s.o part3_lib.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c part3_lib.h
	$(CC) $(CFLAGS) -c $<

clear:
	rm -f $(OBJS) $(TARGETS)