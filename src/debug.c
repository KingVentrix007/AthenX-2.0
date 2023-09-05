
#include <string.h>
#include <vesa.h>
#include <isr.h>
#include "debug.h"
#include "timer.h"
#include "maths.h"
#include "keyboardmap.h"
#include "display.h"
#include "keyboard.h"
#include "kernel.h"
#include "graphics.h"
ADDER_NAME_LIST addr_name;
List myList;

int screen_of_death()
{
    //sleep(10);
    set_background_color(0,51,187);
    for (size_t y = 0; y < 1024; y++)
    {
        for (size_t x = 0; x < 1280; x++)
        {
            vbe_putpixel(x,y,VBE_RGB(0,51,187));
        }
        
    }
    draw_error_image();
}
bool is_in_list(List* list, ADDER_NAME_LIST item) {
    for (int i = 0; i < list->size; i++) {
        if (strcmp(list->nodes[i].data.names, item.names) == 0 &&
            list->nodes[i].data.addr == item.addr) {
            return true;
        }
    }
    return false;
}
void append_to_list_if_not_exists(List* list, ADDER_NAME_LIST item) {
    if (list->size >= MAX_LIST_SIZE) {
        //printf("List is full. Cannot add more items.\n");
        return;
    }

    if (!is_in_list(list, item)) {
        list->nodes[list->size].data = item;
        list->size++;
    }
}

void append_name_addr(char name[20],uint32 addr,uint32 line,char filename[20])
{
    addr_name.addr = addr;
    strcpy(addr_name.names,name);
    strcpy(addr_name.file_name,filename);
    addr_name.line = line;
    append_to_list_if_not_exists(&myList,addr_name);
    
    
}

void get_name_addr(ADDER_NAME_LIST *out)
{
    memcpy(out,&addr_name,sizeof(addr_name));
}

void get_name_adder_list(List *out)
{
    memcpy(out,&myList,sizeof(myList));
}

bool find_by_address(uint32_t *targetAddr) {
    
    
    for (int i = 0; i < myList.size; i++) {
        if (myList.nodes[i].data.addr == targetAddr) {
            return true;
        }
    }
    return false;
}

void print_list(uint32 num) {
   
    if(num == 0)
    {
        printf_("List Contents:\n");
        printf_("Function name:      Function Address:\n");
        for (int i = 0; i < myList.size; i++) {
        //printf("Item %d:    ", i + 1);
        printf_("%s  ", myList.nodes[i].data.names);
        printf_("0x%x\n", myList.nodes[i].data.addr);
        }
    }
    else
    {
         for (int i = 0; i < myList.size; i++) {
        //printf("Item %d:    ", i + 1);
            double perc = check_similarity(num, myList.nodes[i].data.addr);
            if (perc >= 90.0)
            {
                //printf("There is a %f%% probability that this function coursed the error %s\n",perc,myList.nodes[i].data.names);
                 double perc_2 = check_similarity(100.0, perc);
                 if(perc_2 >= 99.99)
                 {
                    printf_("Course of error: %s in %s at line %d. Address : 0x%x  : %f%% probability\n",myList.nodes[i].data.names,myList.nodes[i].data.file_name,myList.nodes[i].data.line,myList.nodes[i].data.addr,perc);
            }
                 }
                
        }
    }
    
}

double check_similarity(double num1, double num2) {
    // Calculate the absolute difference between the two numbers
    double diff = fabs(num1 - num2);
    
    // Calculate the maximum possible difference between the two numbers
    double max_diff = fmax(fabs(num1), fabs(num2));
    
    // Calculate the similarity as a percentage
    double similarity_percentage = ((max_diff - diff) / max_diff) * 100.0;
    
    return similarity_percentage;
}

void print_stack_trace(void *ebp) {
    void **current_ebp = ebp;
    void *return_address;
    uint32* address_list;

    printf_("Stack Trace:\n");

    while (current_ebp) {
        return_address = *(current_ebp + 1);
        printf_("  0x%p\n", return_address);
        address_list = current_ebp;
        address_list++;
        current_ebp = *current_ebp;
    }
    //function_name_from_address(address_list);
}

uint8_t scan_debug(void)
{
    unsigned char brk;
    static uint8_t key = 0;
    uint8_t read_char = input_bytes(0x60); // keyboard port
    uint8_t old_char = read_char; 
    brk = read_char & 0x80;
    read_char = read_char & 0x7f;
    if (brk)
    {
        return key = 0;
    }
    else if(input_bytes(0x60) == 0x0f)
    {
        return "F1";
    }
    else if (read_char != key)
    {
        return key = read_char;
    }
    else
    {
        return 0;
    }
}

char *ctos_debug(char s[2], const char c)
{
     s[0] = c;
     s[1] = '\0';
     return s;
}


#define MAX_COMMAND_LENGTH_DEBUG 50
#define MAX_ARGUMENTS_DEBUG 10
void parse_command_debug(const char* command) {
    FUNC_ADDR_NAME(&parse_command_debug);
    char command_copy[MAX_COMMAND_LENGTH_DEBUG];
    strcpy(command_copy, command);

    // Tokenize the command by spaces
    char* token;
    char* arguments[MAX_ARGUMENTS_DEBUG];
    int arg_count = 0;

    token = strtok(command_copy, " ");
    while (token != NULL) {
        arguments[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    // Check for different commands and execute them
    if (strcmp(arguments[0], "help") == 0) {

    }
    if(strcmp(arguments[0], "dump") == 0)
    {
        if(strcmp(arguments[1],"mem") == 0)
        {
            KERNEL_MEMORY_MAP *out;
            get_map(out);
            display_kernel_memory_map(out);
        }
    }
    if(strcmp(arguments[0],"stack") == 0)
    {   

        uint32 *eip = atoi(arguments[1]);
        
        print_stack_trace(eip);
    }
    if(strcmp(arguments[0],"addr") == 0)
    {
        print_list(0);
    }
    else if(strcmp(arguments[0],"inspect") == 0)
    {
        uint32 addr = atoi(arguments[1]);
        asm volatile (
        "call *%0"
        :
        : "r"(addr)
        : "memory"
    );
        //&addr;
    }
    else
    {
        printf_("%s is not a valid command", arguments[0]);
    }
    
}



void debug_terminal()
{
    //keyboard_init();
    printf_("Debug terminal started");
    char *buffer[512];
     uint8_t byte;
    //char *buffer[512];
     buffer[0] = '\0';
    printf_("\n@DEBUG>");
    while(1)
    {    
            
            //beep();
            char c = kb_getchar();
            
            if(c == '\b')
            {
                //crude_song();
                if(backspace(buffer))
                {
                    printf_("\b");
                    //set_cursor_x(get_cursor_x()-2);
                    //printf(" ");
                    //console_ungetchar();
                }
                else
                {
                    beep();
                }
                
                //printf("\b");
            }
            else if (c == '\n')
            {
                //printf("\n");
                parse_command_debug(buffer);
                memset(buffer, 0,sizeof(buffer));
                next_line();
                set_screen_x(0);
                //set_terminal_colum(get_terminal_col()+16);
                //set_terminal_row(0);
                printf_("@DEBUG>");
                //crude_song();
            }
            
            else
            {
                
                char* s;
                s = ctos(s, c);
                //printf(s);
                printf_(s);
                //printf(s);
                
                append(buffer,c);
                //printf("HERE");
            }
            
            
    }
}