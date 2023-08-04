

DISK_SIZE_ext4 = 1G
DISK_IMAGE_ext4 = ext4.img



ext4: $(DISK_IMAGE)

$(DISK_IMAGE): clean-fat
	@echo "Creating an empty disk image..."
	dd if=/dev/zero of=$(DISK_IMAGE) bs=$(DISK_SIZE) count=1

	@echo "Formatting the disk image with ext4..."
	mkfs.ext4 $(DISK_IMAGE)

	@echo "Mounting the disk image..."
	mkdir -p mount_point
	sudo mount -o loop $(DISK_IMAGE) mount_point

	
	@echo "Unmounting the disk image..."
	sudo umount mount_point
	sudo rm -rf mount_point



clean-ext4:
	@echo "Cleaning up..."
	rm -f $(DISK_IMAGE)

Algea:
	truncate -s 1G Algea.img

	# Copy the ISO file onto the first sector of the virtual disk
	sudo dd if=../HackOS.iso of=Algea.img bs=512 count=1228