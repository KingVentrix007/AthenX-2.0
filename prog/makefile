
CC = i686-elf-gcc
CFLAGS = -m32 -std=gnu99 -ffreestanding -O2 -nostdlib -I../libc/
LDFLAGS = -T linker.ld -nostdlib

PROGRAM = test

# Path to the folder containing object files
OBJ_DIR = ../obj

# Include all .o files from subfolders of OBJ_DIR
KERNEL_OBJS = $(shell find $(OBJ_DIR) -name '*.o')

SRCS = test.c
OBJS = $(SRCS:.c=.o)

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ ../obj/arch/syscall.o ../libc/libc.a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(PROGRAM)