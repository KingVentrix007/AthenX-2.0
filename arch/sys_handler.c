#include "../include/vesa.h"
#include "../include/isr.h"
#include "../include/printf.h"
#include "../include/keyboard.h"
#include "../include/syscall.h"
#include "../include/fat_filelib.h"
void handle_print_system_call(REGISTERS *reg) {
    // Assuming the message is stored in reg->eax
    const char *message = (const char *)reg->ebx;

    // Display the message (you need to implement the display function)
    printf(message);
    // const char *message2 = (const char *)reg->ebx;
    // printf(message2);
}


int handle_fwrite_system_call(const char *filename, const char *buffer) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        // Error opening the file for writing
        return 1;
    }

    // Write the contents of the buffer to the file
    if (fputs(buffer, file) == EOF) {
        // Error writing to the file
        fclose(file);
        return 2;
    }

    // Close the file after writing
    fclose(file);

    // Reopen the file for reading
    file = fopen(filename, "r");
    if (file == NULL) {
        // Error opening the file for reading
        return 3;
    }

    char readBuffer[1024]; // Assuming a maximum line length of 1024 characters
    if (fgets(readBuffer, sizeof(readBuffer), file) == NULL) {
        // Error reading from the file
        fclose(file);
        return 4;
    }

    // Close the file after reading
    fclose(file);

    // Compare the read data with the original buffer
    if (strcmp(buffer, readBuffer) == 0) {
        return 0; // Data matches
    } else {
        return -1; // Data doesn't match
    }
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


// #define SYSCALL_EXIT 0x80
// #define SYSCALL_PRINT 0x81
// #define SYSCALL_TEST_RET 0x83
int system_call_handler_c(int syscall_number, int param1, int param2) {
    int result = 0;
     FILE *fp;
     parameters *parmss;
    //  printf("SYSCALL %d -> \n", syscall_number);
    // printf("\nsyscall_number: %d\n", syscall_number);
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
            result = handle_fwrite_system_call((char *)param1,(char *) param2);
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
            printf("SYSCALL SYS_LSEEK coming soon\n");
            break;
        case SYS_MMAP:
            printf("SYSCALL SYS_MMAP coming soon\n");
            break;
        case SYS_MUNMAP:
            printf("SYSCALL SYS_MUNMAP coming soon\n");
            break;
        case SYS_DUP:
            printf("SYSCALL SYS_DUP coming soon\n");
            break;
        case SYS_DUP2:
            printf("SYSCALL SYS_DUP2 coming soon\n");
            break;
        case SYS_EXECVE:
            printf("SYSCALL SYS_EXECVE coming soon\n");
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
            break;
            // printf("\nResult: (%c)(%d)n",result,result);
        case SYS_SCREEN_CTRL:
            printf("Clear screen X\n");
            if(param1 == 0)
            {
                cls_screen(VBE_RGB(0,0,0));
                clear_screen();
                printf("Clear screen\n");
            }
            break;
        default:
            printf("Unknown syscall number\n");
            break;
    }
    pic8259_eoi(42);
    // printf("returning");
    return result;
}