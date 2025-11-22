CC = gcc
CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -g
SRCS = daemon.c
OBJS = $(SRCS:.c=.o)

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	
.PHONY: all clean
