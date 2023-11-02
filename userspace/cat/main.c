#include "stdio.h"
#include <string.h>
#include "terminal.h"
typedef struct {
    const char *keyword;
    int font_color_R;
    int font_color_G;
    int font_color_B;
    int is_comment;
} KeywordFontColor;

// Function to set font color
// void set_font_color(int R, int G, int B) {
//     // Implement this function to set the font color
//     // based on the given RGB values
// }

// Function to highlight keywords
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void highlight_keywords(const char *code, const KeywordFontColor *keywords) {
    char* token = strtok((char*)code, " "); // Tokenize at spaces
    bool in_comment = false;
    bool in_quotes = false;
    bool in_preprocessor_directive = false;

    while (token != NULL) {
        // Check if the token is enclosed in double quotes
        if (strstr(token, "\"") == token) {
            if (!in_quotes) {
                in_quotes = true;
                // Set text color for text inside quotes
                set_font_c(187, 187, 187);
            } else {
                in_quotes = false;
                // Reset the text color back to the default for text outside quotes
                set_font_c(255, 255, 255);
            }
        }

        // Check if the token is a comment
        if (strstr(token, "//") == token) {
            in_comment = true;
        } else if (in_comment && strstr(token, "\n")) {
            in_comment = false; // Reset the comment flag at the end of a line
        }

        // Check for preprocessor directives following a #
        if (strstr(token, "#") == token) {
            in_preprocessor_directive = true;
            // Set the font color for preprocessor directives
            set_font_c(206, 119, 206);
        }

        // Check if the token ends with a space or newline and reset preprocessor directive flag
        if (in_preprocessor_directive && (strrchr(token, ' ') || strrchr(token, '\n'))) {
            in_preprocessor_directive = false;
            // Reset font color for non-directive text
            set_font_c(255, 255, 255);
        }

        // Check if the token is in the list of keywords and print its color
        int i = 0;
        while (keywords[i].keyword != NULL) {
            if (strcmp(token, keywords[i].keyword) == 0) {
                // Print the token in the corresponding color
                set_font_c(keywords[i].font_color_R, keywords[i].font_color_G, keywords[i].font_color_B);
                printf("%s ", token);
                if (!in_comment && !in_quotes && !in_preprocessor_directive) {
                    // Reset font color if the token is not a comment, not inside quotes, and not a directive
                    set_font_c(255, 255, 255);
                }
                break; // Break if a keyword is found
            }
            i++;
        }

        // Print the token as is if it's not in the list of keywords
        if (keywords[i].keyword == NULL) {
            printf("%s ", token);
            if (in_comment || in_preprocessor_directive) {
                // Reset the comment or directive flags and font color
                in_comment = false;
                in_preprocessor_directive = false;
                set_font_c(255, 255, 255);
            }
        }

        token = strtok(NULL, " "); // Get the next token
    }
}



