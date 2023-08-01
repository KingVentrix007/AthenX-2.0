#include "keyboard.h"
#include "string.h"
#include "display.h"
// #include "ide.h"
int text_editor(int max_letters,char *dest)
{
    int max = 0;
    char out[max_letters];
    while(1==1)
    {
            char c = kb_getchar();
            if(c == '\b')
            {
                //crude_song();
                if(backspace(out))
                {  
                    //max = max +1;
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
                append(out,'\0');
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
            
            else if(max < max_letters)
            {
                
                char* s;
                s = ctos(s, c);
                //printf(s);
                printf(s);
                //printf(s);
                max = max + 1;
                append(out,c);
            }
            if(max >= max_letters)
            {
                beep();
            }
    }
    memcpy(dest,out,sizeof(out));

}