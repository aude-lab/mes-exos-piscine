CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
AR = ar
ARFLAGS = rcs

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libbinary_tree.a

.PHONY: all library clean

all: library

library: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIB)
