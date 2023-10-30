// hello.c
#include "stdio.h"
#include "string.h"
//  #include "syscall.h"

int main() {
    printf("BASIC KEYBOARD\n");
    char buffer[1001];
    FILE *fp = fopen("/var/env.txt","r");
    if(fp == NULL)
    {
        printf("FP error\n");
        return -1;
    }
    
    char test[2000];
    
    // fread(test,fp,sizeof(test));
    char line[1001]; // Adjust the buffer size as needed
    fread(line, fp,sizeof(line));
        // while () {
    printf("%s", line);
    // FILE *fpp = fopen("/var/CAR.txt","w");
    // if(fpp == NULL)
    // {
    //     printf("Fpp not found");
    //     return -2;
    // }
    char write[100] = "CATS EAT FOOD";
    char path[100] = "/var/CAR.txt";
    int r = fwrite(path,write);
    if(r == 0)
    {
        printf("Writing %s to %s was successful",write,path);
    }
    else
    {
        printf("Failed to write %s to %s\n",write,path);
        return -1;
    }
    //syscall(SYS_CLOSE, fpp,0);
    // fread(line, fp,sizeof(line));
        // while () {
    // printf("\n%s", line);
        // }
    // buffer = (char *)resutl;
    // fread(test,fp,sizeof(test));
    // printf("\nTEST->%s\n",test);
    // printf("\nBUFFER->%s\n",buffer);
    // while(1)
    // {
    //      char c = get_char();
   
    //     if(c != '\n' && c != '\0' && (c >= 32 && c <= 126))
    //     {
    //         append(buffer, c);
            
    //         // {
    //         //     printf("q");
    //         // }
    //         // printf("HOLA\n");
    //         // printf("\nTest->(%c)(%d)",c, c);
    //         printf("%c",c);
    //     }
    //     else if (c == '\n')
    //     {
    //         printf("\n");
    //     }
        
    // }
    // printf("\n%s", buffer);
   
    // printf("hello world\n");
    // printf("I was loaded from memory and executed");
    // int c = test_ret(6060);
    // printf("Got this char using get_char system call(0x82) -> %d\n", c);
    // const char* message = "Hello, AthenX!";
    // int result = syscall(0x83, (int)message, 0);
    // printf("%d\n", result);
    // int c = test_ret(6060);
    // printf("ret value: %d\n",c);
    // int rev = reverse_int(c);
    // printf("reverse ret value: %d\n",rev);

    // exit();
    return 0;
}
