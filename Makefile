CC = gcc

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Base Flags
BASE_CFLAGS = -Wall -Wextra -g -I$(INC_DIR)
BASE_LDFLAGS = -lssl -lcrypto 

# Platform-Specific Flags
DARWIN_CFLAGS = $(BASE_CFLAGS)
DARWIN_LDFLAGS = $(BASE_LDFLAGS)

LINUX_CFLAGS = $(BASE_CFLAGS)
LINUX_LDFLAGS = $(BASE_LDFLAGS)

TEST_CFLAGS = $(BASE_CFLAGS) -DBIP39_MAX_ENTROPY_SIZE=512 -D__BIP39_TESTS__
TEST_LDFLAGS = $(BASE_LDFLAGS)

SRC_FILES = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

NAME = KeyForcer
NAME_TEST = Test

# General rule to compile any object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile for Darwin (macOS)
darwin: CFLAGS = $(DARWIN_CFLAGS)
darwin: LDFLAGS = $(DARWIN_LDFLAGS)
darwin: $(BIN_DIR)/$(NAME)

# Rule to compile for Linux
linux: CFLAGS = $(LINUX_CFLAGS)
linux: LDFLAGS = $(LINUX_LDFLAGS)
linux: $(BIN_DIR)/$(NAME)

# Rule to compile for testing
test: CFLAGS = $(TEST_CFLAGS)
test: LDFLAGS = $(TEST_LDFLAGS)
test: $(BIN_DIR)/$(NAME_TEST)

$(BIN_DIR)/$(NAME): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/$(NAME_TEST): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/*

re: clean all

.PHONY: all darwin linux test clean re

