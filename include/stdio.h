#ifndef _STDIO_H
#define _STDIO_H
#define MAX_STRING_LENGTH 1000
#define EOF -1
char get_chr();
char* get_char_block();
void scanf(char* format, ...);
int perror(char *string);
#define IO_BUFFER_SIZE 1000

typedef struct
{
    int buffer[IO_BUFFER_SIZE];
    int buffer_pos;
    
}IO_STREAM;

#endif