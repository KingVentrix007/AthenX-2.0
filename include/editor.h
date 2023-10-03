#ifndef EDITOR_H
#define EDITOR_H
#include "../include/fs.h"
typedef struct
{
    /* data */
}editor_rim;

int text_editor(char buffer[], int bufferSize);
int displayFileContents(const char* filename);
int fim(char buffer[MAX_FILE_SIZE+10],const char* fileName);
size_t read_file_into_buffer(const char* filename, char* buffer, size_t buffer_size);
size_t length_of_last_line(const char *input_string);
#endif
