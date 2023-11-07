#include "stdio.h"
#include "file.h"
// long get_file_size_from_pointer(FILE *file) {
//     if (file == NULL) {
//         return -1; // Return -1 to indicate that the file pointer is invalid
//     }

//     long current_position = ftell(file); // Get the current position
//     fseek(file, 0, SEEK_END); // Move the file pointer to the end of the file
//     long size = ftell(file); // Get the current position, which is the size
//     fseek(file, current_position, SEEK_SET); // Return the file pointer to its original position

//     return size;
// }