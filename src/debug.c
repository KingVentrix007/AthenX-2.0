
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

void append_name_addr(char name[20],uint32 addr,int num_parms,char param_list[10],uint32 line,char filename[20])
{
    addr_name.addr = addr;
    strcpy(addr_name.names,name);
    strcpy(addr_name.file_name,filename);
    addr_name.line = line;
    addr_name.num_params = num_parms;
    strcpy(addr_name.addrtype,param_list);
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

ADDER_NAME_LIST *print_list(uintptr_t num) {
   
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
        printf_("{/330:0,255,0}");
         ADDER_NAME_LIST *addr = find_largest_smaller_address(&myList,num);
         printf("%s(%d) : %s",addr->file_name,addr->line,addr->names);
          printf_("{/330:0,255,0}");
          return addr;
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
// Function to find the largest address smaller than the provided address
const ADDER_NAME_LIST *find_largest_smaller_address(const List *list, uint32 search_addr) {
    const ADDER_NAME_LIST *result = NULL;
    for (int i = 0; i < list->size; ++i) {
        if (list->nodes[i].data.addr < search_addr) {
            if (result == NULL || list->nodes[i].data.addr > result->addr) {
                result = &list->nodes[i].data;
            }
        }
    }
    return result;
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
void parse_command_debug(const char* command, REGISTERS* reg) {
    FUNC_ADDR_NAME(&parse_command_debug,2,"cu");
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
    else if(strcmp(arguments[0], "dump") == 0)
    {
        if(strcmp(arguments[1],"mem") == 0)
        {
            KERNEL_MEMORY_MAP *out;
            get_map(out);
            display_kernel_memory_map(out);
        }
    }
    else if(strcmp(arguments[0],"stack") == 0)
    {   

        int *level = atoi(arguments[1]);
        
        //print_stack_trace(eip);
        unwind_stack(reg,level);
        
    }
    else if(strcmp(arguments[0],"addr") == 0)
    {
        print_list(0);
    }
    else if(strcmp(arguments[0],"inspect") == 0)
    {
    //     uint32 addr = atoi(arguments[1]);
    //     asm volatile (
    //     "call *%0"
    //     :
    //     : "r"(addr)
    //     : "memory"
    // );
        //&addr;
    }
    else
    {
        printf_("%s is not a valid command", arguments[0]);
    }
    
}



void debug_terminal(REGISTERS *regs)
{
    //keyboard_init();
    printf("Debug terminal started");
    char *buffer[512];
     uint8_t byte;
    //char *buffer[512];
     buffer[0] = '\0';
    printf("\n@DEBUG>");
    while(1)
    {    
            
            //beep();
            char c = kb_getchar();
            
            if(c == '\b')
            {
                //crude_song();
                if(backspace(buffer))
                {
                    printf("\b");
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
                printf("\n");
                parse_command_debug(buffer,regs);
                memset(buffer, 0,sizeof(buffer));
                next_line();
                //set_screen_x(0);
                //set_terminal_colum(get_terminal_col()+16);
                //set_terminal_row(0);
                printf("\n@DEBUG>");
                //crude_song();
            }
            
            else
            {
                
                char* s;
                s = ctos(s, c);
                //printf(s);
                printf(s);
                //printf(s);
                
                append(buffer,c);
                //printf("HERE");
            }
            
            
    }
}


void getRegisters(REGISTERS* regs) {
    uint32_t ds, edi, esi, ebp, esp, ebx, edx, ecx, eax, eip, cs, eflags, ss;

    //int eax, ebx, ecx, edx,esi,edi,esp,ebp;

    // Inline assembly to move register values to variables
    asm volatile(
        "movl %%eax, %0\n\t"
        "movl %%ebx, %1\n\t"
        "movl %%ecx, %2\n\t"
        "movl %%edx, %3\n\t"
        
        : "=r" (eax), "=r" (ebx), "=r" (ecx), "=r" (edx)
        :
        :);
    asm volatile(
        "movl %%esi, %0\n\t"
        "movl %%edi, %1\n\t"
        "movl %%esp, %2\n\t"
        "movl %%ebp, %3\n\t"

        : "=r" (esi), "=r" (edi), "=r" (esp), "=r" (ebp)
        :
        :);
    
    // asm volatile(
    //     "movl %%eip, %0\n\t"
    //     "movl %%cs, %1\n\t"
    //     "movl %%eflags, %2\n\t"

    //     :"=r" (eip), "=r" (cs), "=r" (eflags)
    //     :
    //     :);
    // Copy the values to the REGISTERS struct
    regs->ds = ds;
    regs->edi = edi;
    regs->esi = esi;
    regs->ebp = ebp;
    regs->esp = esp;
    regs->ebx = ebx;
    regs->edx = edx;
    regs->ecx = ecx;
    regs->eax = eax;
    regs->eip = eip;
    regs->cs = cs;
    regs->eflags = eflags;
    //regs->ss = ss;
}