CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

all: fifo.o

fifo.o: fifo.c fifo.h
	$(CC) $(CFLAGS) -c fifo.c -o fifo.o

clean:
	rm -f fifo.o

