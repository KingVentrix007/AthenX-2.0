#include "keyboard.h"
#include "string.h"
int text_editor(int max_letters,char *dest)
{
    char out[max_letters];
    while(1==1)
    {
            char c = kb_getchar();
            if(c == '\b')
            {
                //crude_song();
                if(backspace(out))
                {
                    printf("\b");
                }
                else
                {
                    beep();
                }
                
                //printf("\b");
            }
            else if (c == '`')
            {
                break;
                //printf("\n");
                // cmd_handler(buffer);
                // buffer[0] = '\0';
                // next_line();
                // set_screen_x(0);
                // //set_terminal_colum(get_terminal_col()+16);
                // //set_terminal_row(0);
                // printf(">");
                //crude_song();
            }
            
            else
            {
                
                char* s;
                s = ctos(s, c);
                //printf(s);
                printf(s);
                //printf(s);
                
                append(out,c);
            }
    }
    memcpy(dest,out,sizeof(out));

}