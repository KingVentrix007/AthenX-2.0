#!/bin/bash

# Define the compiler and flags
CC="i686-elf-gcc"
CFLAGS="-m32 -std=gnu99 -ffreestanding -O2 -O0 -nostdlib -Ilibc/"
LDFLAGS="-T linker.ld -nostdlib"
LIBS="../obj/arch/syscall.o libc/libc.a"

# Specify the output directory
OUTPUT_DIR="../programs"

# Choose the compiler based on the argument
if [ "$1" == "athenX" ]; then
    echo "Compiling using athenX options"
    compiler="$CC"
    flags="$CFLAGS"
else
    echo "Compiling using the default compiler, linker, and libraries"
    compiler="gcc"
    flags=""
fi

# Find all subdirectories in the current directory
SUBDIRS=$(find . -maxdepth 1 -type d -exec basename {} \;)

# Filter out any directories you want to exclude (if necessary)
EXCLUDE_DIRS="libc kilo"
SUBDIRS=($(comm -23 <(echo "${SUBDIRS[*]}" | tr ' ' '\n' | sort) <(echo "$EXCLUDE_DIRS" | tr ' ' '\n' | sort)))

# Iterate through subdirectories and compile
for dir in "${SUBDIRS[@]}"; do
    source_files=("$dir"/*.c)
    if [ -n "$source_files" ]; then
        output_file="$OUTPUT_DIR/$dir"
        echo "Compiling $dir to $output_file"
        $compiler $flags "${source_files[@]}" -o "$output_file" $LDFLAGS $LIBS
    fi
done
