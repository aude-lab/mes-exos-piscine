CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
TARGET = sieve.o

all: $(TARGET)

sieve.o: sieve.c
	$(CC) $(CFLAGS) -c sieve.c -o sieve.o

clean:
	rm -f $(TARGET)

.PHONY: all clean
