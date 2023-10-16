CC = i686-elf-gcc
AS = as
ASM = nasm
CONFIG = ./config
GCCPARAMS = -m32 -I/include -fno-omit-frame-pointer -nostdlib -fno-pic -fno-builtin -fno-exceptions -ffreestanding -fno-leading-underscore -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long

ASPARAMS = --32
LDPARAMS = -m elf_i386 -T $(CONFIG)/linker.ld -nostdlib -Map LDout.map
SCRIPTS_DIR = script
HDR_DIR = include
# Define source and object directory for each subdirectory
OBJ_DIR = obj
SRC_DIRS = arch kernel drivers fs libk ui utils install
OBJ_DIRS = $(addprefix $(OBJ_DIR)/,$(SRC_DIRS))

# Create object directories if they don't exist
$(shell mkdir -p $(OBJ_DIRS))

# Collect all source files recursively within each subdirectory
SRC_FILES_C = $(wildcard $(addsuffix /*.c,$(SRC_DIRS)))
SRC_FILES_S = $(wildcard $(addsuffix /*.s,$(SRC_DIRS)))
SRC_FILES_ASM = $(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))

OBJ_FILES_C = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES_C))
OBJ_FILES_S = $(patsubst %.s, $(OBJ_DIR)/%.o, $(SRC_FILES_S))
OBJ_FILES_ASM = $(patsubst %.asm, $(OBJ_DIR)/%.o, $(SRC_FILES_ASM))

# Define targets for compiling C, Assembly, and linking
$(OBJ_DIR)/%.o: %.c
	$(CC) $(GCCPARAMS) -I$(HDR_DIR) -c -o $@ $<

$(OBJ_DIR)/%.o: %.s
	$(AS) $(ASPARAMS) -o $@ $<

$(OBJ_DIR)/%.o: %.asm
	$(ASM) -f elf32 -o $@ $<

# Define the all target and dependencies
all: libc-f HackOS.bin user
libc:
	 (cd libc && make)


HackOS.bin: $(OBJ_FILES_C) $(OBJ_FILES_S) $(OBJ_FILES_ASM)
	ld $(LDPARAMS) -o $@ $^ lib/libc.a

# Output directory
OUT_DIR = build

# Copy the output binary to the build directory
COPY_OUTPUT = cp HackOS.bin $(OUT_DIR)/

run-script:
	sudo (cd script && bash run.sh)
run: HackOS.bin
	bash ./run.sh
	qemu-system-i386 -name "INSTALLED" -drive file=AthenX.img,format=raw  -vga std -device intel-hda  -device ac97 -soundhw pcspk -m 4G -netdev user,id=network0 -device e1000,netdev=network0,mac=52:5E:56:12:34:56

clean:
	rm -rf $(OUT_DIR)/*.bin $(OUT_DIR)/*.map $(OUT_DIR)/*.img $(OBJ_DIR)/*
	rm -rf scripts/*.img
	rm -rf *.img


libc-f:
	(cd libc && make)
user:
	(cd prog && make)

lib-c:
	(cd libc && make clean)

user-c:
	(cd prog && make clean)