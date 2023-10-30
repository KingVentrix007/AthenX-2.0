#include "../include/vesa.h"
#include "../include/isr.h"
#include "../include/printf.h"
#include "../include/keyboard.h"
#include "../userspace/libc/syscall.h"
#include "../include/fat_filelib.h"
#include "../include/kheap.h"
#include "../include/exe.h"
#include "../include/string.h"
uintptr_t memory_alloated[1000];
FILE files_opened[1000];
void handle_print_system_call(REGISTERS *reg) {
    // Assuming the message is stored in reg->eax
    const char *message = (const char *)reg->ebx;

    // Display the message (you need to implement the display function)
    printf(message);
    // const char *message2 = (const char *)reg->ebx;
    // printf(message2);
}


int handle_fwrite_system_call(FILE *file, char *buffer) {
    // FILE *file = fopen(filename, "w");
    // if (file == NULL) {
    //     // Error opening the file for writing
    //     return 1;
    // }

    // Write the contents of the buffer to the file
    if (fputs(buffer, file) == EOF) {
        // Error writing to the file
        fclose(file);
        return 2;
    }

    // Close the file after writing
    fclose(file);

    // Reopen the file for reading
    // file = fopen(filename, "r");
    // if (file == NULL) {
    //     // Error opening the file for reading
    //     return 3;
    // }

    // char readBuffer[1024]; // Assuming a maximum line length of 1024 characters
    // if (fgets(readBuffer, sizeof(readBuffer), file) == NULL) {
    //     // Error reading from the file
    //     fclose(file);
    //     return 4;
    // }

    // // Close the file after reading
    // fclose(file);

    // // Compare the read data with the original buffer
    // if (strcmp(buffer, readBuffer) == 0) {
    //     return 0; // Data matches
    // } else {
    //     return -1; // Data doesn't match
    // }
}

char* handler_get_char_system_call(REGISTERS *reg)
{
    asm("sti");
    //printf("sys call get chr\n");
     //return "C";
     
        while(1)
        {
            char *c = get_chr();
            if(c == -1 || c == 0)
            {

            }
            else
            {
                return c;
            }
           
        }
        
        //printf("C%c",chr);
     
    
}
int screen_ctrl()
{

}


// Global array to track memory allocations
#define MAX_ALLOCATIONS 100
void* allocated_memory[MAX_ALLOCATIONS] = {NULL};
size_t allocated_memory_sizes[MAX_ALLOCATIONS] = {0};
int num_allocations = 0; // Number of allocations currently tracked
size_t total_memory_used = 0; // Total memory used by tracked allocation
size_t total_memory_used_allocated = 0; // Total memory allocated by user program
/**
 * Function Name: track_memory_allocation
 * Description: Tracks memory allocations by storing the allocated pointer in the global array and adding the size to the total memory used.
 *
 * Parameters:
 *   ptr (void*) - Pointer to the allocated memory
 *   size (size_t) - Size of the allocated memory
 *
 * Return:
 *   int - 1 if memory is successfully tracked, 0 if the array is full
 */
int track_memory_allocation(void* ptr, size_t size) {
    if (num_allocations < MAX_ALLOCATIONS) {
        allocated_memory[num_allocations] = ptr;
        allocated_memory_sizes[num_allocations] = size;
        num_allocations++;
        total_memory_used += size;
        total_memory_used_allocated+= size;
        return 1; // Memory successfully tracked
    }
    return 0; // Array is full
}

/**
 * Function Name: track_memory_deallocation
 * Description: Tracks memory deallocation by removing the pointer from the global array and subtracting the size from the total memory used.
 *
 * Parameters:
 *   ptr (void*) - Pointer to the deallocated memory
 */
void track_memory_deallocation(void* ptr) {
    for (int i = 0; i < num_allocations; i++) {
        if (allocated_memory[i] == ptr) {
            // Subtract the deallocated memory size from the total
            total_memory_used -= allocated_memory_sizes[i];

            // Shift remaining entries to fill the gap
            for (int j = i; j < num_allocations - 1; j++) {
                allocated_memory[j] = allocated_memory[j + 1];
                allocated_memory_sizes[j] = allocated_memory_sizes[j + 1];
            }

            num_allocations--;
            return;
        }
    }
}

/**
 * Function Name: deallocate_untracked_memory
 * Description: Deallocates any memory left in the global array that was not explicitly deallocated and updates the total memory used.
 */
