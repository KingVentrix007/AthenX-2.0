#!/bin/bash
printf "Creating Disk...\n"
dd if=/dev/zero of=ext2.img bs=1024 count=10000 > /dev/zero
mkfs -t ext2 -i 1024 -b 1024 -F ext2.img > /dev/zero
printf "Mounting...\n"
sudo rm -rf /mnt/ext2
sudo mkdir /mnt/ext2
sudo mount ext2.img /mnt/ext2
printf "MOUNTED!\n"
sudo cp -r rootfs/* /mnt/ext2
sudo umount ext2.img
printf "Done!\n"




