#include "termios.h"
#include "../../include/syscall.h"
int tcgetattr(int fd, struct termios *termios_p) {
    // Your custom implementation of tcgetattr
    // Implement the functionality you need here.
    return 0;  // Return success for simplicity, replace with actual implementation.
}

int set_term_attr(AthenXTerminal term)
{
    syscall(SYS_TERMINAL,2,&term);
}