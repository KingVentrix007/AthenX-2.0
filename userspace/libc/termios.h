#ifndef __TERMIOS__H
#define __TERMIOS__H
#include "syscall.h"
typedef unsigned int tcflag_t;
typedef unsigned char cc_t;
#define NCCS 20
#define TCSAFLUSH 2
#define BRKINT 2
#define ICRNL 2
#define INPCK 2
#define ISTRIP 2
#define IXON 2
#define OPOST 2
#define CS8 2
#define ECHO 2
#define ICANON 2
#define IEXTEN 2
#define ISIG 2
#define VMIN 2
#define VTIME 2
#define STDIN_FILENO 0
#define STDOUT_FILENO 1


// Structs for termios.h
struct termios {
    tcflag_t c_iflag;   // Input modes
    tcflag_t c_oflag;   // Output modes
    tcflag_t c_cflag;   // Control modes
    tcflag_t c_lflag;   // Local modes
    cc_t c_cc[NCCS];    // Control characters
};
int tcgetattr(int fd, struct termios *termios_p);
int set_term_attr(AthenXTerminal term);
#endif