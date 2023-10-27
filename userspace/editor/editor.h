#ifndef __EDITOR__H
#define __EDITOR__H

struct terminal_state {
    int screen_width;
    int screen_height;
    int font_width;
    int font_height;
    int lines_per_screen;
    int top_line_num;
    int total_lines; // Add total_lines
};
void init_terminal_state(struct terminal_state* state, int x, int y, int buffer_length);
void display_text_with_scroll(struct terminal_state* state, char* buffer, int buffer_length);
void clear_and_update_screen(struct terminal_state* state, char* buffer, int buffer_length);
int editor(char filename[100]);
#endif