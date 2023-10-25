# Define the compiler and flags
CC = i686-elf-gcc
CFLAGS = -m32 -std=gnu99 -ffreestanding -O2 -O0 -nostdlib -Ilibc/
LDFLAGS = -T linker.ld -nostdlib
LIBS = ../obj/arch/syscall.o libc/libc.a

# Find all subdirectories in the current directory
SUBDIRS := $(wildcard */)

# Filter out any directories you want to exclude (if necessary)
EXCLUDE_DIRS := /libc/
SUBDIRS := $(filter-out $(EXCLUDE_DIRS),$(SUBDIRS))

# List of source files to compile in each subdirectory
SRC_FILES := $(wildcard $(addsuffix *.c, $(SUBDIRS)))

# List of corresponding output binary names
OUT_FILES := $(patsubst %/,%,$(SUBDIRS))

# Directory for output binaries
OUTPUT_DIR = ../programs

# Full paths to output binaries
OUTPUT_PATHS := $(addprefix $(OUTPUT_DIR)/, $(OUT_FILES))

# Targets to build all programs
all: $(OUTPUT_PATHS)

# Rule to build each program from its source files
$(OUTPUT_DIR)/%: %/*.c | $(OUTPUT_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

# Create the output directory if it doesn't exist
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# Clean rule to remove generated files
clean:
	rm -f $(OUTPUT_PATHS)

.PHONY: all clean
