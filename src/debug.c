#include "ide.h"
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
#include "multiboot.h"
ADDER_NAME_LIST addr_name;
List myList;
struct Function functions[MAX_FUNCTIONS];
int numFunctions = 0;
//#define NO_IMG


void printMultibootInfo(MULTIBOOT_INFO *mb_info) {
    //  printf_("{/330:255,0,255}");
    // printf("Multiboot Information:\n");
    //  printf_("{/330:0,255,0}");
    // printf("Flags: 0x%x\n", mb_info->flags);
    // printf("Memory (Low): 0x%x\n", mb_info->mem_low);
    // printf("Memory (High): 0x%x\n", mb_info->mem_high);
    // printf("Boot Device: 0x%x\n", mb_info->boot_device);
    // printf("Kernel Command Line: 0x%x\n", mb_info->cmdline);
    
    // if (mb_info->flags & (1 << 3)) {
    //     printf("Modules Count: %d\n", mb_info->modules_count);
    //     printf("Modules Address: 0x%x\n", mb_info->modules_addr);
    // }
    // else
    // {
    //      printf_("{/330:255,0,0}");
    //      printf("No modules\n");
    //       printf_("{/330:0,255,0}");
    // }
    
    // if (mb_info->flags & (1 << 4) && mb_info->flags & (1 << 5)) {
    //     printf("Symbol Table Info (AOUT):\n");
    //     printf("Tabsize: %d\n", mb_info->u.aout_sym.tabsize);
    //     printf("Strsize: %d\n", mb_info->u.aout_sym.strsize);
    //     printf("Addr: 0x%x\n", mb_info->u.aout_sym.addr);
    // } else if (mb_info->flags & (1 << 4)) {
    //     printf("Symbol Table Info (ELF):\n");
    //     printf("Num: %d\n", mb_info->u.elf_sec.num);
    //     printf("Size: %d\n", mb_info->u.elf_sec.size);
    //     printf("Addr: 0x%x\n", mb_info->u.elf_sec.addr);
    //     printf("Shndx: %d\n", mb_info->u.elf_sec.shndx);
    // }
    
    // Add conditions for other flags as needed
     printf_("{/330:255,255,0}");
     printf("Multiboot memory map\n");
      printf_("{/330:0,255,0}");
    printf("Memory Map Length: %d\n", mb_info->mmap_length);
    printf("Memory Map Address: 0x%x\n", mb_info->mmap_addr);
    // Add more fields as needed
    printf_("{/330:255,0,0}");
    printf("Only applied to to moment grub was launched\n");
     printf_("{/330:0,255,0}");
    printf("Framebuffer Address: 0x%x\n", mb_info->framebuffer_addr);
    printf("Framebuffer Pitch: %d\n", mb_info->framebuffer_pitch);
    printf("Framebuffer Width: %d\n", mb_info->framebuffer_width);
    printf("Framebuffer Height: %d\n", mb_info->framebuffer_height);
    printf("Framebuffer BPP: %d\n", mb_info->framebuffer_bpp);
    
    // Print the color for framebuffer type
    char *framebuffer_type_color = "";
    switch (mb_info->framebuffer_type) {
        case 0:
            framebuffer_type_color = "RGB";
            break;
        case 1:
            framebuffer_type_color = "Indexed";
            break;
        case 2:
            framebuffer_type_color = "EGA";
            break;
    }
    printf("Framebuffer Type (Color): [%d, %d, %d] %s\n",
           0, 0, 0, framebuffer_type_color);
}


