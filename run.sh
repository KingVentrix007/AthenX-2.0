#!/bin/bash

# Set the path to your disk image
image_file="AthenX.img"

# Set the path to the rootfs folder within the mount point
rootfs_mount_point="/mnt/AthenX/rootfs"

# Set the path to AthenX.bin
hackos_bin="AthenX.bin"

Directory="/mnt/AthenX"



check_multiboot_header() {
    file_path=$1

    # Use GRUB's file command to check the Multiboot header
    file_output=$(file -b "$file_path")

    # Check if the file output contains "Multiboot"
    if [[ $file_output == *Multiboot* ]]; then
        echo "File $file_path has a valid Multiboot header."
        return 0  # Success
    else
        echo "File $file_path does not have a valid Multiboot header."
        return 1  # Failure
    fi
}




if [ -f "$image_file" ]; then
  # If the .img file exists, mount it
  sudo mkdir -p /mnt/AthenX
  sudo losetup /dev/loop0 "$image_file"
  sudo losetup /dev/loop1 "$image_file" -o 1048576
  sudo mount -o rw /dev/loop1 /mnt/AthenX
  # result=$(test -w "$directory" )
  # echo  $result
  # sleep 10
  
  # sudo file /mnt/AthenX
  # sudo grub-install --target=i386-pc --boot-directory=/mnt/AthenX --root-directory=/mnt/AthenX   --force --no-floppy --modules="part_msdos fat" /dev/loop0
  # echo "saved_entry=/boot/grub/grub.cfg" | sudo tee /boot/grub/grubenv
  # sleep 5
  # sudo dosfsck -w -r -a /dev/loop1



  # sudo umount /mnt/AthenX
  # fsck.fat -n "$disk_image"

  # Check the return code of fsck
  # if [ $? -eq 0 ]; then
  #     echo "Disk image is valid."
  #     # Add your commands to run if the image is valid
  # else
  #     echo "Disk image is invalid or contains errors."
  #     exit
  #     # fsck.fat "$image_file"
  #     # Add your commands to run if the image is invalid
  # fi
  # Copy AthenX.bin into the rootfs folder
  # sudo rm "/mnt/AthenX/boot/AthenX.bin"
  # sleep 5
  check_multiboot_header "AthenX.bin"
  # sleep 6
  sudo cp "out/AthenX.bin" "/mnt/AthenX/tmp/AthenX.bin"
  sudo cp -r "/mnt/AthenX/" "inspect/"
  # sleep 5
  # sudo chroot /mnt/AthenX
  # GRUB_CONFIG="/mnt/AthenX/grub"
    # sudo grub-install --target=i386-pc --boot-directory=/mnt/AthenX --root-directory=/mnt/AthenX   --force --no-floppy --modules="part_msdos fat" /dev/loop0
  # exit
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

  # sudo cp "grub/grub.cfg" "/mnt/AthenX/grub/grub.cfg"
  echo "Contents of the mount point and its subdirectories:"
   sudo cp "../sound/sound.wav" /mnt/AthenX/sys/sound.wav
  ls -R /mnt/AthenX

  sudo cp "ui/cmdhandler.c" "/mnt/AthenX/root/cmd.c"
  sudo cp "LICENSE.txt" "/mnt/AthenX/root/license.txt"
  sudo cp "test.fac" "/mnt/AthenX/root/test.fac"
  # sudo cp -r "sysroot" "/mnt/AthenX/home/"
   
  # Unmount the image
  # sudo losetup -d /dev/loop0
  # sudo fsck.fat /dev/loop1
  
  sudo umount /mnt/AthenX
  sudo losetup -d /dev/loop1
  sudo losetup -d /dev/loop0
  
  # sleep 10
else
  # Create an empty image file
  dd if=/dev/zero of="$image_file" bs=1M count=1024
  # sudo parted "$image_file" mklabel msdos
  # sudo parted "$image_file" mkpart primary fat32 1MiB 100%
  # Partition the disk using fdisk
  echo -e "n\np\n1\n2048\n131071\nt\nc\na\n1\nw" | fdisk "$image_file"

  sudo losetup /dev/loop0 "$image_file"
  sudo losetup /dev/loop1 "$image_file" -o 1048576
  # Format the partition as FAT32 with a volume label
  # sudo mkfs.vfat -F 32 -n MYBOOT "$image_file"
  # sudo mke2fs /dev/loop1
  #  sudo mkfs.vfat -F 32 -n MYBOOT /dev/loop1
   sudo mkdosfs -F32 -f 2 /dev/loop1

   sudo mount /dev/loop1 /mnt/AthenX
  # # Mount the image
  # sudo mkdir -p /mnt/AthenX

  # sudo mount -o loop,rw "$image_file" /mnt/AthenX
  # sudo mkdir -p /mnt/AthenX/bin
  # sudo mkdir -p /mnt/AthenX/etc
  # sudo mkdir -p /mnt/AthenX/home
  # sudo mkdir -p /mnt/AthenX/mnt
  # sudo mkdir -p /mnt/AthenX/tmp
  # sudo mkdir -p /mnt/AthenX/usr
  # sudo mkdir -p /mnt/AthenX/var
  # sudo mkdir -p /mnt/AthenX/boot
  # sudo mkdir -p /mnt/AthenX/media
  # sudo mkdir -p /mnt/AthenX/sys
  # sudo mkdir -p /mnt/AthenX/dev
  # sudo mkdir -p /mnt/AthenX/proc
  # sudo mkdir -p /mnt/AthenX/lib
  # sudo mkdir -p /mnt/AthenX/sbin
  # sudo mkdir -p /mnt/AthenX/opt
  # sudo mkdir -p /mnt/AthenX/root
  # sudo mkdir -p /mnt/AthenX/gui
  # sudo mkdir -p /mnt/AthenX/user
  # sudo mkdir -p /mnt/AthenX/var/log
  # sudo mkdir -p /mnt/AthenX/man
 
  sudo cp -r "sysroot"/* /mnt/AthenX/
  # Copy your OS files and GRUB configuration

  sudo cp out/AthenX.bin /mnt/AthenX/boot
  check_multiboot_header "AthenX.bin"
  # sleep 6
  # Install GRUB to the MBR (Master Boot Record)
  sudo grub-install --target=i386-pc --boot-directory=/mnt/AthenX --root-directory=/mnt/AthenX   --force --no-floppy --modules="part_msdos fat" /dev/loop0
  echo "saved_entry=/boot/grub/grub.cfg" | sudo tee /boot/grub/grubenv
  echo "saved_entry=grub.cfg" | sudo tee /grub/grubenv

  sudo cat /boot/grub/grubenv
  # sudo cp "simple.cfg" /mnt/AthenX/grub/grub.cfg
  sudo cp -r "grub/." "/mnt/AthenX/grub"
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

  # echo "Root Directory LBA Address: $root_dir_lba"
  echo "TESTING $image_file intergraty"
  fsck.fat "/mnt/AthenX"
  # sleep 10
  # Unmount the image
  sudo umount /mnt/AthenX
  sudo losetup -d /dev/loop0
  sudo losetup -d /dev/loop1
fi
