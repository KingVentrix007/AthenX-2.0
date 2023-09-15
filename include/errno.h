#ifndef ERRNO_H
#define ERRNO_H
#include "timer.h"
#include "stdbool.h"

typedef char *(*Decode_Function)(uint16_t);
typedef struct 
{
    int exit_code;
    int sub_code;
     char function[100];
    char code_str[30];
    TIME event_time;
    bool is_error;
}event;

typedef struct
{
    int code;
    int sub_code;
    char function[100];
    char code_str[30];
    char sub_code_str[30];
    int m;
    int h;
    int s;
    TIME time;
    Decode_Function df;
}error;

typedef struct
{
    error *error;
    int num_errors;
}error_list;




// Define error codes in hexadecimal format
#define SUCCESS           0x00
#define GENERAL_ERROR     0x01
#define FILE_NOT_FOUND    0x02
#define ACCESS_DENIED     0x03
#define INVALID_COMMAND   0x04
#define OUT_OF_MEMORY     0x05
#define DISK_FULL         0x06
#define INVALID_FILESYSTEM 0x07
#define IP_TRACE_ERROR    0x08
#define GUI_INIT_ERROR    0x09
#define PCI_ERROR         0x0A
#define ETHERNET_ERROR    0x0B
#define EXT2_ERROR        0x0C
#define PACKET_SEND_ERROR 0x0D
#define CUSTOM_ERROR2     0x0E
#define CUSTOM_ERROR3     0x0F
#define CUSTOM_ERROR4     0x10
#define CUSTOM_ERROR5     0x11
#define CUSTOM_ERROR6     0x12
#define CUSTOM_ERROR7     0x13
#define CUSTOM_ERROR8     0x14
#define CUSTOM_ERROR9     0x15
#define CUSTOM_ERROR10    0x16

// Function to decode error code meanings
const char* decode_error(int error_code);

// exit function
int exit(int code);
//Better exit function

int main_exit(int exit_code,int subcode, char function[100],bool text,Decode_Function func);

// inspect all errors;
void inspect_errors();

//end init messages
int init_complete();

void logEvent(const event* newEvent);

int compareTime(const TIME* time1, const TIME* time2);

void filterAndPrintEvents(const TIME* startTime, const TIME* endTime, bool isError,const char* functionName);

void print_events();
#endif // ERRNO_H
