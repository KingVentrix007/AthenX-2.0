
#include "printf.h"
#include "../include/fat_filelib.h"
#include "../include/kheap.h"
#include "stddef.h"
#include "../include/kernel.h"
#include "../include/multiboot.h"

#define MULTIBOOT_HEADER_MAGIC 0x1BADB002
#define multiboot_header_t MULTIBOOT_HEADER
/**
 * Function Name: is_valid_multiboot_header
 * Description: Check if a file has a valid multiboot header.
 *
 * Parameters:
 *   file_path (const char*) - Path to the file.
 *
 * Return:
 *   int - 1 if valid, 0 if not valid, -1 if an error occurs.
 */
int is_valid_multiboot_header(const char* file_path) {
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        // Handle file opening error
        printf("Error: Unable to open file %s\n", file_path);
        return -1;
    }

    // Get the size of the file
    size_t file_size = get_file_size(file);

    // Read the multiboot header from the file
    multiboot_header_t* multiboot_header = (multiboot_header_t*)malloc(file_size);
    if (multiboot_header == NULL) {
        // Handle memory allocation error
        printf("Error: Memory allocation failed for multiboot header\n");
        fclose(file);
        return -1;
    }

    fread(multiboot_header, 1, file_size, file);

    // Check if the multiboot header is valid
    int is_valid = (multiboot_header->magic == MULTIBOOT_HEADER_MAGIC);

    if (!is_valid) {
        // Print an error message if the multiboot header is not valid
        printf("Error: Invalid multiboot header in file %s\n", file_path);
    }

    // Clean up
    free(multiboot_header);
    fclose(file);

    return is_valid;
}

/**
 * Function Name: install_new_version
 * Description: Install the new version of AthenX if it has a valid multiboot header.
 *
 * Parameters:
 *   new_version_path (const char*) - Path to the new version of AthenX (e.g., "/tmp/AthenX.bin")
 *   old_version_path (const char*) - Path where the old version of AthenX is installed (e.g., "/boot/bin/AthenX.bin")
 *
 * Return:
 *   int - 0 if successful, -1 if there was an error
 */
int install_new_version(const char* new_version_path, const char* old_version_path) {
    // Check if the old version file has a valid multiboot header
    if (!is_valid_multiboot_header(old_version_path)) {
        // Handle error: Old version is not valid
        printf("Error: Old version %s does not have a valid multiboot header\n", old_version_path);
        // return -1;
    }

    // Read the new version from file
    FILE* new_version_file = fopen(new_version_path, "rb");
    if (new_version_file == NULL) {
        // Handle file opening error
        printf("Error: Unable to open file %s\n", new_version_path);
        return -1;
    }

    // Get the size of the new version file
    size_t new_version_size = get_file_size(new_version_file);

    // Read the multiboot header from the new version
    multiboot_header_t* multiboot_header = (multiboot_header_t*)malloc(new_version_size);
    if (multiboot_header == NULL) {
        // Handle memory allocation error
        printf("Error: Memory allocation failed for multiboot header\n");
        fclose(new_version_file);
        return -1;
    }

    fread(multiboot_header, 1, new_version_size, new_version_file);

    // Check if the multiboot header is valid
    if (multiboot_header->magic != MULTIBOOT_HEADER_MAGIC) {
        // Invalid multiboot header
        printf("Error: Invalid multiboot header in file %s\n", new_version_path);
        free(multiboot_header);
        // fclose(new_version_file);
        // return -1;
    }

    // Close the file and reopen for writing to the old version path
    fclose(new_version_file);

    // Write the new version to the old version path
    FILE* old_version_file = fopen(old_version_path, "wb");
    if (old_version_file == NULL) {
        // Handle file opening error
        printf("Error: Unable to open file %s for writing\n", old_version_path);
        free(multiboot_header);
        return -1;
    }

    // Reopen the new version file
    new_version_file = fopen(new_version_path, "rb");
    if (new_version_file == NULL) {
        // Handle file opening error
        printf("Error: Unable to open file %s\n", new_version_path);
        fclose(old_version_file);
        free(multiboot_header);
        return -1;
    }

    // Copy the contents from the new version to the old version
    char* buffer = (char*)malloc(new_version_size);
    if (buffer == NULL) {
        // Handle memory allocation error
        printf("Error: Memory allocation failed for file content buffer\n");
        fclose(new_version_file);
        fclose(old_version_file);
        free(multiboot_header);
        return -1;
    }

    fread(buffer, 1, new_version_size, new_version_file);
    fwrite(buffer, 1, new_version_size, old_version_file);

    // Clean up
    free(buffer);
    free(multiboot_header);

    // Close the files
    fclose(new_version_file);
    fclose(old_version_file);
    remove(new_version_path);
    return 0;  // Successful installation
}