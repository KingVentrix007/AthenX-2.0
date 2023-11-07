#include "../libc/include/fileio.h"
#include "../libc/include/stdlib.h"
#include "../libc/include/stdio.h"
#include "file.h"
#include "editor.h"


int handle_input(char *buffer,int *buffer_pos)
{
    char c = get_char();
    switch (c)
    {
    case '\b':
        printf("\b");
        buffer[*buffer_pos] = '\0';
        break;
    case 75:
        break;
    case 72:
        break;
    case 77:
        break;
    case 80:
        break;
    default:
        return c;
        break;
    }
}


int main(int argc, char **argv)
{
    printf("editor");
    if(argc > 100)
    {
         
    }
    else
    {
        FILE *fp = fopen("/root/cmd.c","r");
        char *buffer = malloc(get_file_size_from_pointer(fp));
        int buffer_pos = 0;
        if(buffer == NULL)
        {
            printf("Malloc error\n");
            return -1;
        }
        while (1)
        {
            char chr = handle_input(buffer,&buffer_pos);
            printf("%c",chr);
        }
        
    }

    
    
}