
#include "keyboard.h"
#include "string.h"
#include "display.h"
// #include "ide.h"
#include "printf.h"
#include "debug.h"
#include "fs.h"
int text_editor(int max_letters,char dest[MAX_FILE_SIZE])
{
    DEBUG("H");
    int max = 0;
    char out[1024];
    
    while(1==1)
    {
            char c = kb_getchar();
            if(c == '\b')
            {
                //crude_song();
                if(backspace(out))
                {  
                    //max = max +1;
                    printf_("\b");
                }
                else
                {
                    beep();
                }
                
                //printf_("\b");
            }
            else if (c == '`')
            {
                append(out,'\0');
                break;
                //printf_("\n");
                // cmd_handler(buffer);
                // buffer[0] = '\0';
                // next_line();
                // set_screen_x(0);
                // //set_terminal_colum(get_terminal_col()+16);
                // //set_terminal_row(0);
                // printf_(">");
                //crude_song();
            }
            
            else if(max < max_letters && c != '\0')
            {
                
                char* s;
                s = ctos(s, c);
                //printf_(s);
                printf_(s);
                //printf_(s);
                max = max + 1;
                append(out,c);
            }
            if(max >= max_letters)
            {
                beep();
            }
    }
    memcpy(&dest,out,sizeof(dest));
    return strlen(out);

}