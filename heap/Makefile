CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
TARGET = libheap.a
OBJ = create.o add.o pop.o del.o print.o

all: library

library: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)

create.o: create.c heap.h
	$(CC) $(CFLAGS) -c create.c -o create.o

add.o: add.c heap.h
	$(CC) $(CFLAGS) -c add.c -o add.o

pop.o: pop.c heap.h
	$(CC) $(CFLAGS) -c pop.c -o pop.o

del.o: del.c heap.h
	$(CC) $(CFLAGS) -c del.c -o del.o

print.o: print.c heap.h
	$(CC) $(CFLAGS) -c print.c -o print.o

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all library clean
