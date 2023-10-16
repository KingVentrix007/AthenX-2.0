// hello.c
#include "../libc/lib_stdio.h"
int main() {
    printf("hello world\n");
    printf("I was loaded from memory and executed");
    int c = test_ret(6060);
    printf("Got this char using get_char system call(0x82) -> %d\n", c);
    // int c = test_ret(6060);
    // printf("ret value: %d\n",c);
    // int rev = reverse_int(c);
    // printf("reverse ret value: %d\n",rev);


    return 0;
}
