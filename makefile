CC = i686-elf-gcc
AS = as
ASM = nasm
CONFIG = ./config
GCCPARAMS =  -O0 -I/include -fno-omit-frame-pointer -nostdlib -fno-pic -fno-builtin -fno-exceptions -ffreestanding -fno-leading-underscore -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long

ASPARAMS = --32
LDPARAMS = -m elf_i386 -T $(CONFIG)/linker.ld -nostdlib -Map LDout.map
SCRIPTS_DIR = script
HDR_DIR = include
# Define source and object directory for each subdirectory
OBJ_DIR = obj
SRC_DIRS = arch kernel emulator drivers fs libk ui utils installer security drivers/net network drivers/audio drivers/dma hal
OBJ_DIRS = $(addprefix $(OBJ_DIR)/,$(SRC_DIRS))

# Create object directories if they don't exist
$(shell mkdir -p $(OBJ_DIRS))

# Collect all source files recursively within each subdirectory
SRC_FILES_C := $(shell find $(SRC_DIRS) -type f -name '*.c')
SRC_FILES_S := $(shell find $(SRC_DIRS) -type f -name '*.s')
SRC_FILES_ASM := $(shell find $(SRC_DIRS) -type f -name '*.asm')

OBJ_FILES_C := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES_C))
OBJ_FILES_S := $(patsubst %.s, $(OBJ_DIR)/%.o, $(SRC_FILES_S))
OBJ_FILES_ASM := $(patsubst %.asm, $(OBJ_DIR)/%.o, $(SRC_FILES_ASM))

# ... (Rest of your Makefile)

# Define targets for compiling C, Assembly, and linking
# Define targets for compiling C, Assembly, and linking
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(GCCPARAMS) -I$(HDR_DIR) -c -o $@ $<

$(OBJ_DIR)/%.o: %.s
	@mkdir -p $(@D)
	$(AS) $(ASPARAMS) -o $@ $<

$(OBJ_DIR)/%.o: %.asm
	@mkdir -p $(@D)
	$(ASM) -f elf32 -o $@ $<



# Automatically generate dependencies for each subdirectory
define generate_deps
SRC_FILES_C += $(wildcard $(1)/*.c)
SRC_FILES_S += $(wildcard $(1)/*.s)
SRC_FILES_ASM += $(wildcard $(1)/*.asm)
OBJ_FILES_C += $(patsubst %.c, $(OBJ_DIR)/%.o, $(wildcard $(1)/*.c))
OBJ_FILES_S += $(patsubst %.s, $(OBJ_DIR)/%.o, $(wildcard $(1)/*.s))
OBJ_FILES_ASM += $(patsubst %.asm, $(OBJ_DIR)/%.o, $(wildcard $(1)/*.asm))
endef

$(foreach dir,$(SRC_DIRS),$(eval $(call generate_deps,$(dir))))

# Define the all target and dependencies
all: libc-f AthenX.bin user

libc:
	(cd porg/libc && make)

AthenX.bin: $(OBJ_FILES_C) $(OBJ_FILES_S) $(OBJ_FILES_ASM)
	ld $(LDPARAMS) -o $@ $^
	mv AthenX.bin out/

# Output directory
OUT_DIR = out

# Copy the output binary to the build directory
COPY_OUTPUT = cp AthenX.bin $(OUT_DIR)/

run-script:
	sudo (cd script && bash run.sh)

run: AthenX.bin
	bash ./run.sh
	qemu-system-i386 -name "INSTALLED" --no-reboot --no-shutdown -drive file=AthenX.img,format=raw -vga std -device sb16 -m 4G -device e1000,netdev=n1 -netdev user,id=n1,net=192.168.0.1/24,tftp=/home/king/AthenXDev/tftp -object filter-dump,id=f1,netdev=n1,file=dump.dat -M hpet=on -serial stdio

run-tap:
	bash ./run.sh
	qemu-system-i386 -name "INSTALLED" -drive file=AthenX.img,format=raw -vga std -device intel-hda -device ac97 -soundhw pcspk -m 4G -device e1000,netdev=n1 -netdev bridge,id=n1,br=Network\ Bridge -M hpet=on -serial stdio

clean:
	rm -rf $(OUT_DIR)/*.bin $(OUT_DIR)/*.map $(OUT_DIR)/*.img $(OBJ_DIR)/*
	rm -rf scripts/*.img
	rm -rf *.img
	sudo rm -rf inspect/

libc-f:
	(cd userspace/libc && make)
user:
	(cd userspace && make)

lib-c:
	(cd userspace/libc && make clean)

user-c:
	(cd userspace && make clean)

# curlty i need to add cases for each indvil subdir, can u make it automatic, so if it finds a subdir it will siply search in it
dis: $(OUT_DIR)/AthenX.bin
	objdump -D -b binary -m i386 $< > disassembly.asm


