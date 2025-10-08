CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
LIBRARY = libdlist.a

SRCS = dlist_clear.c \
       dlist_concat.c \
       dlist_find.c \
       dlist_get.c \
       dlist_init.c \
       dlist_insert_at.c \
       dlist_levenshtein.c \
       dlist_map_square.c \
       dlist_print.c \
       dlist_push_back.c \
       dlist_push_front.c \
       dlist_remove_at.c \
       dlist_reverse.c \
       dlist_size.c \
       dlist_split_at.c

OBJS = $(SRCS:.c=.o)

all: $(LIBRARY)

$(LIBRARY): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIBRARY)

.PHONY: all clean
