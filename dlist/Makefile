CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
AR = ar
ARFLAGS = rcs

SRCS = dlist.c 

OBJS = $(SRCS:.c=.o)

LIBRARY = libdlist.a


library: $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^


%.o: %.c dlist.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIBRARY)

.PHONY: library clean
