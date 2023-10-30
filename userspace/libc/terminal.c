#include "syscall.h"
#include "stdio.h"
#include "terminal.h"

uint32 vbe_rgb(uint8 red, uint8 green, uint8 blue) {
    uint32 color = red;
    color <<= 16;
    color |= (green << 8);
    color |= blue;
    return color;
}
int set_terminal_state(int x, int y, int r, int g, int b)
{
    terminal_struct *term;
    term->x = x; term->y = y;
    term->color = vbe_rgb(r, g, b);
    syscall(SYS_TERMINAL,0,term);
};
int set_pos(int x,int y)
{
    terminal_struct term;
    term.x = x; term.y = y;
    // term->color = vbe_rgb(r, g, b);
    syscall(SYS_TERMINAL,1,&term);
}
int get_x() {
    int x = syscall(SYS_GET_X, 90, 90);
    printf("\nx=%d\n", x); // Use %d to print x as an integer
    return x;
}
int get_y()
{
    return syscall(SYS_GET_Y,0,0);
}
int set_x(int x)
{ 
    
    syscall(SYS_SET_X,x,0); 
}
int set_y(int y)
{
    syscall(SYS_SET_Y,y,0);
}
int set_font_c(int R, int G, int B)
{
    terminal_struct term; 
    term.color = vbe_rgb(R,G,B);
    syscall(SYS_SET_FONT_C,&term,0);
}

int set_bg_c(int R, int G, int B)
{
     terminal_struct term;
    term.color = vbe_rgb(R,G,B);
    syscall(SYS_SET_BG_COLOR,&term,0);
}
