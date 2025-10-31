CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
AR = ar
ARFLAGS = rcs

SRC = vector.c
OBJ = $(SRC:.c=.o)
LIB = libvector.a

.PHONY: all library clean

all: library

library: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c vector.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIB)
