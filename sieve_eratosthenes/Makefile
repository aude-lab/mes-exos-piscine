CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

all: sieve.o

sieve.o: sieve.c sieve.h
	$(CC) $(CFLAGS) -c sieve.c -o sieve.o

clean:
	rm -f sieve.o

