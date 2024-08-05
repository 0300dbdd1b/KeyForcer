CC = gcc

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

CFLAGS = -Wall -Wextra -I$(INC_DIR)
LDFLAGS = -lcrypto

SRC_FILES = $(shell find $(SRC_DIR) -type f -name '*.c')

OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

NAME = KeyForcer

DARWIN_FLAGS =
LINUX_FLAGS = 

all: darwin

darwin: CFLAGS += $(DARWIN_FLAGS)
darwin: $(BIN_DIR)/$(NAME)

linux: CFLAGS += $(LINUX_FLAGS)
linux: $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/$(NAME)

.PHONY: all darwin linux clean

