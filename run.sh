#!/bin/bash

# Set the path to your disk image
image_file="AthenX.img"

# Set the path to the rootfs folder within the mount point
rootfs_mount_point="/mnt/my_bootable/rootfs"

# Set the path to HackOS.bin
hackos_bin="HackOS.bin"

if [ -f "$image_file" ]; then
  # If the .img file exists, mount it
  sudo mkdir -p /mnt/my_bootable
  sudo mount -o loop "$image_file" /mnt/my_bootable

  # Copy HackOS.bin into the rootfs folder
  echo "Contents of the mount point and its subdirectories:"
  ls -R /mnt/my_bootable
  sudo cp "$hackos_bin" "/mnt/my_bootable/rootfs"
  sudo cp "src/cmdhandler.c" "/mnt/my_bootable/rootfs/cmd.c"
  # Unmount the image
  sudo umount /mnt/my_bootable
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
    sudo mkdir -p /mnt/my_bootable

    sudo mount -o loop "$image_file" /mnt/my_bootable

    # Copy your OS files and GRUB configuration
    sudo cp -r "rootfs" /mnt/my_bootable/rootfs
    sudo cp HackOS.bin /mnt/my_bootable/rootfs

    # Install GRUB to the MBR (Master Boot Record)
    sudo grub-install --target=i386-pc --boot-directory=/mnt/my_bootable --force --no-floppy --modules="part_msdos fat" /dev/loop0
    echo "saved_entry=grub.cfg" | sudo tee /mnt/my_bootable/grub/grubenv
    sudo cat /mnt/my_bootable/grub/grubenv
    # sudo cp "simple.cfg" /mnt/my_bootable/grub/grub.cfg
    sudo cp -r "iso/boot/grub/." "/mnt/my_bootable/grub/"
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
    sudo umount /mnt/my_bootable

fi