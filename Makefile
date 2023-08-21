CC=gcc
AS=as
ASM = nasm
CONFIG = ./config
GCCPARAMS = -m32 -nostdlib -fno-pic -fno-builtin -fno-exceptions -ffreestanding -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -m elf_i386 -T $(CONFIG)/linker.ld -nostdlib --allow-multiple-definition

SRC_DIR=src
HDR_DIR=include/
OBJ_DIR=obj
START_ASM = asm/
OBJ_ASM = $(OBJ)/asm
SRC_FILES1=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES1=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES1))
SRC_FILES2=$(wildcard $(SRC_DIR)/*.s)
OBJ_FILES2=$(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(SRC_FILES2))
SRC_FILES3=$(wildcard $(SRC_DIR)/*.asm)
OBJ_FILES3=$(patsubst $(SRC_DIR)/%.asm, $(OBJ_DIR)/%.o, $(SRC_FILES3))


# check_dir:
# 	if [ ! -d "$(OBJ_DIR)" ]; then \
# 		mkdir -p $(OBJ_DIR); \
# 	fi

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(GCCPARAMS) $^ -I$(HDR_DIR) -c -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	$(AS) $(ASPARAMS) -o $@ $<

$(OBJ_ASM)/%.o: $(START_ASM)/%.asm
	nasm -f elf32 -o $@ $<


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	nasm -f elf32 -o $@ $<

%.o: %.asm
	${ASM} $< -f elf -o $@

%.bin: %.asm
	${ASM} $< -f bin -o $@

all: HackOS.bin
bootl:
	(cd asm ; nasm -f elf bootloader.asm)
	

HackOS.bin:$(OBJ_FILES1) $(OBJ_FILES2) $(OBJ_FILES3)

	ld $(LDPARAMS) -o $@ $(OBJ_DIR)/*.o
	
	
iso: HackOS.bin
	
	make HackOS.bin
# mkdir iso
# mkdir iso/boot
# mkdir iso/boot/grub
	cp HackOS.bin iso/boot/HackOS.bin
	
	grub-mkrescue --output=HackOS.iso iso
#	rm -rf iso
HackOS.bin2: $(OBJ_FILES1) $(OBJ_FILES2) $(OBJ_FILES3)

	
	ld $(LDPARAMS) -o  $@ -Ttext 0x1000 $^ --oformat binary
	
os-image.bin: boot/boot.bin HackOS.bin2
	cat $^ > os-image.bin

boot/boot.bin: boot/boot.asm
	${ASM} $< -f bin -o $@
install: HackOS.bin
	sudo cp $< /boot/HackOS.bin

clean:
	rm -f *.o HackOS HackOS.iso HackOS.bin $(OBJ_DIR)/*.o
	rm -rf boot/*.bin boot/*.o
	

version+:
	./update_version "turd"
	echo 'Version updated'


changlog-test:
	gcc update_changelog.c -o update_changelog
	./ update_changelog
changlog:
	./update_changelog
run: iso
	
	qemu-system-x86_64 -cdrom HackOS.iso  -drive file=HDD.img,format=raw -serial file:"serial.log" -vga std -device sb16 -soundhw pcspk -m 2G
run-ext2:
	
	qemu-system-x86_64 -drive file=HDD.img,format=raw -serial file:"serial.log" -vga std -device sb16 -soundhw pcspk

run-fat:
#	make iso
#	make fat32
	qemu-system-x86_64 -drive format=raw,file=fat32.img,if=ide,index=0,media=disk -m 2G
# run-c:
# 	make iso
# 	qemu-system-x86_64 HackOS.iso -drive file=HDD.img -serial stdio

# run-nd:
# 	make iso
# 	qemu-system-x86_64 HackOS.iso -drive file=HDD.img

# run-bin:
# 	make clean
# 	make os-image.bin
# 	qemu-system-x86_64 os-image.bin -drive file=HDD.img

HDD-SSFS:
	qemu-img create HDD.img 1G

run-part:
	make iso
	qemu-system-x86_64 -cdrom HackOS.iso  -drive file=Algea-fs.img -serial file:"serial.log" -vga std -device sb16 -soundhw pcspk
fs-img:
	qemu-img create Algea-fs.img 1G

ext2-img:
	bash ./mkdisk.sh

