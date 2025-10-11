CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -Wvla

SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

BIN = evalexpr

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

check: $(BIN)
	./tests/simple_test.sh

clean:
	$(RM) $(BIN) $(OBJS)

.PHONY: all check clean
