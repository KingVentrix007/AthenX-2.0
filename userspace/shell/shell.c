#define ATHENX_LIBC
#include "stdio.h"
#include "maths.h"
#include "string.h"
#include "stdlib.h"
#include "terminal.h"
#include "shell.h" 
#include "termios.h"
#define MAX_COMMAND_LENGTH 50
#define MAX_ARGUMENTS 10
void Sappend(char *buf, char c) {
    int len = strlen(buf);
    if (len < 1000) {  // To prevent buffer overflow
        // printf("C%c", c);
        buf[len] = c;
        // printf("B%c|", buf[len]);
        buf[len + 1] = '\0';  
    }
}
char cwd[100] = "/";  
int main(int argc, char **argv)
{
    strcpy(cwd, argv[1]);
    printf("Welcome to the shell!\n");
    printf("\nShell>");
    // set_pos(100,100);
    // printf("Press any key to continue\n"); 
    // char buf[1001];
    char *buf = (char*)malloc(1001);
    memset(buf, 0, sizeof(buf));
    buf[0] = '\0';
    
    while(1)
    {
        // printf("HERE>");
        char c = get_char();
        if (c == '\n') {
            // printf("\nBUFFER %s\n", buf);
            printf("\n");
            if (shell(buf) == -567) {
                free(buf);
                return 0;
            }
            buf[0] = '\0';
            printf("\nshell>");
        }
        else if (c == '\0')
        {
            /* code */
        }
        else if(c == '\b')
            {
                //crude_song();
                if(backspace(buf))
                {
                    printf("\b");
                    
                    //set_cursor_x(get_cursor_x()-2);
                    //printf_(" ");
                    //console_ungetchar();
                }
                else
                {
                    ///beep();
                }
                
                //printf_("\b");
            }
        else
        {
            printf("%c",c);
            Sappend(buf, c);
        }
        // printf("hello\n");
    }
}




