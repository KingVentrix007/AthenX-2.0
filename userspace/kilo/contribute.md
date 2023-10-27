# Kilo text editor

## These are the missing functions that I need to create in order to successfully port kilo. Any contribution to this list will be greatly appreciated

1. [ ] `tcsetattr`: Set terminal attributes, used for configuring the terminal.
2. [ ] `isatty`: Check if a file descriptor is associated with a terminal.
3. [ ] `atexit`: Register functions to be called at program exit.
4. [ ] `read` and `write`: Read from and write to file descriptors, used for input and output operations.
5. [ ] `ioctl`: Perform I/O control operations on file descriptors, often used for terminal-related operations.
6. [ ] `perror`: Print a description for the last error that occurred during a system call or library function.
7. [ ] `getline`: Read a line from a file stream, commonly used for reading lines from files.
8. [ ] `open`, `ftruncate`, `close`, and `strerror`: Functions for file operations like opening, truncating, closing files, and handling errors.
9. [ ] `isprint`: Check if a character is a printable character, typically used for character classification.