/**/
int main(int argc, char **argv) {
    // printf("\n%s\n",argv[1]);
    // Example file content
    // const char *cCode = "int main() { return 0; }";
    // const char *mapContent = "Memory Sections Size VMA LMA";

    // Example keyword definitions
    // const KeywordFontColor cKeywords[] = {
    //     {"int", 0, 128, 255},  // Blue
    //     {"return", 0, 0, 255}, // Blue
    //     {NULL, 0, 0, 0}        // End marker
    // };
  KeywordFontColor cKeywords[] = {
    {"auto", 255, 128, 0, 0},      // Orange
    {"break", 255, 0, 0, 0},       // Red
    {"case", 255, 0, 0, 0},        // Red
    {"continue", 255, 0, 0, 0},    // Red
    {"do", 255, 0, 0, 0},          // Red
    {"default", 255, 0, 0, 0},     // Red
    {"//", 128, 128, 128, 1},      // Grey (for comments)
    {"/*", 128, 128, 128, 1},      //Grey (for comments)
    {"*/", 128, 128, 128, 1},      //Grey (for comments)
    {"else", 255, 0, 0, 0},        // Red
    {"extern", 0, 0, 255, 0},      // Blue
    {"for", 255, 0, 0, 0},         // Red
    {"if", 255, 0, 0, 0},          // Red
    {"goto", 255, 0, 0, 0},        // Red
    {"register", 0, 0, 255, 0},    // Blue
    {"return", 0, 0, 255, 0},      // Blue
    {"sizeof", 0, 0, 255, 0},      // Blue
    {"switch", 255, 0, 0, 0},      // Red
    {"volatile", 0, 0, 255, 0},    // Blue
    {"while", 255, 0, 0, 0},       // Red

    // C11 keywords
    {"_Alignas", 0, 255, 0, 0},    // Green
    {"_Alignof", 0, 255, 0, 0},    // Green
    {"_Atomic", 0, 255, 0, 0},     // Green
    {"_Generic", 0, 255, 0, 0},    // Green
    {"_Noreturn", 0, 255, 0, 0},   // Green
    {"_Static_assert", 0, 255, 0, 0}, // Green
    {"_Thread_local", 0, 255, 0, 0}, // Green

    // C99 keywords
    {"_Bool", 0, 255, 0, 0},       // Green
    {"_Complex", 0, 255, 0, 0},    // Green
    {"_Imaginary", 0, 255, 0, 0},  // Green

    // Types, variables & functions related
    {"bool", 0, 128, 255, 0},      // Blue
    {"char", 0, 128, 255, 0},      // Blue
    {"const", 0, 128, 255, 0},     // Blue
    {"double", 0, 128, 255, 0},    // Blue
    {"enum", 0, 128, 255, 0},      // Blue
    {"false", 0, 128, 255, 0},     // Blue
    {"float", 0, 128, 255, 0},     // Blue
    {"int", 0, 128, 255, 0},       // Blue
    {"long", 0, 128, 255, 0},      // Blue
    {"short", 0, 128, 255, 0},     // Blue
    {"signed", 0, 128, 255, 0},    // Blue
    {"static", 0, 128, 255, 0},    // Blue
    {"struct", 0, 128, 255, 0},    // Blue
    {"true", 0, 128, 255, 0},      // Blue
    {"typedef", 0, 128, 255, 0},   // Blue
    {"union", 0, 128, 255, 0},     // Blue
    {"unsigned", 0, 128, 255, 0},  // Blue
    {"void", 0, 128, 255, 0},      // Blue
    {"#", 255, 0, 255, 0},   // Magenta (for preprocessor directives)
    {NULL, 0, 0, 0, 0}             // End marker
};
    const KeywordFontColor mapKeywords[] = {
        {"Memory", 255, 128, 0}, // Orange
        {"Sections", 255, 0, 0}, // Red
        {"Size", 0, 128, 255},   // Blue
        {"VMA", 0, 128, 255},    // Blue
        {"LMA", 0, 128, 255},    // Blue
        {NULL, 0, 0, 0}          // End marker
    };

    // Highlight .c code
    FILE *file = fopen(argv[1],"r");
    if(file == NULL){
        printf("Error opening file %s\n", argv[1]);
        return -1;
    }
    size_t file_size = get_file_size_from_pointer(file);
    char *code= malloc(file_size);
    fread(code, 1, file_size, file);
    char extension[30];
    const char *dot = strrchr(argv[1], '.');
    if (!dot || dot == argv[1]) {
        strcpy(extension," "); // No file extension found or the dot is at the beginning of the path.
    }
    // return dot + 1;  // +1 to skip the dot.
    strcpy(extension,dot+1);
    if(strcmp(extension,".map") == 0)
    {
        highlight_keywords(code, mapKeywords);
    }
    else if (strcmp(extension,".c") == 0)
    {
       highlight_keywords(code, cKeywords);
    }
    else
    {
        printf("Unknown extension highlighting disabled\n\n"); 
        printf("%s\n",code);

    }
    
    free(code);
    return 0;
}
