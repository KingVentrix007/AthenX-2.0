# Define the compiler and flags
CC = i686-elf-gcc
CFLAGS = -m32 -std=gnu99 -ffreestanding -DATHENX_LIBC -O2 -O0 -nostdlib -Ilibc/
LDFLAGS = -T linker.ld -nostdlib
LIBS = ../obj/arch/syscall.o libc/libc.a

# Find all subdirectories in the current directory
SUBDIRS := $(wildcard */)

# Filter out any directories you want to exclude (if necessary)
EXCLUDE_DIRS := libc/  editor/ test/ kilo/
SUBDIRS := $(filter-out $(EXCLUDE_DIRS),$(SUBDIRS))

# List of source files to compile in each subdirectory
SRC_FILES := $(wildcard $(addsuffix *.c, $(SUBDIRS)))

# List of corresponding output binary names (the name of the folder in "user")
OUT_FILES := $(patsubst %/,%,$(SUBDIRS))

# List of corresponding ".1" files with the same name as the folder
MAN_FILES := $(addsuffix .1, $(OUT_FILES))

# Directories for output binaries and man pages
OUTPUT_DIR = ../programs
MAN_DIR = ../man

# Full paths to output binaries and man pages
OUTPUT_PATHS := $(addprefix $(OUTPUT_DIR)/, $(OUT_FILES))
MAN_PAGES := $(addprefix $(MAN_DIR)/, $(MAN_FILES))

# Targets to build all programs and man pages
all: $(OUTPUT_PATHS) $(MAN_PAGES)

# Rule to build each program from its source files
$(OUTPUT_DIR)/%: %/*.c | $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -DATHENX_LIBC $^ init.c -o $@ $(LDFLAGS) $(LIBS)

# Rule to copy man pages to the man directory with the same name as the folder
$(MAN_DIR)/%.1: %/*.1 | $(MAN_DIR)
	cp $^ $@

# Create the output directories if they don't exist
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(MAN_DIR):
	mkdir -p $(MAN_DIR)

# Clean rule to remove generated files
clean:
	rm -f $(OUTPUT_PATHS) $(MAN_PAGES)

.PHONY: all clean
