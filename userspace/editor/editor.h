#ifndef __EDITOR__H
#define __EDITOR__H

struct terminal_state
{
    int pixels_per_line; /* number of pixels per line */
    int pixels_per_column; /* number of pixels per column */
    int p_width; /* width of screen in pixels */
    int p_height; /* height of screen in pixels */
    int current_position_x; /* current position if cursor x */
    int current_position_y; /* current position if cursor y */
    int file_pos_at_00; /* position of data in file at 0 0 on the screen */
    int file_pos_at_END; /* position of data in file at the end of the sceen */
    int top_line_num; /* the index of the top line on the screen */
    int total_lines; /* total number of lines on the int the file */

};
void init_terminal_state(struct terminal_state* state, int x, int y, int buffer_length);
void display_text_with_scroll(struct terminal_state* state, char* buffer, int buffer_length);
void clear_and_update_screen(struct terminal_state* state, char* buffer, int buffer_length);
int editor(char filename[100]);
#endif