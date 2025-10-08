CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
AR = ar
ARFLAGS = rcs

SRCS = binary_tree_static.c

OBJS = $(SRCS:.c=.o)

LIBRARY = libbinary_tree.a


library: $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^


%.o:%.c  binary_tree_static.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(LIBRARY)

.PHONY: library clean
