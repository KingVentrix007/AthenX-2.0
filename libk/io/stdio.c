#include "../include/../include/stdio.h"
#include "../include/keyboard.h"
#include "../include/string.h"
#include "../include/stdarg.h"
#include "../include/fat_filelib.h"
#include "../include/printf.h"
IO_STREAM std_stream;
char get_chr() {
    int scan = get_char();
    if(scan != EOF || (scan >= 32 && scan <= 126))
    {
        //printf("%c", scan);
        return scan;
    }
    else if(scan != -10)
    {
        if(unget_char(scan) != EOF)
        {
            //printf("%c",scan);
            return -1;
        }
        else
        {
            return 0;
        }
    }
    else
    {

    }
    // do {
    //     
        
    // } while (c == '\n' || c == '\0');  // Continue until a non-newline, non-null character is read
    // printf("%c",c);
    // if(c == '\n')
    // {
    //     printf("NEW");
    // }
    // return c;
}

char* get_char_block()
{
    char c;
    while(1)
    {
        c = kb_getchar();
        if(c == '\0')
        {
            
        }
        else
        {
            // printf("\nChar->(%c)(%d)", c,c);
            return c;
        }
    }
    // while((c = get_char()) != '\0' && (c < 32 || c > 127))
    // {
        
    //     return c;
    // }
}
int get_int() {
    int result = 0;
    int sign = 1;
    char c;

    while (1) {
        c = get_chr();

        if (c == '+') {
            continue;
        } else if (c == '-') {
            sign = -1;
        } else if (c >= '0' && c <= '9') {
            result = (result * 10) + (c - '0');
        } else {
            break;
        }
    }

    return result * sign;
}
void scanf(char* format, ...) {
    va_list args;
    va_start(args, format);

    char c;

    while ((c = *format++) != '\0') {
        if (c == '%') {
            char format_specifier = *format++;  // Read the format specifier from the format string
            if (format_specifier == '\0') {
                break;  // Stop if the format string ends unexpectedly
            }

            // Check the format specifier
            if (format_specifier == 'c') {
                char* dest = va_arg(args, char*);
                *dest = get_chr();
                //printf("%c", *dest);  // Print the character
            } else if (format_specifier == 's') {
                char* dest = va_arg(args, char*);
                int i = 0;
                while (1) {
                    char next_char = get_chr();
                    if (next_char == EOF || next_char == '\n' || next_char == '\0') {
                        break;  // Stop if end of input or newline is reached
                    }
                    dest[i++] = next_char;
                }
                dest[i] = '\0';  // Null-terminate the string
                //printf("%s", dest);  // Print the string
            } else if (format_specifier == 'd') {
                int* dest = va_arg(args, int*);
                *dest = get_int();
                
                // Implement reading integers here
            } else if (format_specifier == 'f') {
                double* dest = va_arg(args, double*);
                // Implement reading floating-point numbers here
            }
            // Add more format specifiers as needed
        } else {
            // Handle non-format specifiers here
        }
    }

    va_end(args);
}


    

int perror(char *string)
{
    printf("%s\n",string);
    FILE * err;
    if(fl_is_dir("/var/log/") == 0)
    {
         err = fl_fopen("/var/log/err.txt", "a");
        if(err==NULL)
        {
            printf("FAILED");
        }
        fl_fwrite("\n",sizeof(char),strlen("\n"),err);
        fl_fwrite(string,sizeof(char),strlen(string),err);
        fl_fclose(err);
    }
    else
    {
        fl_createdirectory("/var/log/");
    }
   
    //exit_main();
    
}

// voidfind(const char* path, const char* extension) {
//     FL_DIR* dir;
//     fl_dirent* entry;

//     if ((dir = opendir(path)) == NULL) {
//         perror("Failed to open directory");
//         return;
//     }

//     while ((entry = readdir(dir)) != NULL) {
//         if (entry->is_dir == 0) { // Check if it's a regular file
//             const char* file_name = entry->filename;
//             const char* file_extension = strrchr(file_name, '.');

//             if (file_extension != NULL && strcmp(file_extension, extension) == 0) {
//                 printf("%s/%s\n", path, file_name);
//             }
//         }
//     }

//     closedir(dir);
// }

int sscanf(const char* format, const char* input, ...) {
    va_list args;
    va_start(args, input);

    const char* formatPtr = format;
    const char* inputPtr = input;
    int count = 0;

    while (*formatPtr) {
        if (*formatPtr == '%') {
            formatPtr++;

            switch (*formatPtr) {
                case 's': {
                    char* strArg = va_arg(args, char*);
                    while (*inputPtr && *inputPtr != ' ') {
                        *strArg++ = *inputPtr++;
                    }
                    *strArg = '\0';
                    count++;
                    break;
                }
                case 'c': {
                    char* charArg = va_arg(args, char*);
                    *charArg = *inputPtr++;
                    count++;
                    break;
                }
                case 'd': {
                    int* intArg = va_arg(args, int*);
                    *intArg = 0;

                    while (*inputPtr >= '0' && *inputPtr <= '9') {
                        *intArg = (*intArg * 10) + (*inputPtr++ - '0');
                    }

                    count++;
                    break;
                }
            }
        } else if (*formatPtr != *inputPtr) {
            // If the characters don't match, return the number of items parsed
            break;
        } else {
            // Characters match, continue parsing
            formatPtr++;
            inputPtr++;
        }
    }

    va_end(args);

    return count;
}