CC = cc
CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
OBJ_DIR ?= ../../build/obj/http
SRC_DIR = .
SOURCES = http.c response.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean

all: $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)
