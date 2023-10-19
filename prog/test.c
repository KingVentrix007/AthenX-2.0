// hello.c
#include "../libc/lib_stdio.h"
#include "../include/syscall.h"
int main() {
    // printf("hello world\n");
    // printf("I was loaded from memory and executed");
    // int c = test_ret(6060);
    // printf("Got this char using get_char system call(0x82) -> %d\n", c);
    const char* message = "Hello, AthenX!";
    int result = syscall(0x83, (int)message, 0);
    printf("%d\n", result);
    // int c = test_ret(6060);
    // printf("ret value: %d\n",c);
    // int rev = reverse_int(c);
    // printf("reverse ret value: %d\n",rev);

    exit();
    // return 0;
}
