CC = gcc
AS = as
ASM = nasm
CONFIG = ./config
GCCPARAMS = -m32 -fno-omit-frame-pointer -nostdlib -fno-pic -fno-builtin -fno-exceptions -ffreestanding -fno-leading-underscore -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long

ASPARAMS = --32
LDPARAMS = -m elf_i386 -T $(CONFIG)/linker.ld --allow-multiple-definition -nostdlib -Map LDout.map

SRC_DIR = src
HDR_DIR = include/
OBJ_DIR = obj
START_ASM = asm/
OBJ_ASM = $(OBJ)/asm
SRC_FILES1 = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES1 = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES1))
SRC_FILES2 = $(wildcard $(SRC_DIR)/*.s)
OBJ_FILES2 = $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(SRC_FILES2))
SRC_FILES3 = $(wildcard $(SRC_DIR)/*.asm)
OBJ_FILES3 = $(patsubst $(SRC_DIR)/%.asm, $(OBJ_DIR)/%.o, $(SRC_FILES3))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(GCCPARAMS) $^ -I$(HDR_DIR) -c -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	$(AS) $(ASPARAMS) -o $@ $<

$(OBJ_ASM)/%.o: $(START_ASM)/%.asm
	nasm -f elf32 -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	nasm -f elf32 -o $@ $<

all: AthenX.bin

AthenX.bin: $(OBJ_FILES1) $(OBJ_FILES2) $(OBJ_FILES3)
	ld $(LDPARAMS) -o $@ $(OBJ_DIR)/*.o

iso: AthenX.bin
	cp AthenX.bin iso/boot/AthenX.bin
	cp LDout.map iso/boot/grub/
	./calculate_size_and_write.sh
#sudo grub-mkrescue --output=HackOS.iso iso

run: iso MAIN
	truncate -s 0 pipe
	bash ./run.sh
	qemu-system-i386 -name "MAIN" -drive file=AthenX.img,format=raw -serial pipe:pipe -vga std -device intel-hda  -device ac97 -m 4G -netdev user,id=network0 -device e1000,netdev=network0,mac=52:5E:56:12:34:56 > /dev/null 2>&1
#	@make stop_pulseaudio
run-fat: iso
	bash ./run.sh
	qemu-system-i386 -name "INSTALLED" -drive file=AthenX.img,format=raw  -serial pipe:pipe -vga std -device intel-hda  -device ac97 -soundhw pcspk -m 4G -netdev user,id=network0 -device e1000,netdev=network0,mac=52:5E:56:12:34:56
run-m:
	qemu-system-i386 -name "MAIN" -drive file=AthenX.img,format=raw -drive file=my_disk_image.img,format=raw -serial pipe:pipe -vga std -device intel-hda  -device ac97 -m 4G -netdev user,id=network0 -device e1000,netdev=network0,mac=52:5E:56:12:34:56
start_pulseaudio:
	@echo "Starting PulseAudio..."
	pulseaudio --start

stop_pulseaudio:
	@echo "Stopping PulseAudio..."
	pulseaudio --kill

SECCOND:
	qemu-img create SECCOND.img 1G

MAIN:
	qemu-img create MAIN.img 1G

clean:
	rm -f *.o HackOS HackOS.iso AthenX.bin $(OBJ_DIR)/*.o
	rm -f AthenX.img

version+:
	./update_version "turd"
	echo 'Version updated'

changelog-test:
	gcc update_changelog.c -o update_changelog
	./update_changelog

changelog:
	./update_changelog

generate_hexdump:
	hexdump -C $(FILE1) > $(FILE1_HEX)
	hexdump -C $(FILE2) > $(FILE2_HEX)

compare_hexdump: generate_hexdump
	diff -u --suppress-common-lines $(FILE1_HEX) $(FILE2_HEX) |  tee $(DIFF_OUTPUT)

clean-hex:
	rm -f $(FILE1_HEX) $(FILE2_HEX) $(DIFF_OUTPUT)
	
# Define the image file and folder
IMG_FILE = AthenX.img
INSPECT_FOLDER = inspect


# Target to mount the image
mount-img:
	@echo "Mounting the image to $(INSPECT_FOLDER)..."
	@if [ ! -d "$(INSPECT_FOLDER)" ]; then mkdir $(INSPECT_FOLDER); fi
	@sudo mount -o loop $(IMG_FILE) $(INSPECT_FOLDER)

# Target to unmount the image
unmount-img:
	@echo "Unmounting the image from $(INSPECT_FOLDER)..."
	@sudo umount $(INSPECT_FOLDER)

# Target to list the contents of the folder
list-contents:
	@echo "Listing contents of $(INSPECT_FOLDER):"
	@ls -l $(INSPECT_FOLDER)
