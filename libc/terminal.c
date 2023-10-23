#include "../include/syscall.h"
#include "stdio.h"


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