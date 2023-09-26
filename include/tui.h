#ifndef __TUI__H
#define __TUI__H

typedef struct
{
    int row;
    int col;
    int width;
    int hight;
    char name[100];
}TUI_MAIN;

int printArrayWithSelection(char* arr[], int arrSize);
void tui_main(int argc, char *argv[]);
int tui_install();
#endif