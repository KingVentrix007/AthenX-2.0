#include "stdio.h"
#include "stdlib.h"
#include "file.h"
#include "editor.h"




// Initialize the terminal state
// Initialize the terminal state
void init_terminal(struct terminal_state* state, int screen_width, int screen_height, int font_width, int font_height) {
    state->screen_width = screen_width;
    state->screen_height = screen_height;
    state->font_width = font_width;
    state->font_height = font_height;
    state->lines_per_screen = screen_height / font_height;
    state->top_line_num = 0;
    state->total_lines = 0; // Initialize to 0
}

// Function to display text based on the terminal state
void display_text(struct terminal_state* state, char* buffer, int buffer_length) {
    int max_displayable_line = state->top_line_num + state->lines_per_screen - 1;
    for (int line_num = state->top_line_num; line_num <= max_displayable_line && line_num < buffer_length; line_num++) {
        int buffer_position = line_num * state->screen_width;
        // Display text from the buffer at buffer_position using printf
        printf("%s\n", &buffer[buffer_position]); // Modify the format according to your text format
    }
}

// Function to handle user input (arrow keys and ESC)
int handle_user_input(struct terminal_state* state) {
    char input = get_char(); // Get user input

    switch (input) {
        case SCAN_CODE_KEY_DOWN: // Up arrow key
            if (state->top_line_num > 0) {
                state->top_line_num--;
            }
            return 0;
            // break;
        case SCAN_CODE_KEY_UP: // Down arrow key
            if (state->top_line_num + state->lines_per_screen < state->total_lines) {
                state->top_line_num++;
            }
            return 0;
            // break;
        case SCAN_CODE_KEY_ESC: // ESC key (ASCII value)
            return 1; // Exit
        default:
        printf("input = %d(%s)\n", input,input);
            return -2;
            // break;
    }
    return 0; // Continue
}

int main(int argc, char argv[20][100])
{
    if(argc < 2)
    {
        return exit(-9);   
    }
    else
    {
        editor("/root/cmd.c");
    }
}

int editor(char filename[100]){
    // char filename[100]; // Provide the filename here
   int screen_width = 1280;
    int screen_height = 768;
    size_t buffer_length;
    FILE* f = fopen(filename, "ar");
    if (f == NULL) {
        printf("Error: File not found.\n");
        return 1;
    }

    long fileSize = get_file_size_from_pointer(f);
    char* buffer = (char*)malloc(fileSize);
 
    if (buffer == NULL) {
        fclose(f);
        printf("Error: Memory allocation failed.\n");
        return 1;
    }
    fread(buffer, f, fileSize);
    buffer_length = strlen(buffer);
    struct terminal_state state;
    init_terminal(&state, screen_width, screen_height, 8, 16);
    state.total_lines = buffer_length / (screen_width / 8); // Adjust based on your text data
    clear_screen();
    display_text(&state, buffer, fileSize);
    while (1) {
       
        int  user = handle_user_input(&state);
        if (user == 1) {
            break; // Exit on user request (ESC key)
        }
        else if (user != -1) 
        {
            clear_screen();
            display_text(&state, buffer, fileSize);
        }
        
        
      
        
    }

    free(buffer);
    fclose(f);
    return 0;
}