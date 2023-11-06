#include "unistd.h"
#include "stdio.h"



int access(const char *path, int mode)
{   
    int ret;
    switch (mode)
    {
    case F_OK:
        ret = fl_is_dir(path);
        if(ret != 1)
        {
            FILE *fp = fopen(path,"r");
            if(fp == NULL)
            {
                ret = -1;
            }
            else
            {
                ret = 0;
            }
        }
        else
        {
            ret = 0;
        }
        return ret;
        break;
    // FILE
    default:
        break;
    }
}