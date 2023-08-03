#!/bin/bash
printf "Creating Disk...\n"
dd if=/dev/zero of=fat32.img bs=1024 count=10000 > /dev/zero
mkfs fat -F 32 -n "MY_DISK" fat32.img
# printf "Mounting...\n"
# rm -rf /mnt/ext2
# mkdir /mnt/ext2
# mount fat32.img /mnt/ext2
# printf "MOUNTED!\n"
# cp -r rootfs/* /mnt/ext2
# umount fat32.img
printf "Done!\n"




