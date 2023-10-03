#!/bin/bash

# Define paths to your ISO file and HDD image
ISO_FILE="HackOS.iso"           # Replace with your ISO file path
HDD_IMAGE="HDD.img"          # Replace with your HDD image path

# Define mount points
ISO_MOUNT="/mnt/iso_mount_point"
HDD_MOUNT="/mnt/hdd_mount_point"

# Define mount points
# Create mount points if they don't exist
if [ ! -d "$ISO_MOUNT" ]; then
    mkdir -p "$ISO_MOUNT"
fi

if [ ! -d "$HDD_MOUNT" ]; then
    mkdir -p "$HDD_MOUNT"
fi

# Mount the ISO file
sudo mount -o loop "$ISO_FILE" "$ISO_MOUNT"

# Mount the HDD image
sudo mount -o loop "$HDD_IMAGE" "$HDD_MOUNT"

# Copy ISO contents to HDD image
cp -r "$ISO_MOUNT"/* "$HDD_MOUNT/"

# Unmount both images
sudo umount "$ISO_MOUNT"
sudo umount "$HDD_MOUNT"

# Optionally, update GRUB configuration here if needed
# Example: sudo nano "$HDD_MOUNT/boot/grub/grub.cfg"

# Display a message indicating the process is complete
echo "ISO contents copied to HDD image successfully."