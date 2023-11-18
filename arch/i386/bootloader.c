#include "../include/vga.h"
#include "../include/printf.h"
#include "../include/stdio.h"
#include "../include/keyboard.h"
#include "../include/vesa_display.h"
#include "../include/vesa.h"
#include "../include/bootloader.h"
#include "kernel.h"

void loader(MULTIBOOT_INFO *multiboot)
{
    cls_screen(VBE_RGB(0,0,0));
    
    
}