int shell(char buf[1001]) {


    char command_copy[1001];
    strcpy(command_copy, buf);

    // Tokenize the command by spaces
    char* token;
     char* saveptr;
    char* arguments[MAX_ARGUMENTS];
    char* sub_args[MAX_ARGUMENTS];
    int sub_arg_count = 0;
    int arg_count = 0;
    int in_quote = 0;
    token = strtok(command_copy, " ");
   while (token != NULL) {
        if (arg_count >= MAX_ARGUMENTS) {
            printf("Too many arguments!\n");
            break;
        }

        // Check if it's an argument or sub-argument
        if (token[0] == '-' && token[1] == '-') {
            sub_args[sub_arg_count++] = token;
        } else {
            arguments[arg_count++] = token;
        }

        token = strtok(NULL, " ");
    }
    if(strcmp(arguments[0],"echo") == 0)
    {
        printf("%s\n", arguments[1]);
    } 
    else if (strcmp(arguments[0],"cat") == 0)
    {
        AthenXTerminal term;
        term.auto_scroll = false;
        set_term_attr(term);
        
        FILE *file = fopen(arguments[1], "r");
        if (file == NULL) {
          printf("Error opening file %s\n",arguments[1]); 
          return -1;
        }

    int allocv = get_file_size_from_pointer(file);
    printf("\nAlocating %d bytes of memory for file\n",allocv);
    char *data = malloc(allocv);
    if(data == NULL)
    { 
        printf("Error creating data\n");
        return -1;
    }
    size_t bytes_read = fread(data, sizeof(char),9000, file);
    printf("file size: %d\n", bytes_read);
    if (bytes_read > 0) {
        data[bytes_read] = '\0'; // Null-terminate  the data.

        // Print the data to the console. 
        printf("data ->%s\n", data);

        fclose(file);
        term.auto_scroll = true; //
        set_term_attr(term); //
        free(data);
        return 0; // Success
    }
     term.auto_scroll = true; //
        set_term_attr(term); //
    fclose(file);
    return -1;
    }
    else if (strcmp(arguments[0],"cls") == 0)
    {
        // printf("clear\n");
        clear_screen();
    }
    else if (strcmp(arguments[0],"ls") == 0)
    {
        ls(cwd); 
    }
    else if (strcmp(arguments[0],"test") == 0)
    {
        char *fake = malloc(100);
        // FILE *file = fopen("/root/cmd.c", "r");
        // if (file == NULL) {
        //     printf("File not found.\n");
        //     return 1;
        // }

        // int intValue;
        // char strValue[256];

        // int itemsRead = fscanf(file, "Test %d String %s", &intValue, strValue);

        // if (itemsRead == 2) {
        //     printf("Read values: %d, %s\n", intValue, strValue);
        // } else {
        //     printf("Failed to read values.\n");
        // }

        // fclose(file);
    }
    
    else if (strcmp(arguments[0],"rm") == 0)
    {
        char tmp[1001];
        strcpy(tmp,cwd);
        strcat(tmp,"/");
        strcat(tmp,arguments[1]);
        rm_file(tmp);
    }
    
    else if (strcmp(arguments[0],"exit") == 0)
    {
        return -567;
    }
     
    else if(strcmp(arguments[0],"cd") == 0)
    {
        if (arg_count >= 2) {
    if (strcmp(arguments[1], "..") == 0) {
        // Check if the current directory is the rootfs directory
        if (strcmp(cwd, "root") == 0 || strcmp(cwd, "/root/") == 0) {
            // Allow going one level above rootfs
            strcpy(cwd, "/");
        } else if (strstr(cwd, "/") != 0) {
            // Find the last '/' character in the current directory path
            char* last_slash = strrchr(cwd, '/');
            
            if (last_slash != NULL) {
                //printf("Here\n");
                // Remove the last directory from the current path
                *last_slash = '\0'; 
                char* last_slash = strrchr(cwd, '/');
            
                if (last_slash != NULL) {
                    //printf("Here23\n");
                    // Remove the last directory from the current path
                    *last_slash = '\0';
                    
                    }
            } else {
                // If there is no '/', set the current directory to rootfs
                strcpy(cwd, "root");
            }
        }
        if(cwd[strlen(cwd)-1] == '/' && cwd[strlen(cwd)-2] == '/')
        {
            cwd[strlen(cwd)-1] = '\0';
        } 
        if(cwd[strlen(cwd)-1] != '/')
        {
            strcat(cwd,"/");
        }
    } else { 
        char tmp[260];
        strcpy(tmp,cwd);
        if(tmp[strlen(tmp)-1] != '/')
        {
             strcat(tmp, "/");
        }
       
        strcat(tmp, arguments[1]);
        
        if (fl_is_dir(tmp) == 1) {
            strcpy(cwd, tmp);
        } else {
            printf("\n%s is not a valid path\n", tmp);
        }
        if(cwd[strlen(cwd)-1] == '/' && cwd[strlen(cwd)-2] == '/')
        {
            // DEBUG("");
            cwd[strlen(cwd)-1] = '\0';
        }
        if(cwd[strlen(cwd)-1] != '/')
        {
            strcat(cwd,"/");
        }
    }
}
    }else if (strcmp(arguments[0],"pwd") == 0)
    {
        printf("%s\n",cwd);
    }
    else if(strcmp(arguments[0],"man") == 0)
    { 
        syscall(37,arguments[1],0);
    }
    else if (strcmp(arguments[0],"touch") == 0)
    { 
        char *tmp = cwd;
        strcat(tmp,"/");
        strcat(tmp,arguments[1]);
        
        create_file(tmp);
    }
    else if (strcmp(arguments[0],"term" ) == 0)
    {
        
        int x = get_x();
        // set_x(x+900);
        printf("food"); 
        // int y = get_y();
        // printf("\ny=%d\n",y); 
        // return 0;
    }
    else if (strcmp("write",arguments[0]) == 0) 
    {
        FILE *f = fopen("/var/fake.txt", "w"); 
        fprintf(f,"%s\n",arguments[1]);
        fclose(f);
        FILE *q = fopen("/var/fake.txt", "r"); 
        char buf[1024] = {0};
        fread(buf,1024,sizeof(buf),q);
        printf("read %s from %s\n",buf,"/var/fake.txt");
        fclose(q);
    }
    
    
    
 

















    else
    {
        
        printf("Invalid command %s pls try again\n", arguments[0]);
        // printf("ending command ");
    }
    // printf("end\n");
    // return -1;
    
    // char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];
    // memset(tokens, 0, sizeof(tokens));
    // int tokenCount = tokenizeCommand(buf, tokens);
    // printf("tokenCount = %d\n", tokenCount);
    // if (tokenCount >= 100) {
    //     return 0;
    // }
    // for (int i = 0; i < tokenCount; i++) {
    //     printf("Token %d: %s\n", i, tokens[i]);
    // }
    // if(strstr(tokens[0],"echo") != NULL)
    // {
    //     printf("\necho %s\n", tokens[1]);
    // }
    // else
    // {
    //     printf("%s is not a valid command\n", tokens[0]);
    // }
}


int tokenizeCommand(const char* buffer, char tokens[][MAX_TOKEN_LENGTH]) {
    int tokenCount = 0;
    int insideQuotes = 0;
    char currentToken[MAX_TOKEN_LENGTH] = "";
    int tokenIndex = 0;

    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ' ' && !insideQuotes) {
            if (strlen(currentToken) > 0) {
                strcpy(tokens[tokenIndex], currentToken);
                tokenCount++;
                tokenIndex = 0;
                memset(currentToken, 0, sizeof(currentToken));
            }
        } else if (buffer[i] == '"') {
            insideQuotes = !insideQuotes;
        } else {
            currentToken[tokenIndex] = buffer[i];
            tokenIndex++;
        }
    }

    if (strlen(currentToken) > 0) {
        strcpy(tokens[tokenCount], currentToken);
        tokenCount++;
    }

    return tokenCount;
}