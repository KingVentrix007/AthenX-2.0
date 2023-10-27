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

- **Compatibility:** The AthenX Custom Libc is designed to be compatible with existing Standards(POSIXS) as well as the AthenXOS standerd.
## Getting Started

This documentation will guide you through the various custom libc functions available in AthenX. Whether you're a system developer or a software application programmer, understanding these functions is essential for working with the AthenX operating system effectively.

You'll find detailed descriptions, usage examples, and best practices for each function. Whether you want to read or write files, manipulate strings, or make system calls, we've got you covered.

Explore this documentation to unlock the full potential of the AthenX Custom Libc and empower your software development projects.

Let's embark on a journey to explore the capabilities of these custom libc functions, enabling you to harness the power of AthenX for your unique programming needs.

## Table of Contents

- [File I/O Functions](#file-io-functions)
- [Memory Management Functions](#memory-management-functions)
- [String Manipulation Functions](#string-manipulation-functions)
- [System Call Functions](#system-call-functions)

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


# MEMORY MANAGEMENT FUNCTIONS

<!-- - [atoi](#atoi) -->
- [malloc](#malloc)
- [free](#free)
- [realloc](#realloc)
- [calloc](#calloc)
- [athenx_memalign](#athenx_memalign)
<!-- - [exit](#exit) -->

<!-- ### `atoi`

**Description:**
Converts a string to an integer.

**Parameters:**
- `str` (const char*) - The input string to convert to an integer.

**Return:**
- `int` - The integer representation of the input string. If the input is not a valid integer, 0 is returned.

**Usage Example:**
```c
const char* numberStr = "12345";
int number = atoi(numberStr);
``` -->

### `malloc`

**Description:**
Memory allocation function.

**Parameters:**
- `size` (size_t) - Size of the memory to allocate.

**Return:**
- `void*` - A pointer to the allocated memory, or NULL if allocation fails.

**Usage Example:**
```c
size_t bufferSize = 1024;
void* buffer = malloc(bufferSize);
```
### `free`

**Description:**
Memory deallocation function.

**Parameters:**
- `ptr` (void*) - Pointer to the memory to deallocate.

**Return:**
- None

**Usage Example:**
```c
void* memory = malloc(1024); // Allocate memory.
// ... Use the allocated memory ...
free(memory); // Deallocate the memory.
```
### `realloc`
**Description:**
Memory reallocation function.

**Parameters:**
- ptr (void *) - Pointer to the previously allocated memory
- size (size_t) - The new size, in bytes, for the memory blocks

**Returns:**
- void* - A pointer to the reallocated memory block

**Usage Example:**
```c

 // Allocate memory using malloc
void* memory = malloc(1024);

// Use the allocated memory here

// Resize memory using realloc
void* resized_memory = realloc(memory, 2048);

// Use the resized memory here

// Deallocate memory using free
free(resized_memory);
``````

### `calloc`
**Description:** Memory allocation function for initialized memory.

**Parameters:**
- num (size_t) - The number of elements to allocate memory for.
- size (size_t) - The size, in bytes, of each element.

**Return:**
-  void* - A pointer to the allocated and zero-initialized memory block, or NULL if the allocation fails.
```c
// Allocate and initialize memory using calloc
void* memory = calloc(10, sizeof(int));

// Use the allocated and initialized memory here

// Deallocate memory using free
free(memory);
``````


# String Manipulation Functions
## Functions Provided

- [`memset`](#memset)
- [`strstr`](#strstr)
- [`memcpy`](#memcpy)
- [`memmove`](#memmove)
- [`parse_string`](#parse_string)
- [`memcmp`](#memcmp)
- [`atoi`](#atoi)
- [`strlen`](#strlen)
- [`strncmp`](#strncmp)
- [`strncpy`](#strncpy)
- [`strcmp`](#strcmp)
- [`strcpy`](#strcpy)
- [`ctos`](#ctos)
- [`strcat`](#strcat)
- [`isspace`](#isspace)
- [`isalpha`](#isalpha)
- [`upper`](#upper)
- [`lower`](#lower)
- [`backspace`](#backspace)
- [`strrchr`](#strrchr)
- [`itoa`](#itoa)
- [`string_length`](#string_length)
- [`append`](#append)
- [`hex_string_to_uint16`](#hex_string_to_uint16)
- [`strtoul`](#strtoul)
- [`strtok`](#strtok)
- [`strtok_r`](#strtok_r)
- [`strtoull`](#strtoull)
- [`tolower`](#tolower)
- [`isdigit`](#isdigit)
- [`isxdigit`](#isxdigit)
- [`strtol`](#strtol)
- [`isalnum`](#isalnum)
- [`strcspn`](#strcspn)


### `memset`

**Description:**
Sets a block of memory with a specified value.

**Parameters:**
- `dst` (void*) - A pointer to the destination memory.
- `c` (char) - The value to set in the memory.
- `n` (uint32) - The number of bytes to set.

**Example:**
```c
#include "string.h"
uint8 buffer[10];
memset(buffer, 0, 10);
``````
### `strstr`

**Description:**

Finds the first occurrence of a substring in a string.

**Parameters:**
- `s1` (char*) - The string to search in.
- `s2` (const char*) - The substring to find.

**Example:**
```c
#include "string.h"
char* text = "Hello, World!";
char* sub = "World";
char* result = strstr(text, sub);
``````
### `memcpy`

**Description:**
Copies a block of memory from one location to another.

**Parameters:**
- `dst` (void*) - A pointer to the destination memory.
- `src` (const void*) - A pointer to the source memory.
- `n` (uint32) - The number of bytes to copy.

**Example:**
```c
#include "string.h"
uint8 source[5] = {1, 2, 3, 4, 5};
uint8 destination[5];
memcpy(destination, source, 5);
``````
### `memmove`

**Description:**
Copies a block of memory from one location to another, even if the source and destination regions overlap.

**Parameters:**
- `dest` (void*) - A pointer to the destination memory.
- `src` (const void*) - A pointer to the source memory.
- `size` (size_t) - The number of bytes to copy.

**Example:**
```c
#include "string.h"
uint8 data[5] = {1, 2, 3, 4, 5};
memmove(data + 1, data, 4);
``````
### `parse_string`

**Description:**
Parses a string and splits it based on a specified character.

**Parameters:**
- `parser` (char*) - The parsed string will be stored here.
- `string` (char*) - The input string to parse.
- `c` (char) - The delimiter character.

**Example:**
```c
#include "string.h"
char input[] = "apple,banana,cherry";
char* tokens[3];
parse_string(tokens, input, ',');
``````

### `memcmp`

**Description:**
Compares two memory blocks.

**Parameters:**
- `s1` (uint8*) - Pointer to the first memory block.
- `s2` (uint8*) - Pointer to the second memory block.
- `n` (uint32) - Number of bytes to compare.

**Example:**
```c
#include "string.h"
uint8 buffer1[5] = {1, 2, 3, 4, 5};
uint8 buffer2[5] = {1, 2, 3, 4, 5};
int result = memcmp(buffer1, buffer2, 5);
``````
### `strlen`

**Description:**
Calculates the length of a string.

**Parameters:**
- `s` (const char*) - The input string.

**Example:**
```c
#include "string.h"
const char* str = "Hello, World!";
int length = strlen(str);
``````
### `strncmp`

**Description:**
Compares two strings up to a specified number of characters.

**Parameters:**
- `s1` (const char*) - The first string to compare.
- `s2` (const char*) - The second string to compare.
- `n` (size_t) - The maximum number of characters to compare.

**Example:**
```c
#include "string.h"
const char* str1 = "apple";
const char* str2 = "appetizer";
int result = strncmp(str1, str2, 3);
``````
### `strncpy`

**Description:**
Copies a portion of one string to another.

**Parameters:**
- `dst` (char*) - The destination string.
- `src` (const char*) - The source string.
- `n` (size_t) - The maximum number of characters to copy.

**Example:**
```c
#include "string.h"
char dest[10];
const char* source = "Hello, World!";
strncpy(dest, source, 5);
``````
### `strcmp`

**Description:**
Compares two strings.

**Parameters:**
- `s1` (const char*) - The first string to compare.
- `s2` (char*) - The second string to compare.

**Example:**
```c
#include "string.h"
const char* str1 = "apple";
char* str2 = "banana";
int result = strcmp(str1, str2);
``````
### `strcpy`

**Description:**
Copies one string to another.

**Parameters:**
- `dst` (char*) - The destination string.
- `src` (const char*) - The source string.

**Example:**
```c
#include "string.h"
char dest[10];
const char* source = "Hello, World!";
strcpy(dest, source);
``````
### `ctos`

**Description:**
Converts a character to a string.

**Parameters:**
- `s` (char[2]) - The resulting string.
- `c` (const char) - The character to convert.

**Example:**
```c
#include "string.h"
char s[2];
const char c = 'A';
ctos(s, c);
``````
### `strcat`

**Description:**
Concatenates two strings.

**Parameters:**
- `dest` (char*) - The destination string.
- `src` (const char*) - The source string to append.

**Example:**
```c
#include "string.h"
char dest[20] = "Hello, ";
const char* src = "World!";
strcat(dest, src);
``````
### `isspace`

**Description:**
Checks if a character is a white-space character.

**Parameters:**
- `c` (char) - The character to check.

**Example:**
```c
#include "string.h"
char c = ' ';
bool result = isspace(c);
``````
### `isalpha`

**Description:**
Checks if a character is an alphabetic character.

**Parameters:**
- `c` (char) - The character to check.

**Example:**
```c
#include "string.h"
char c = 'A';
int result = isalpha(c);
``````
### `upper`

**Description:**
Converts a character to uppercase.

**Parameters:**
- `c` (char) - The character to convert.

**Example:**
```c
#include "string.h"
char c = 'a';
char result = upper(c);
``````

### `lower`

**Description:**
Converts a character to lowercase.

**Parameters:**
- `c` (char) - The character to convert.

**Example:**
```c
#include "string.h"
char c = 'Z';
char result = lower(c);
``````
### `backspace`

**Description:**
Handles a backspace in a character buffer.

**Parameters:**
- `buffer` (char*) - The character buffer to handle.

**Example:**
```c
#include "string.h"
char buffer[10] = "Hello";
bool result = backspace(buffer);
``````
### `strrchr`

**Description:**
Locates the last occurrence of a character in a string.

**Parameters:**
- `str` (const char*) - The string to search.
- `ch` (int) - The character to locate.

**Example:**
```c
#include "string.h"
const char* str = "Hello, World!";
int ch = 'o';
char* result = strrchr(str, ch);
``````
### `itoa`

**Description:**
Converts an integer to a string.

**Parameters:**
- `buf` (char*) - The destination string.
- `base` (int) - The number base (e.g., 10 for decimal).
- `d` (int) - The integer to convert.

**Example:**
```c
#include "string.h"
char str[10];
int num = 123;
itoa(str, 10, num);
``````
### `string_length`

**Description:**
Calculates the length of a string.

**Parameters:**
- `s` (char[]) - The input string.

**Example:**
```c
#include "string.h"
char str[] = "Hello, World!";
int length = string_length(str);
``````
### `append`

**Description:**
Appends a character to a string.

**Parameters:**
- `s` (char[]) - The target string.
- `n` (char) - The character to append.

**Example:**
```c
#include "string.h"
char str[20] = "Hello, ";
append(str, 'W');
``````
### `hex_string_to_uint16`

**Description:**
Converts a hexadecimal string to a 16-bit unsigned integer.

**Parameters:**
- `hexString` (const char*) - The hexadecimal string to convert.
- `result` (uint16_t*) - A pointer to the result.

**Example:**
```c
#include "string.h"
const char* hexStr = "1A2F";
uint16_t result;
bool success = hex_string_to_uint16(hexStr, &result);
``````
### `strtoul`

**Description:**
Converts a string to an unsigned long integer.

**Parameters:**
- `str` (const char*) - The input string to convert.
- `endptr` (char**) - The address of a pointer to the character following the converted value.
- `base` (int) - The base for conversion (e.g., 10 for decimal).

**Example:**
```c
#include "string.h"
const char* str = "12345";
char* end;
unsigned long num = strtoul(str, &end, 10);
``````
### `strtok`

**Description:**
Tokenizes a string based on specified delimiters.

**Parameters:**
- `str` (char*) - The input string to tokenize.
- `delimiters` (const char*) - A set of delimiter characters.

**Example:**
```c
#include "string.h"
char input[] = "apple,banana,cherry";
char* token = strtok(input, ",");
``````
### `strtok_r`

**Description:**
Tokenizes a string based on specified delimiters with reentrant support.

**Parameters:**
- `str` (char*) - The input string to tokenize.
- `delim` (const char*) - A set of delimiter characters.
- `saveptr` (char**) - A pointer to a char* variable that stores the context.

**Example:**
```c
#include "string.h"
char input[] = "apple,banana,cherry";
char* saveptr;
char* token = strtok_r(input, ",", &saveptr);
``````
### `strtoull`

**Description:**
Converts a string to an unsigned long long integer.

**Parameters:**
- `str` (const char*) - The input string to convert.
- `endptr` (char**) - The address of a pointer to the character following the converted value.
- `base` (int) - The base for conversion (e.g., 10 for decimal).

**Example:**
```c
#include "string.h"
const char* str = "1234567890";
char* end;
unsigned long long num = strtoull(str, &end, 10);
``````
### `tolower`

**Description:**
Converts a character to lowercase.

**Parameters:**
- `c` (int) - The character to convert.

**Example:**
```c
#include "string.h"
int c = 'A';
int result = tolower(c);
````
### `isdigit`

**Description:**
Checks if a character is a decimal digit.

**Parameters:**
- `c` (int) - The character to check.

**Example:**
```c
#include "string.h"
int c = '5';
int result = isdigit(c);
``````
### `isxdigit`

**Description:**
Checks if a character is a hexadecimal digit.

**Parameters:**
- `c` (int) - The character to check.

**Example:**
```c
#include "string.h"
int c = 'A';
int result = isxdigit(c);
``````
### `strtol`

**Description:**
Converts a string to a long integer.

**Parameters:**
- `str` (const char*) - The input string to convert.
- `endptr` (char**) - The address of a pointer to the character following the converted value.
- `base` (int) - The base for conversion (e.g., 10 for decimal).

**Example:**
```c
#include "string.h"
const char* str = "12345";
char* end;
long num = strtol(str, &end, 10);
``````
### `isalnum`

**Description:**
Checks if a character is alphanumeric (a letter or a digit).

**Parameters:**
- `c` (int) - The character to check.

**Example:**
```c
#include "string.h"
int c = 'A';
int result = isalnum(c);
``````
### `strcspn`

**Description:**
Calculates the length of the initial segment of a string that contains only characters not in a specified set.

**Parameters:**
- `str` (const char*) - The input string.
- `reject` (const char*) - A string containing characters to be rejected.

**Example:**
```c
#include "string.h"
const char* str = "Hello, World!";
const char* reject = " ,!";
size_t result = strcspn(str, reject);
``````
