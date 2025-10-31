CC=gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

main: main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

debug: CFLAGS += -fsanitize=address -g
debug: LDFLAGS += -fsanitize=address 
debug: main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	$(RM) main.o debug main
.PHONY: clean
