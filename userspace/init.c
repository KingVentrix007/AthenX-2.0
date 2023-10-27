#include "stdlib.h"
#include "stdio.h"
int _start(int argc, char argv[20][100])
{
    init_stdio();
    return exit(main(argc, argv)); 
}