# Makefile for libc

CC = i686-elf-gcc
CFLAGS = -m32 -nostdlib -nostartfiles -nodefaultlibs -ffreestanding -Iinclude

SRC_DIR = ../libc
BUILD_DIR = obj

# List all source files in the SRC_DIR
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Generate corresponding object files in the BUILD_DIR
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Name of the resulting library
LIB = libc.a

# Compile and create the static library
$(LIB): $(OBJ_FILES)
	ar rcs $@ $^

# Compile each source file to an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)/*.o $(LIB)
