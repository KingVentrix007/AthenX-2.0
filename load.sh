#!/bin/bash

# Define the paths to the two folders to be combined
folder1_path="src"
folder2_path="include"

# Create a new folder called "main_folder"
main_folder_path="load/main_folder"
mkdir -p "$main_folder_path"

# Copy contents of folder1 into main_folder
cp -r "$folder1_path"/* "$main_folder_path"

# Copy contents of folder2 into main_folder, handle naming conflicts
cp -r --backup=numbered "$folder2_path"/* "$main_folder_path"

# Create an ISO image of main_folder
genisoimage -o main_folder.iso -R -J "$main_folder_path"

# Calculate the size of the .img file (slightly larger)
img_size=$(stat -c %s main_folder.iso)
img_size=$((img_size + 1024)) # Adding 1024 bytes for safety

# Create a raw .img file
dd if=/dev/zero of=output.img bs=1M count="$img_size" status=progress

# Copy the contents of main_folder.iso to the .img file using dd
dd if=main_folder.iso of=output.img bs=1M conv=notrunc status=progress

echo "Folder contents copied to output.img."