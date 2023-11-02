# Makefile for libc

CC = i686-elf-gcc
CFLAGS = -m32 -nostdlib -nostartfiles -nodefaultlibs -ffreestanding -Iinclude

SRC_DIR = ../libc
BUILD_DIR = obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
DEST_DIR = ../../sysroot/usr/source
DEST_FILES = $(patsubst $(SRC_DIR)/%,$(DEST_DIR)/%,$(SRC_FILES))
all: libc.a $(DEST_FILES)
	cp -RT /include ../../sysroot/usr/include
	cp libc.a ../../sysroot/usr/libc
# List all source files in the SRC_DIR


# Generate corresponding object files in the BUILD_DIR
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Name of the resulting library
LIB = libc.a

# Compile and create the static library
$(LIB): $(OBJ_FILES)
	ar rcs $@ $^

$(DEST_DIR)/%.c: $(SRC_DIR)/%.c | $(DEST_DIR)
	cp $^ $@
# Compile each source file to an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts

	

clean:
	rm -rf $(BUILD_DIR)/*.o $(LIB)
