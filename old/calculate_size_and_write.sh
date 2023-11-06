#!/bin/bash

# Define the name of your binary file (AthenX.bin)
binaryFileName="AthenX.bin"

# Define the percentage to add (30%)
percentageToAdd=1

# Calculate the size of the binary file
fileSize=$(stat -c%s "iso/boot/$binaryFileName")

# Calculate the size to add (30%)
sizeToAdd=$((fileSize * 501 / 100))

# Calculate the total size
totalSize=$((fileSize + sizeToAdd))

# Write the total size to install.txt in the desired format
echo "INSTALL" > iso/boot/grub/install.txt
echo "#SIZE $totalSize;" >> iso/boot/grub/install.txt

