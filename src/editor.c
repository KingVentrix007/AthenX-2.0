#include "vesa.h"
#include "keyboard.h"
#include "string.h"
#include "display.h"
// #include "ide.h"
#include "printf.h"
#include "debug.h"
#include "fs.h"
#include "editor.h"
#include "fat_filelib.h"
// Define the default background color
#define DEFAULT_BG_COLOR 0 // Black

// Structure to represent a keyword and its font color
typedef struct {
    const char* keyword;
    int font_color_r; // Red component of the font color
    int font_color_g; // Green component of the font color
    int font_color_b; // Blue component of the font color
} KeywordFontColor;

// Define an array of keyword-font color mappings
KeywordFontColor keywordFontColors[] = {
    {"auto", 255, 128, 0},      // Orange
    {"break", 255, 0, 0},       // Red
    {"case", 255, 0, 0},        // Red
    {"continue", 255, 0, 0},    // Red
    {"do", 255, 0, 0},          // Red
    {"default", 255, 0, 0},     // Red
    {"else", 255, 0, 0},        // Red
    {"extern", 0, 0, 255},      // Blue
    {"for", 255, 0, 0},         // Red
    {"if", 255, 0, 0},          // Red
    {"goto", 255, 0, 0},        // Red
    {"register", 0, 0, 255},    // Blue
    {"return", 0, 0, 255},      // Blue
    {"sizeof", 0, 0, 255},      // Blue
    {"switch", 255, 0, 0},      // Red
    {"volatile", 0, 0, 255},    // Blue
    {"while", 255, 0, 0},       // Red

    // C11 keywords
    {"_Alignas", 0, 255, 0},    // Green
    {"_Alignof", 0, 255, 0},    // Green
    {"_Atomic", 0, 255, 0},     // Green
    {"_Generic", 0, 255, 0},    // Green
    {"_Noreturn", 0, 255, 0},   // Green
    {"_Static_assert", 0, 255, 0}, // Green
    {"_Thread_local", 0, 255, 0}, // Green

    // C99 keywords
    {"_Bool", 0, 255, 0},       // Green
    {"_Complex", 0, 255, 0},    // Green
    {"_Imaginary", 0, 255, 0},  // Green

    // Types, variables & functions related
    {"bool", 0, 128, 255},      // Blue
    {"char", 0, 128, 255},      // Blue
    {"const", 0, 128, 255},     // Blue
    {"double", 0, 128, 255},    // Blue
    {"enum", 0, 128, 255},      // Blue
    {"false", 0, 128, 255},     // Blue
    {"float", 0, 128, 255},     // Blue
    {"int", 0, 128, 255},       // Blue
    {"long", 0, 128, 255},      // Blue
    {"short", 0, 128, 255},     // Blue
    {"signed", 0, 128, 255},    // Blue
    {"static", 0, 128, 255},    // Blue
    {"struct", 0, 128, 255},    // Blue
    {"true", 0, 128, 255},      // Blue
    {"typedef", 0, 128, 255},   // Blue
    {"union", 0, 128, 255},     // Blue
    {"unsigned", 0, 128, 255},  // Blue
    {"void", 0, 128, 255},      // Blue
    {"#include",255,0,255},
    {NULL, 0, 0, 0}             // End marker
};
int text_editor(char buffer[], int bufferSize)
{
    //DEBUG("H");
    char input[MAX_FILE_SIZE];
    int inputSize = 0;

    // Read input from the keyboard using kb_getchar()
    char c;
    
    while (1) {
        c = kb_getchar();
        if(c == '`')
        {
            break;
        }
        //int exit = kb_get_scancode();
        // if(exit != -1)
        // {
        //     printf("%d",exit);
        // }
        // if (exit == 29 && c == 'q') {
        //     return -1; // Ctrl+Q is pressed, exit requested
        // }
        // Check if the buffer is full
        if (bufferSize >= MAX_FILE_SIZE - 1) {
            printf("Buffer is full. Cannot append more text.\n");
            return 0; // Buffer is full
        }
        else if (c != '\0')
        {
            buffer[bufferSize] = c;
            input[inputSize] = c;
            bufferSize++;
            inputSize++;
            printf("%c",c);
        }
        
        // Append the character to the buffer and input
       
    }

    // Null-terminate the input
    input[inputSize] = '\0';

    // Print the input for confirmation
    //printf("Appended to buffer: %s\n", input);

    return 1; // Successfully appended text
}



int is_c_file(const char* filename) {
    // Check if the file has a .c extension
    const char* extension = strrchr(filename, '.');
    return (extension != NULL && strcmp(extension, ".c") == 0);
}