void deallocate_untracked_memory() {
    printf("Allocated memory size: %d\n", total_memory_used_allocated);
    printf("Total memory size not deallocated: %d\n",total_memory_used);
    // printf("Freed %d allocated memory units:\n",num_allocations);
    for (int i = 0; i < num_allocations; i++) {
        total_memory_used -= allocated_memory_sizes[i];
        kfree(allocated_memory[i]);
        allocated_memory[i] = NULL;
        allocated_memory_sizes[i] = 0;
    }
    // printf("Allocated memory size: %d\n", total_memory_used_allocated);
    // printf("Total memory size not deallocated: %d\n",total_memory_used);
    printf("Freed %d allocated memory units:\n",num_allocations);
    num_allocations = 0;
}
uint8_t process_stack_user[8192] __attribute__((aligned(16)));
int ehco_mode = false;
int system_call_handler_c(int syscall_number, int param1, int param2) {
    int result = 0;
     terminal_struct *term;
     FILE *fp;
     parameters *parmss;
     struct syscall *s;
     Entry dirs[MAX];
    Entry files[MAX];
     int dir_count = 0;
            int file_count = 0;
             char b[5] = "f";
    //  printf("SYSCALL %d -> \n", syscall_number);
    // printf("%d", syscall_number);
    // printf("param1: %d\n", param1);
    // printf("param2: %d\n", param2);
   switch (syscall_number) {
        case SYS_EXIT:
            printf("SYSCALL SYS_EXIT coming soon\n");
            break;
        case SYS_FORK:
            printf("SYSCALL SYS_FORK coming soon\n");
            break;
        case SYS_READ:
            parmss = (parameters *)param1;
            fp = (FILE *)param2;
            int count=0;
            // printf(
            //     "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
            // );
            // printf("%d bytes",10);
            result = 0;
            char buffer[1001]; // Buffer to store the characters read, including the null-terminator
            int index = 0;

            int ch;
            while (index < 1000 && (ch = fgetc(fp)) != EOF) {
                buffer[index++] = (char)ch;
            }
            memcpy((char *)param1, buffer,sizeof(buffer));

    buffer[index] = '\0'; // Null-terminate the string
            // char line[256]; // Adjust the buffer size as needed
            // while (fgets(line, sizeof(line), fp) != NULL) {
            //     printf("FILE->%s", line);
            // }
            // result  = fl_fgetc(fp);
            // result = fl_fread(param1, sizeof);
            break;
        case SYS_WRITE:
            result = handle_fwrite_system_call((FILE *)param1,(char *)param2);
            break;
        case SYS_OPEN:
           fp = fopen(param1, param2);
           result = (int)fp;
            break;
        case SYS_CLOSE:
            fp = (FILE *)param1;
            fclose(fp);
            break;
        case SYS_LSEEK:
            fp = (FILE *)param1;
            parmss = (parameters *)param2;
            fl_fseek(fp,parmss->param1,parmss->param2);
            break;
        case SYS_FSEEK:
            fp = (FILE *)param1;
            parmss = (parameters *)param2;
            result = fl_fseek(fp,parmss->param3,parmss->param2);
            break;
        case SYS_MMAP:
            result = kmalloc(param1);
            
            int ret = track_memory_allocation(result,param1);
            if(ret == 0)
            {
                printf("MAXIMUM MEMORY ALLOCATION REACHED\n");
                result = NULL;
            }
            break;
        case SYS_MUNMAP:
            kfree(param1);
            track_memory_deallocation(param1);
            break;
        case SYS_DUP:
            printf("SYSCALL SYS_DUP coming soon\n");
            break;
        case SYS_DUP2:
            printf("SYSCALL SYS_DUP2 coming soon\n");
            break;
        case SYS_EXECVE:
            load_exe_file(param1, process_stack_user);
            break;
        case SYS_WAITPID:
            printf("SYSCALL SYS_WAITPID coming soon\n");
            break;
        case SYS_GETPID:
            printf("SYSCALL SYS_GETPID coming soon\n");
            break;
        case SYS_GETPPID:
            printf("SYSCALL SYS_GETPPID coming soon\n");
            break;
        case SYS_GETUID:
            printf("SYSCALL SYS_GETUID coming soon\n");
            break;
        case SYS_GETGID:
            printf("SYSCALL SYS_GETGID coming soon\n");
            break;
        case SYS_SETUID:
            printf("SYSCALL SYS_SETUID coming soon\n");
            break;
        case SYS_SETGID:
            printf("SYSCALL SYS_SETGID coming soon\n");
            break;
        case SYS_SLEEP:
            printf("SYSCALL SYS_SLEEP coming soon\n");
            break;
        
        case SYS_PRINT:
            printf("%s",param1);
            break;
        case SYS_GETCHAR:
            result = (int)get_char_block();
            // if(ehco_mode == true)
            // {
            //     printf("%s",result);
            // }
            break;
            // printf("\nResult: (%c)(%d)n",result,result);
        case SYS_SCREEN_CTRL:
            // printf("Clear screen X\n");
            if(param1 == 0)
            {
                cls_screen(VBE_RGB(0,0,0));
                clear_screen();
                printf("Clear screen\n");
            }
            if(param1 == 1)
            {
                parmss = (parameters *)param2;
                set_screen_x(parmss->param1);
                set_screen_y(parmss->param2);
            }
            break;
        case SYS_PUTCHAR:

            printf("%c",param1);
            // result = param1;
            break;
        case SYS_RM:
            remove(param1);
            break;
        case SYS_TELL:
            fp = (FILE *)param1;
            result = fl_ftell(fp);
        case SYS_FTELL:
            fp = (FILE *)param1;
            result = fl_ftell(fp);
            break;
        
        case SYS_LIST_DIR:
            
           

            // Call fl_listdirectory with the path and the buffers
            fl_listdirectory(param1, dirs, files, &dir_count, &file_count);
            break;
        case SYS_IS_DIR:
            result = fl_is_dir(param1);
            printf("PATH: %s == %d\n", param2,result);
            // result = -909;
            break;
        case SYS_IS_PATH:
            result = fl_is_dir(param1);
            break;
        case SYS_CREATE:
            fp = fopen(param1, "a");
            fclose(fp);
            break;
        case SYS_TERMINAL:
            if(param1 == 0)
            {
                terminal_struct *term = (terminal_struct *)param2;
                
                vbe_putpixel(term->x,term->y,term->color);
            }
            else if(param1 == 1)
            {
                terminal_struct *term = (terminal_struct *)param2;
                set_screen_x(term->x);
                set_screen_y(term->y);
            }
            else if(param1 == 2)
            {
                AthenXTerminal *term = (AthenXTerminal *)param2;
                if(term->auto_scroll  == true)
                {
                    set_scroll_mode(0);
                }
                else if(term->auto_scroll == false)
                {
                    set_scroll_mode(2);
                }
            }
            break;
        case SYS_GET_SCAN:
            result = (int)kb_get_scancode();
            break;
        case SYS_CALLOC:
            return kcalloc(param1, param2);
            break;
        case SYS_REALLOC:
            result = krealloc((void *)param1, (size_t)param2);
            if(result == NULL)
            {
                printf("realloc failed\n");
            }
            break;
        case SYS_LAZY_MAN:
            man_main((char *)param1);
            set_scroll_mode(1);
            break;
        case SYS_FWRITE:
            fp = (FILE *)param1;
            printf("%s  %s\n", fp->path,fp->filename);
            struct file_io *io_write = (struct file_io *)param2;        
            if(strcmp(fp->filename,"stdio") == 0 && strcmp(fp->path,"/dev") == 0)
            {
                // printf("here");
                printf("%s\n",io_write->ptr);
            }
            else
            {
                result = fl_fwrite(io_write->ptr,io_write->size,io_write->count,fp);
            }
                
            
            break;
        case SYS_FREAD:
              fp = (FILE *)param1;
            
            struct file_io *io_read = (struct file_io *)param2;
            if(strcmp(fp->filename,"stdio") && strcmp(fp->path,"/dev") == 0)
            {
                result = 1;
                io_read->ptr = get_char();

            }
            else
            {
                result = fl_fread(io_read->ptr, io_read->size, io_read->count, fp);
            }
            
            // printf("syscall output: \n%s",io_read->ptr);
            break;
        case SYS_FGETC:
            result =  fl_fgetc((FL_FILE *)param1);
            break;
        case SYS_FPUTC:
            result = fl_fputc((FL_FILE *)param1,param2);
            break;
        case SYS_FPUTS:
            result = fl_fputc(param1,(FL_FILE * )param2);
            break;
        case SYS_FGETS:
            s = (struct syscall *)param1;
            result = fl_fgets(s->parameter1,s->parameter2,(FL_FILE * )param2);
            break;
        case SYS_SET_FONT_C:
            term = (terminal_struct *)param1;
            set_font_c32(term->color);
            break;
        case SYS_SET_BG_COLOR:
            term = (terminal_struct *)param1;
            // set_font_c32(;
            set_bg_color32(term->color);
            break;
        case SYS_SET_X:
            // printf("set-x =%d\n",param1);
            set_vesa_colum(param1);
            break;
        case SYS_SET_Y:
            set_vesa_row(param1);
            // printf("set-t =%d\n",param1);
            break;
        case SYS_GET_X:
           
            result = get_screen_x();
            // result = ;
            // printf("Size=%d c=%d",sizeof(unsigned int),sizeof(char));
            // printf("&syscall_hander = %d\n",&sys_handler);
            // printf("&system_call_handler_c = 0x%x\n",&system_call_handler_c);
            if(sizeof(result) > 4)
            {
                printf("SIZE ERROR");
            }
            // printf("x=%d    ",result);
            break;
        case SYS_GET_Y:
            result = get_screen_y();
            // printf("y=%d    ",result);
            break;
        default:
            printf("Unknown syscall number\n");
            break;
    }
    pic8259_eoi(42);
    // printf("returning");
    return result;
}

int syscall_init()
{
    memset(memory_alloated, 0, sizeof(memory_alloated));
    memset(files_opened, 0, sizeof(files_opened));
}
int syscall_destroy()
{
    memset(memory_alloated, 0, sizeof(memory_alloated));
    memset(files_opened, 0, sizeof(files_opened));
}