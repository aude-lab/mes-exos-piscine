CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
LDFLAGS = -lcriterion
LDLIBS = -fsanitize=address -g

SRCS = src/tinyprintf.c

OBJ = $(SRCS: .c=.o)

TEST_SUITE = tests/tests_tinyprintf.c
BIN_TEST = test_tinyprintf

all: src/tinyprintf.o

check: $(BIN_TEST)
	./$(BIN_TEST)

$(BIN_TEST): $(TEST_SUITE)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $(TEST_SUITE) $(SRCS)


clean:
	rm -rf $(BIN_TEST)