int displayFileContents(const char* filename) {
    if (!is_c_file(filename)) {
        // If it's not a .c file, just print the plain text
        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            printf("Error: Unable to open file '%s'\n", filename);
            return -1;
        }

        char line[256]; // Adjust the buffer size as needed
        while (fgets(line, sizeof(line), file) != NULL) {
            printf("%s", line);
        }

        fclose(file);
        return 0;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        return -1;
    }

    char line[256]; // Adjust the buffer size as needed
    while (fgets(line, sizeof(line), file) != NULL) {
        int isKeywordLine = 0; // Flag to track if it's a line with keywords

        // Loop through the keyword-font color mappings
        for (int i = 0; keywordFontColors[i].keyword != NULL; i++) {
            if (strstr(line, keywordFontColors[i].keyword) != NULL) {
                // If the line contains a keyword, set the flag and break
                isKeywordLine = 1;
                break;
            }
        }

        if (isKeywordLine) {
            // If it's a line with keywords, apply syntax highlighting
            char* token = strtok(line, " \t\n"); // Split the line into tokens

            while (token != NULL) {
                // Assume the word is not a keyword until proven otherwise
                int isKeyword = 0;

                // Loop through the keyword-font color mappings
                for (int i = 0; keywordFontColors[i].keyword != NULL; i++) {
                    if (strcmp(token, keywordFontColors[i].keyword) == 0) {
                        set_font_c(keywordFontColors[i].font_color_r,
                                   keywordFontColors[i].font_color_g,
                                   keywordFontColors[i].font_color_b);
                        isKeyword = 1; // It's a keyword
                        printf("%s ",token);
                         set_font_c(255, 255, 255); // Reset font color
                        break; // Stop searching for keywords once found
                    }
                }

                if (!isKeyword) {
                    // If the token is not a keyword, print it without color
                    printf("%s ", token);
                }

                token = strtok(NULL, " \t\n"); // Get the next token
            }

            printf("\n"); // Print a newline at the end of the line
            set_font_c(255, 255, 255); // Reset font color
        } else {
            // If it's not a line with keywords, print it as-is
            printf("%s", line);
        }
    }

    fclose(file);
    return 0;
}
bool cursor_visible_fim = true;

