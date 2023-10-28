#include "stdlib.h"
#include "stdio.h"
extern int main(int argc, char argv[20][100]);
int _start(int argc, char argv[20][100])
{
    init_libc();
    return exit(main(argc, argv)); 
}