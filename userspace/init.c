#include "stdlib.h"
#include "stdio.h"
extern int main(int argc, char **argv);
int _start(int argc, char **argv)
{
    init_libc();
    return exit(main(argc, argv)); 
}