void toggle_cursor_visibility_fim() {
    cursor_visible_fim = !cursor_visible_fim;
}
size_t length_of_last_line(const char *buffer) {
    if (buffer == NULL || buffer[0] == '\0') {
        return 0; // Handle empty or NULL buffer
    }

    size_t buffer_length = strlen(buffer);
    size_t last_line_length = 0;

    // Start from the end of the buffer and move backwards
    for (int i = buffer_length - 1; i >= 0; i--) {
        if (buffer[i] == '\n') {
            break; // Stop when a newline character is found
        }
        last_line_length++;
    }

    return last_line_length;
}
int ftruncate(const char* fileName) {
    FILE* file = fopen(fileName, "w"); // Open the file for writing (truncates to zero length)
    if (file == NULL) {
        printf("Error opening file for truncation");
        return -1;
    }
    
    fclose(file); // Close the file to truncate it
    
    return 0;
}
bool fim_backspace(char *buffer, int* currentIndex) {
    int len = strlen(buffer);
    if (currentIndex >= 0 && currentIndex < len) {
        // Remove the character at the specified index
        memmove(&buffer[*currentIndex], &buffer[*currentIndex + 1], len - *currentIndex);
        (*currentIndex)--;
        return true;
    } else {
        return false;
    }
}
int fim(char buffer[MAX_FILE_SIZE+10],const char* fileName)
{   
    
    const size_t buffer_size = MAX_FILE_SIZE+10; // Adjust this size according to your needs
    //char buffer[buffer_size];
     size_t bytes_read = read_file_into_buffer(fileName, buffer, buffer_size);
     printf("\n%s", buffer);
     char c;
     int pos_buffer = strlen(buffer);
     while (1) 
     {
        int ticks = get_ticks();
            if(ticks >= 500)
            {

                undraw_line(get_screen_x(),get_screen_y());
                reset_ticks();
            }
             if (ticks >= 50) {
                toggle_cursor_visibility_fim();
                reset_ticks();
            }
        c = kb_getchar();
         if (c == '`') 
         {
                break; // Ctrl+Q is pressed, exit requested
        }
        else if (c == '\b') 
        {       if(buffer[strlen(buffer)-1] == '\n' && fim_backspace(buffer,pos_buffer))
                {
                    //printf("I AM HERE");
                    undraw_line(get_screen_y(),get_screen_x());
                    set_screen_x(get_screen_x()-16);
                    
                    size_t x = length_of_last_line(buffer)*8;
                    //printf("X: %d blank",x);
                     
                    set_screen_y(x+8);
                    printf("\b");
                }
                else if (fim_backspace(buffer,pos_buffer)) {
                    printf("\b"); // Erase the character on the screen
                }
                else
                {
                    ///beep();
                }
        }
         else if (strlen(buffer) >= MAX_FILE_SIZE+10 - 1) 
         {
                    printf("Buffer is full. Cannot append more text.\n");
                    return 0; // Buffer is full
        }
        else if(c != '\0' && (int)c != 0x48 && (int)c != 0x50 && (int)c != 0x4D && (int)c != 0x4B )
        {
            undraw_line(get_screen_y(),get_screen_x());
            printf("%c",c);
            buffer[pos_buffer] = c;
            pos_buffer++;
        }
        else if (c == SCAN_CODE_KEY_LEFT)
        {
            pos_buffer--;
             set_screen_y(get_screen_y()-16);
             if(buffer[pos_buffer] == '\n')
             {
                set_screen_x(get_screen_x()-16);
             }
        }
        else if (c == SCAN_CODE_KEY_RIGHT)
        {
            if(pos_buffer+1 < strlen(buffer))
            {
                pos_buffer++;
                set_screen_y(get_screen_y()+16);
            }
        }
        
        
        if (cursor_visible_fim) {
                draw_line_cursor(get_screen_y(),get_screen_x(),VBE_RGB(255, 0, 0));
            } else {
               undraw_line(get_screen_y(),get_screen_x());
            }
    }
    size_t dataLength = 0;
    while (buffer[dataLength] != '\0') {
                dataLength++;
    }
    FILE *fp;
    //ftruncate(fileName);
    fl_remove(fileName);
    fp = fopen(fileName,"w");
    
    // char tmp[bytes_read];
    // memset(tmp,0,bytes_read);
    //fl_fwrite(" ", sizeof(char), bytes_read, fp);
    //printf("\npath = %s\n",fileName);
    if(fp == NULL)
    {
        printf("ERROR reading file");
        return -1;
    }
    //char buffer_2[MAX_FILE_SIZE];
    
    //strcpy(buffer_2,buffer);
    // size_t dataLength = 0;
    //         while (buffer[dataLength] != '\0') {
    //             dataLength++;
    //     }
    size_t elementsWritten = fl_fwrite(buffer, sizeof(char), dataLength, fp);
    if(elementsWritten != dataLength)
    {
        printf("ERROR");
    }
    fclose(fp);
      //printf("\nDATA: %s: ",buffer);
     read_file_into_buffer(fileName, buffer, buffer_size);
     //printf("\n%s", buffer);
    // if (bytes_written != strlen(buffer)) {
    //     printf("Failed to write the entire buffer");
    //     return -1; // Return a negative value to indicate failure
    // }
   
    return 0;
}
// Function to save the buffer contents to a file
size_t read_file_into_buffer(const char* filename, char* buffer, size_t buffer_size) {
     //printf("\npath = %s\n",filename);
    FILE* file = fopen(filename, "rb"); // Open the file in binary read mode
    if (file == NULL) {
        printf("Error opening file\n");
        return 0; // Return 0 to indicate an error
    }

    size_t bytes_read = fread(buffer, 1, buffer_size, file); // Read from file to buffer
    fclose(file); // Close the file

    return bytes_read;
}
// int main() {
//     char fileBuffer[MAX_FILE_SIZE];
//     int bufferSize = 0;

//     printf("Text Editor\n");

//     // Get the file name from the user
//     char fileName[256];
//     printf("Enter file name (or press Enter to skip): ");
//     fgets(fileName, sizeof(fileName), stdin);
//     fileName[strcspn(fileName, "\n")] = '\0'; // Remove newline character if present

//     int result = appendTextToFile(fileBuffer, bufferSize, fileName);
//     if (result == -2) {
//         return 1; // File not found, exit with an error code
//     }

//     printf("Final content in buffer:\n%s\n", fileBuffer);

//     return 0;
// }
//     input[inputSize] = '\0';

//     // Print the input for confirmation
//     //printf("Appended to buffer: %s\n", input);

//     return 1; // Successfully appended text

void undraw_line(int x, int y) {
    for (int i = x; i < x + 10; i++) {
        for (int j = y; j < y+18; j++) {
            vbe_putpixel(i, j, get_bg_color());  // Set pixel to black (assuming 0 represents black)
        }
    }
}
void draw_line_cursor(int x, int y, int color) {
    for (int i = x; i < x + 8; i++) {
        for (int j = y; j < y+18; j++) {
            vbe_putpixel(i, j, color);
        }
    }
}