#!/bin/bash

# Check if the user provided a filename for the .img file and a rootfs folder
if [ $# -ne 2 ]; then
  echo "Usage: $0 <output_filename.img> <path_to_rootfs_folder>"
  exit 1
fi

# Set the output filename and path to the rootfs folder
output_file="$1"
rootfs_folder="$2"

# Size of the .img file in megabytes (adjust as needed)
img_size_mb=100

# Create an empty .img file of the specified size
sudo dd if=/dev/zero of="$output_file" bs=1M count="$img_size_mb"

# Format the .img file with FAT32
sudo mkfs.vfat -F 32 -n "FAT32" "$output_file"

# Create a mount point for the .img file
sudo mkdir -p /mnt/fat32

# Mount the .img file
sudo mount -o loop "$output_file" /mnt/fat32

# Copy the contents of the rootfs folder to the .img file
sudo cp -r "$rootfs_folder"/* /mnt/fat32/

# Unmount the .img file
sudo umount /mnt/fat32

# Remove the temporary mount point
sudo rmdir /mnt/fat32

echo "FAT32 .img file created and rootfs folder mounted: $output_file"
