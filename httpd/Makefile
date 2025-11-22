CC = cc
CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
TARGET = httpd
SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests

all: $(TARGET)

$(TARGET):
	@echo "Building $(TARGET)..."
	$(MAKE) -C $(SRC_DIR) CC="$(CC)" CFLAGS="$(CFLAGS)"

clean:
	@echo "Cleaning project..."
	$(MAKE) -C $(SRC_DIR) clean
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

check:
	@echo "Running tests..."
	$(MAKE) -C $(TESTS_DIR) check

test-deps:
	$(MAKE) -C $(TESTS_DIR) deps

debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

.PHONY: all clean check $(TARGET)

