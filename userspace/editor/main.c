#include "stdio.h"
#include "stdlib.h"
#include "file.h"
#include "editor.h"




void init_terminal_state(struct terminal_state* state, int x, int y, int buffer_length) {
    int font_width = 8; // Width of each character in pixels
    int font_height = 16; // Height of each character in pixels
    int lines_per_screen = y / font_height;

    state->pixels_per_line = font_width;
    state->pixels_per_column = font_height;
    state->p_width = x;
    state->p_height = y;
    state->current_position_x = 0;
    state->current_position_y = 0;
    state->file_pos_at_00 = 0;
    state->file_pos_at_END = buffer_length;
    state->top_line_num = 0;
    state->total_lines = buffer_length / (x / font_width); // Adjust this based on your text data
}

int main(int argc, char argv[20][100])
{
    if(argc < 2)
    {
        return exit(-9);   
    }
    else
    {
        editor(argv[1]);
    }
}

int editor(char filename[100]) {
    FILE *f = fopen("/root/cmd.c", "rb");
    if(f == NULL) {return -6;}
    // long fileSize = get_file_size_from_pointer(f);
    char buffer[1024*3];
    fread(buffer,f,sizeof(buffer));
    if (buffer != NULL) {
        int x = 1280; // Screen width
        int y = 768;  // Screen height

        // Pass the screen dimensions and buffer length to init_terminal_state
        struct terminal_state state;
        init_terminal_state(&state, x, y, 1024*3);
        
        while (1) {
            // Handle user input and update the terminal state
            char scancode = get_char(); // Implement your get_scancode function
           
            if (scancode == SCAN_CODE_KEY_UP) {
                // Handle scrolling up
                if (state.top_line_num > 0) {
                    state.top_line_num--;
                }
                clear_and_update_screen(&state, buffer, 1024*3);
            } else if (scancode == SCAN_CODE_KEY_DOWN) {
                // Handle scrolling down
                if (state.top_line_num + (y / state.pixels_per_column) < state.total_lines) {
                    state.top_line_num++;
                }
                clear_and_update_screen(&state, buffer, 1024*3);
            }
            else{

                // printf("%c",scancode);
            }
            

            // Clear the screen and update the displayed text
            
            //  printf("scancode = %d\n", scancode);
            //  printf("buffer = %s\n", buffer);

            // You can introduce a sleep to control the loop speed
            // usleep(10000); // Sleep for 10 milliseconds (adjust as needed)

            // Exit the loop or handle an exit condition (e.g., press a specific key)
            if (scancode == SCAN_CODE_KEY_ESC) {
                break;
            }
        }

        // free(buffer);
    } else {
        printf("Memory allocation failure\n");
        return -2;
        // Handle the case where memory allocation fails
    }

    // Return an appropriate value at the end of your function
    return 0; // Replace with your actual return value
}




void clear_and_update_screen(struct terminal_state* state, char* buffer, int buffer_length) {
    // Clear the screen (you need to implement this part based on your environment)
    // This could involve printing special characters to clear the screen or using system-specific functions.
    clear_screen();
    // Display the text with scrolling
    display_text_with_scroll(state, buffer, buffer_length);

    // You may need to flush or update the display to make the changes visible,
    // depending on your environment.
}
void display_text_with_scroll(struct terminal_state* state, char* buffer, int buffer_length) {
    // printf("display text with scroll\n");
    // Calculate the number of visible lines on the screen
    int visible_lines = state->p_height / state->pixels_per_column;

    // Calculate the maximum line that can be displayed on the screen without scrolling
    int max_displayable_line = state->top_line_num + visible_lines - 1;

    // Display text within the visible portion of the screen
    for (int line_num = state->top_line_num; line_num <= max_displayable_line && line_num < state->total_lines; line_num++) {
        // Calculate the position in the buffer corresponding to this line
        int buffer_position = state->file_pos_at_00 + (line_num - state->top_line_num);

        // Display the text from the buffer at buffer_position using printf
        printf("%s\n", &buffer[buffer_position]); // Modify the format according to your text format
    }
    
    // Handle scrolling when arrow keys are pressed
    // int scancode = get_scancode(); // Get the scancode for the pressed key

    // if (scancode == UP_ARROW_SCANDCODE) {
    //     // Handle scrolling up
    //     if (state->top_line_num > 0) {
    //         state->top_line_num--;
    //     }
    // } else if (scancode == DOWN_ARROW_SCANDCODE) {
    //     // Handle scrolling down
    //     if (max_displayable_line < state->total_lines - 1) {
    //         state->top_line_num++;
    //     }
    // }
}

// 




