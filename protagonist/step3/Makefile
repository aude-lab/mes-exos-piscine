CC= gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
LDLIBS= -lm
SRCS= compute_price.c compute_quantity.c my_price.c
OBJS= $(SRCS:.c=.o)
TARGET= my_price

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDLIBS)

clean:
	$(RM) $(OBJS) $(TARGET)

