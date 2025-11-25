CC = cc
CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
TARGET = httpd
MODULES = config utils http daemon server logger
BUILD_DIR = ../build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/BIN

MODULE_OBJS = $(addprefix $(OBJ_DIR)/, \
	config/config.o \
	utils/string/string.o \
	http/http.o http/response.o \
	daemon/daemon.o \
	server/server.o \
	logger/logger.o \
	main.o)

.PHONY: all clean $(MODULES)

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(MODULE_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	mv $(BIN_DIR)/$(TARGET) ../

$(MODULES):
	$(MAKE) -C $@ CC="$(CC)" CFLAGS="$(CFLAGS)" OBJ_DIR="$(OBJ_DIR)"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
	for dir in $(MODULES); do \
		$(MAKE) -C $$dir clean; \
	done

$(MODULE_OBJS): $(MODULES)