int screen_of_death()
{
    //sleep(10);
    set_background_color(0,0,0);
    for (size_t y = 0; y < 1024; y++)
    {
        for (size_t x = 0; x < 1280; x++)
        {
            vbe_putpixel(x,y,VBE_RGB(0,0,0));
        }
        
    }
    #ifndef NO_IMG
    draw_error_image();
    #endif
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
    addr_name.stack_ptr = 0;
    addr_name.esp = 0;
    addr_name.esp_in = 0;
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
        printf_("{/330:255,255,0}");
        //printf("\n0x%016x\n",num);
        //const struct Function *func = resolve_function_name(num,functions,numFunctions);
        //  printf("\n%s 0x%016x -- num 0x%016x\n",func->name,func->address,num);
         ADDER_NAME_LIST *addr = find_largest_smaller_address(&myList,num);
         printf("%s line: %d : %s\n",addr->file_name,addr->line,addr->names);
         
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
    else if (strcmp(arguments[0],"cls") == 0)
    {
        screen_of_death();
        set_screen_x(0);
        set_screen_y(0);
    }
    
    else if(strcmp(arguments[0], "dump") == 0)
    {
        if(strcmp(arguments[1],"mem") == 0)
        {
            KERNEL_MEMORY_MAP *out;
            get_map(out);
            display_kernel_memory_map(out);
        }
        else if(strcmp(arguments[1],"multiboot") == 0)
        {
            MULTIBOOT_INFO *info = get_mb_info();
            printMultibootInfo(info);
            
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
int get_adder_map(char output[71629])
{
    // char output[71629] = {0};
    // memcpy(output, adder,size);
    // printf("\n%s",output);
    // int drive = 1;
    // char b[100];
    // char buffer[512] = {0};
    // char output[71629] = {0};
    // memset(buffer,0,sizeof(buffer));
    // int lba = 0;
    // memcpy(output,adder,size);
    // //ide_read_sectors(drive,1,lba,(uint32)buffer);
    // //printf("\nOutput:\n%s",buffer);
    // int start = 0;
    // printf("\n%s",output);
    // for (size_t i = 1; i < strlen("Memory Configuration")+1; i++)
    //     {
    //             append(b,buffer[i]);
    //     }
    //         printf("\nIUT:0%s\n",b);
     
    if(1==1)
    {
        printf("Found MAP\n");
        //ide_read_sectors(drive,(71629/512),0,(uint32)output);
        //printf("OUTPUT:\n%s",output);
        

    // Parse the memory map data and get the number of functions
        numFunctions = parseMemoryMap(output, functions);
        printf("");
    // // Now you have a list of functions and their addresses
    // // You can loop through the list as needed
        
        for (int i = 0; i < numFunctions; i++) {
            //printf("Function name: %s, Address: 0x%016x\n", functions[i].name, functions[i].address);
        }
    //     // for (size_t q = 0; q < 71629; q++)
    //     // {
    //     //     ide_read_sectors(drive,1,q,(uint32)buffer);
    //     //     for (size_t i = start; i < start+512; i++)
    //     //     {
                
    //     //         output[i] = buffer[i];
    //     //     }
    //     //     memset(buffer,0,sizeof(buffer));
    //     //     start = start+512;
    //     // }
        
        
    }
}
char* findNextSpaceOrNewline(char* ptr) {
    while (*ptr != ' ' && *ptr != '\n' && *ptr != '\0') {
        ptr++;
    }
    return ptr;
}
unsigned long long parseHex(const char* str) {
    unsigned long long result = 0;
    while (*str != '\0') {
        char c = *str;
        result *= 16;
        if (c >= '0' && c <= '9') {
            result += c - '0';
        } else if (c >= 'a' && c <= 'f') {
            result += c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            result += c - 'A' + 10;
        } else {
            break; // Stop parsing if a non-hex character is encountered
        }
        str++;
    }
    return result;
}
// Function to parse memory map data and create a list of functions
int parseMemoryMap(const char* mapData, struct Function* functions) {
    // Initialize a pointer to the start of the buffer
    const char* ptr = mapData;

    // Read and parse the memory map data
    int count = 0;
    while (count < MAX_FUNCTIONS && *ptr != '\0') {
        // Skip leading spaces
        while (*ptr == ' ') {
            ptr++;
        }

        // Check if the line starts with "0x" indicating an address
        if (*ptr == '0' && (*(ptr + 1) == 'x' || *(ptr + 1) == 'X')) {
            // Skip the "0x" prefix
            ptr += 2;

            // Parse the hexadecimal address
            uint64_t address = 0;
            while ((*ptr >= '0' && *ptr <= '9') || (*ptr >= 'a' && *ptr <= 'f') || (*ptr >= 'A' && *ptr <= 'F')) {
                char digit = *ptr;
                if (digit >= 'a' && digit <= 'f') {
                    digit = digit - 'a' + 'A'; // Convert to uppercase
                }
                address = (address << 4) | (digit - '0');
                ptr++;
            }

            // Skip spaces
            while (*ptr == ' ') {
                ptr++;
            }

            // Find the end of the function name
            char *nameStart = ptr;
            while (*ptr != '\n' && *ptr != '\0') {
                ptr++;
            }

            // Copy the function name into the struct
            int nameLength = ptr - nameStart;
            if (nameLength > 99) {
                nameLength = 99; // Limit the function name length to 99 characters
            }
            strncpy(functions[count].name, nameStart, nameLength);
            functions[count].name[nameLength] = '\0'; // Null-terminate the name

            functions[count].address = address;
            count++;
        } else {
            // Move to the next line
            while (*ptr != '\n' && *ptr != '\0') {
                ptr++;
            }
            if (*ptr == '\n') {
                ptr++; // Move past the newline character
            }
        }
    }

    return count; // Return the number of functions found
}

const struct Function* resolve_function_name(uintptr_t address, const struct Function* functions, int numFunctions) {
    //printf("\n0x%016x",address);
    const struct Function* result = NULL;
    for (int i = 0; i < numFunctions; i++) {
        if (functions[i].address <= address && (result == NULL || functions[i].address > result->address)) {
            result = &functions[i];
        }
    }
    //printf("\n0x%016x",address);
    return result;
}