#!/bin/bash

# Set the path to your disk image
image_file="AthenX.img"

# Set the path to the rootfs folder within the mount point
rootfs_mount_point="/mnt/AthenX/rootfs"

# Set the path to HackOS.bin
hackos_bin="HackOS.bin"

if [ -f "$image_file" ]; then
  # If the .img file exists, mount it
  sudo mkdir -p /mnt/AthenX
  sudo mount -o loop,rw "AthenX.img" /mnt/AthenX
  # sudo umount /mnt/AthenX

  # Copy HackOS.bin into the rootfs folder
  sudo cp "HackOS.bin" "/mnt/AthenX/boot"
  # Define the source directory
  SRC_DIR="programs" # Change this to the actual source directory

  # Define the destination directory for programs
  DEST_DIR="/mnt/AthenX/sys"

  # Copy all files from the source directory to the destination directory
  for file in "$SRC_DIR"/*; do
    if [ -f "$file" ]; then
      sudo cp "$file" "$DEST_DIR"
    fi
  done

  # Define the source directory for man pages
  MAN_DIR="man" # Change this to the actual man pages directory

  # Define the destination directory for man pages
  MAN_DEST_DIR="/mnt/AthenX/man"

  # Copy all man pages from the source directory to the destination directory
  for man_file in "$MAN_DIR"/*; do
    if [ -f "$man_file" ]; then
      sudo cp "$man_file" "$MAN_DEST_DIR"
    fi
  done

  sudo cp "grub/grub.cfg" "/mnt/AthenX/grub/grub.cfg"
  echo "Contents of the mount point and its subdirectories:"
  ls -R /mnt/AthenX

  sudo cp "ui/cmdhandler.c" "/mnt/AthenX/root/cmd.c"
  sudo cp "LICENSE.txt" "/mnt/AthenX/root/license.txt"
  sudo cp "test.fac" "/mnt/AthenX/root/test.fac"
  sudo cp -r "sysroot" "/mnt/AthenX/home/"
  # Unmount the image
  sudo umount /mnt/AthenX
else
  # Create an empty image file
  dd if=/dev/zero of="$image_file" bs=1M count=1024
  sudo parted "$image_file" mklabel msdos
  sudo parted "$image_file" mkpart primary fat32 1MiB 100%
  # Partition the disk using fdisk
  echo -e "o\nn\np\n1\n\n\nw" | sudo fdisk "$image_file"

  # Format the partition as FAT32 with a volume label
  sudo mkfs.vfat -F 32 -n MYBOOT "$image_file"

  # Mount the image
  sudo mkdir -p /mnt/AthenX

  sudo mount -o loop,rw "$image_file" /mnt/AthenX
  sudo mkdir -p /mnt/AthenX/bin
  sudo mkdir -p /mnt/AthenX/etc
  sudo mkdir -p /mnt/AthenX/home
  sudo mkdir -p /mnt/AthenX/mnt
  sudo mkdir -p /mnt/AthenX/tmp
  sudo mkdir -p /mnt/AthenX/usr
  sudo mkdir -p /mnt/AthenX/var
  sudo mkdir -p /mnt/AthenX/boot
  sudo mkdir -p /mnt/AthenX/media
  sudo mkdir -p /mnt/AthenX/sys
  sudo mkdir -p /mnt/AthenX/dev
  sudo mkdir -p /mnt/AthenX/proc
  sudo mkdir -p /mnt/AthenX/lib
  sudo mkdir -p /mnt/AthenX/sbin
  sudo mkdir -p /mnt/AthenX/opt
  sudo mkdir -p /mnt/AthenX/root
  sudo mkdir -p /mnt/AthenX/gui
  sudo mkdir -p /mnt/AthenX/user
  sudo mkdir -p /mnt/AthenX/var/log
  sudo mkdir -p /mnt/AthenX/man

  # Copy your OS files and GRUB configuration

  sudo cp HackOS.bin /mnt/AthenX/boot

  # Install GRUB to the MBR (Master Boot Record)
  sudo grub-install --target=i386-pc --boot-directory=/mnt/AthenX --force --no-floppy --modules="part_msdos fat" /dev/loop0
  echo "saved_entry=grub.cfg" | sudo tee /mnt/AthenX/grub/grubenv
  sudo cat /mnt/AthenX/grub/grubenv
  # sudo cp "simple.cfg" /mnt/AthenX/grub/grub.cfg
  sudo cp -r "grub/." "/mnt/AthenX/grub/"
  sudo cp "LDout.map" "/mnt/AthenX/var"
  sudo cp "userspace/test" "/mnt/AthenX/sys"
  # Verify the root directory's LBA address using the file command
  # Calculate the LBA address for the root directory
  bytes_per_sector=$(sudo fdisk -l "$image_file" | grep "Sector size" | awk '{print $4}')
  sectors_per_cluster=$(sudo fdisk -l "$image_file" | grep "Sectors per cluster" | awk '{print $5}')
  reserved_sector_count=$(sudo fdisk -l "$image_file" | grep "Reserved sector count" | awk '{print $6}')
  first_fat_sector=$(expr $reserved_sector_count)
  root_dir_cluster=$(expr 2) # For FAT32, the root directory cluster is typically 2
  root_dir_lba=$(expr $first_fat_sector + \( \( $root_dir_cluster - 2 \) \* $sectors_per_cluster \))

  echo "Root Directory LBA Address: $root_dir_lba"
  echo "TESTING $image_file intergraty"
  fsck.fat $image_file
  # Unmount the image
  sudo umount /mnt/AthenX
fi
