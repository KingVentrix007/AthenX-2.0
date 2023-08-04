#!/bin/bash

# Check if any parameters are provided
if [ $# -eq 0 ]; then
  echo "Usage: $0 <param1> <param2> ..."
  exit 1
fi

# Parse the parameters and perform actions accordingly
case "$1" in
  "disk")
    # Action for param1
    echo "creating virtual disk without format"
    cd ..
    make HDD-SSFS
    # Add your commands here
    ;;
  "fat32")
    # Action for param2
    echo "creating virtual disk with FAT32"
    # Add your commands here
    ;;
  "param3")
    # Action for param3
    echo "Performing action for param3"
    # Add your commands here
    ;;
  *)
    # Default action if the parameter is not recognized
    echo "Invalid parameter: $1"
    echo "Usage: $0 <param1> <param2> ..."
    exit 1
    ;;
esac

# Add more cases as needed for additional parameters.

# End of the script
