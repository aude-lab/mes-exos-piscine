CC=gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
LDFLAGS=

main: main.o
	$(CC) $(LDFLAGS) -o $@ $^

debug: CFLAGS += -fsanitize=address -g
debug: LDFLAGS += -fsanitize=address -g
debug: main

clean:
	rm -rf main.o main
.PHONY: debug clean
