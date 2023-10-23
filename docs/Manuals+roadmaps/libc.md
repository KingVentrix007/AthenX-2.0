# AthenX Custom Libc Documentation

## Introduction

Welcome to the AthenX Custom Libc Documentation, a comprehensive guide to the custom C library functions that power the AthenX operating system. This library, designed with a focus on efficiency, compatibility, and extensibility, serves as the foundation for AthenX's core functionalities, allowing you to work with files, manage memory, and perform system calls effectively.

## What is the AthenX Custom Libc?

The AthenX Custom Libc is a collection of C library functions tailored specifically for the AthenX operating system. These functions are carefully crafted to provide essential tools for system development and application programming within the AthenX environment.

## Key Features

Our custom libc offers several key features:

- **Efficient File I/O:** You can work with files seamlessly using functions like `fopen`, `fwrite`, `fread`, and `fclose`.

- **String Manipulation:** Perform operations on strings with ease, thanks to functions like `strcpy`, `strcat`, and `strlen`.

- **Custom System Calls:** Utilize functions like `syscall` to interact with the AthenX operating system kernel for various operations.

- **Memory Management:** Functions like `memcpy`, `memset`, and `malloc` enable efficient memory management within your applications.

- **Extensibility:** The AthenX Custom Libc is designed to be extensible, allowing you to add additional functions as needed for your projects.

## Getting Started

This documentation will guide you through the various custom libc functions available in AthenX. Whether you're a system developer or a software application programmer, understanding these functions is essential for working with the AthenX operating system effectively.

You'll find detailed descriptions, usage examples, and best practices for each function. Whether you want to read or write files, manipulate strings, or make system calls, we've got you covered.

Explore this documentation to unlock the full potential of the AthenX Custom Libc and empower your software development projects.

Let's embark on a journey to explore the capabilities of these custom libc functions, enabling you to harness the power of AthenX for your unique programming needs.

## Table of Contents

- [File I/O Functions](#file-io-functions)
- [String Manipulation Functions](#string-manipulation-functions)
- [System Call Functions](#system-call-functions)
- [Memory Management Functions](#memory-management-functions)

Now, let's dive into each category and explore the custom libc functions that will assist you in your software development endeavors.

# FILE I/O Functions
AthenX Custom Libc provides a set of functions to handle file input and output, making it easier to work with files within the AthenX operating system.
### Table of Contents - File I/O Functions

- [fopen](#fopen)
- [fclose](#fclose)
- [fwrite](#fwrite)
- [fread](#fread)
- [create_file](#create_file)
- [rm_file](#rm_file)
You can use this script in your Markdown documentation to create a table of contents for the File I/O functions.






### `fopen`
```c
/**
 * Function Name: fopen
 * Description: Opens a file for reading or writing.
 *
 * Parameters:
 *   filename (const char*) - The name of the file to open.
 *   mode (char*) - The mode for opening the file (e.g., "r" for reading, "w" for writing).
 *
 * Return:
 *   FILE* - A pointer to the opened file, or NULL on error.
 */
FILE* fopen(const char *filename, char *mode);
````
**Usage Example:**
```c
#include "stdio.h"

int main() {
    FILE *file;
    const char *filename = "example.txt";
    const char *mode = "w"; // Open the file for writing.

    // Open the file for writing.
    file = fopen(filename, mode);

    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Use the 'file' pointer to write to the file.

    // Don't forget to close the file when done.
    fclose(file);

    return 0;
}
```
### `fclose`
```c
/**
 * Function Name: fclose
 * Description: Closes a previously opened file.
 *
 * Parameters:
 *   file (FILE*) - The pointer to the file to be closed.
 */
void fclose(FILE *file);
``````
**Usage example:**
```c
#include "stdio.h"

int main() {
    FILE *file;
    const char *filename = "example.txt";

    // Open the file for reading.
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Use the 'file' pointer to read from the file.

    // Close the file when done.
    fclose(file);

    return 0;
}
```
### `fwrite`
```c
/**
 * Function Name: fwrite
 * Description: Writes data to a file.
 *
 * Parameters:
 *   filename (const char*) - The name of the file to write to.
 *   buffer (char*) - The data to be written to the file.
 *
 * Return:
 *   int - The number of bytes written, or -1 on error.
 */
int fwrite(const char *filename, char *buffer);
``````
**Ussage example:**
```c
#include "stdio.h"

int main() {
    const char *filename = "example.txt";
    const char *data = "This is a test message.";

    // Write data to the file.
    int bytes_written = fwrite(filename, data);

    if (bytes_written == -1) {
        printf("Failed to write to the file.\n");
        return 1;
    }

    printf("%d bytes written.\n", bytes_written);

    return 0;
}
``````
### `fread`

```c
/**
 * Function Name: fread
 * Description: Reads data from a file into a buffer.
 *
 * Parameters:
 *   buffer (char*) - The buffer to store the read data.
 *   fp (FILE*) - A pointer to the opened file to read from.
 *   size (size_t) - The size of each element to be read.
 *
 * Return:
 *   int - The number of elements successfully read, or -1 on error.
 */
int fread(char *buffer, FILE *fp, size_t size);
````
**Usage example:**
```c
#include "stdio.h"

int main() {
    FILE *file;
    const char *filename = "example.txt";

    // Open the file for reading.
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    char data[100]; // Buffer to store the read data.
    size_t size = sizeof(char);
    int elements_read = fread(data, file, size);

    if (elements_read == -1) {
        printf("Failed to read from the file.\n");
        return 1;
    }

    // Process the read data in the 'data' buffer.

    // Close the file when done.
    fclose(file);

    return 0;
}
```
### `create_file`

```c
/**
 * Function Name: create_file
 * Description: Creates a new file at the specified path.
 *
 * Parameters:
 *   path (const char*) - The path where the new file will be created.
 *
 * Return:
 *   int - 0 on success, or -1 on error.
 */
int create_file(const char *path);
```
**Usage example:**
```c
#include "stdio.h"

int main() {
    const char *path = "new_file.txt";

    // Create a new file at the specified path.
    int result = create_file(path);

    if (result == -1) {
        printf("Failed to create the file.\n");
        return 1;
    }

    printf("File created successfully.\n");

    return 0;
}
``````
### `rm_file`
```c
/**
 * Function Name: rm_file
 * Description: Removes a file at the specified path.
 *
 * Parameters:
 *   path (const char*) - The path of the file to be removed.
 *
 * Return:
 *   int - 0 on success, or -1 on error.
 */
int rm_file(const char *path);
```
**Usage example:**
```c
#include "stdio.h"

int main() {
    const char *path = "file_to_remove.txt";

    // Remove the file at the specified path.
    int result = rm_file(path);

    if (result == -1) {
        printf("Failed to remove the file.\n");
        return 1;
    }

    printf("File removed successfully.\n");

    return 0;
}
```