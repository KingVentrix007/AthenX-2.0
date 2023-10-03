#!/bin/bash

# Define your input ISO and output IMG filenames
ISO_FILE="HackOS.iso"
IMG_FILE="athenX.img"

# Create a blank FAT32 .img file
dd if=/dev/zero of="$IMG_FILE" bs=26M count=1

# Create a FAT32 partition
fdisk "$IMG_FILE" << EOF
n
p
1


t
b
w
EOF

# Set up loop devices
LOOPDISK=$(losetup -f)
sudo losetup "$LOOPDISK" "$IMG_FILE"

# Format the partition as FAT32
sudo mkfs.fat -F 32 -n ATHENX "$LOOPDISK"

# Mount the partition
sudo mount "$LOOPDISK" /mnt

# Copy ISO contents to the mounted partition
sudo cp -r "$ISO_FILE"/* /mnt

# Install GRUB to the .img file
sudo grub-install --root-directory=/mnt --no-floppy --modules="normal part_msdos fat" "$LOOPDISK"

# Unmount the partition
sudo umount /mnt

# Clean up loop devices
sudo losetup -d "$LOOPDISK"

# Sync and set ownership
sync
sudo chown $SUDO_USER:$SUDO_USER "$IMG_FILE"
