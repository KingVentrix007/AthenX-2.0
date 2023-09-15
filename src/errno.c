#include "errno.h"
#include "printf.h"
#include "display.h"
#include "stdbool.h"
#include "timer.h"
//list of errors;
error_list errors;
bool init_message = true;
const char* decode_error(int error_code) {
    switch (error_code) {
        case SUCCESS:
            return "Success";
        case GENERAL_ERROR:
            return "General error";
        case FILE_NOT_FOUND:
            return "File not found";
        case ACCESS_DENIED:
            return "Access denied";
        case INVALID_COMMAND:
            return "Invalid command";
        case OUT_OF_MEMORY:
            return "Out of memory";
        case DISK_FULL:
            return "Disk full";
        case INVALID_FILESYSTEM:
            return "Invalid filesystem";
        case IP_TRACE_ERROR:
            return "IP trace error";
        case GUI_INIT_ERROR:
            return "GUI initialization error";
        case PCI_ERROR:
            return "PCI error";
        case ETHERNET_ERROR:
            return "Ethernet error";
        case EXT2_ERROR:
            return "EXT2 filesystem error";
        case PACKET_SEND_ERROR:
            return "Packet send error";
        case CUSTOM_ERROR2:
            return "Custom error 2";
        case CUSTOM_ERROR3:
            return "Custom error 3";
        case CUSTOM_ERROR4:
            return "Custom error 4";
        case CUSTOM_ERROR5:
            return "Custom error 5";
        case CUSTOM_ERROR6:
            return "Custom error 6";
        case CUSTOM_ERROR7:
            return "Custom error 7";
        case CUSTOM_ERROR8:
            return "Custom error 8";
        case CUSTOM_ERROR9:
            return "Custom error 9";
        case CUSTOM_ERROR10:
            return "Custom error 10";
        default:
            return "Unknown error";
    }
}


int init_errors()
{
    errors.num_errors = 0;
    //errors.error[errors.num_errors];
}


int exit(int code)
{
    if(code != SUCCESS)
    {
        printf_("{/330:255,0,0}");
        printf(decode_error(code));
        printf("\n");
        printf_("{/330:0,255,0}");
    }
    return code;
}

int main_exit(int exit_code,int subcode, char function[100],bool text,Decode_Function func)
{
    // printf("exit_code: 0x%x",exit_code);
    event event;
    if(exit_code != SUCCESS)
    {
        errors.error[errors.num_errors].code = exit_code;
        strcpy(errors.error[errors.num_errors].function,function);
        errors.error[errors.num_errors].sub_code = subcode;
        strcpy(errors.error[errors.num_errors].code_str,decode_error(exit_code));
        errors.error[errors.num_errors].time = get_time();
        errors.error[errors.num_errors].df = func;
        strcpy(event.code_str,errors.error[errors.num_errors].code_str);
        event.event_time = errors.error[errors.num_errors].time;
        event.exit_code = exit_code;
        event.sub_code = subcode;
        strcpy(event.function,function);
        event.is_error = true;
        //errors.num_errors = errors.num_errors +1; 
        // TIME time = get_time();
        // errors.error[errors.num_errors].h = time.h;
        //  errors.error[errors.num_errors].m = time.m;
        //   errors.error[errors.num_errors].s = time.s;
        //printf("time: %d/%d/%d\n",day,month,year);
        //printf("time: %d.%d:%d\n",errors.error[errors.num_errors].h,errors.error[errors.num_errors].m,errors.error[errors.num_errors].s);
        //printf("Number of errors: %d\n",errors.num_errors);
        errors.num_errors = errors.num_errors +1;
        
        if (text == true)
        {
                printf_("{/330:255,0,0}");
                 TIME time = get_time();
                printf("[%02d:%02d:%02d] ERROR: Function %s exited with error 0x%04x (Subcode 0x%04X)",time.h,time.m,time.s,function,exit_code,subcode);
                printf("\n");
                printf_("{/330:0,255,0}");
                get_time();
        }
        
    }
    else if (exit_code == SUCCESS && (text==true || init_message == true))
    {
        //printf("\nHERE\n");
        printf_("{/330:0,0,255}");
        TIME time = get_time();
        strcpy(event.code_str,decode_error(exit_code));
        event.event_time = time;
        event.exit_code = exit_code;
        event.sub_code = subcode;
        strcpy(event.function,function);
        event.is_error = false;
        //errors.num_errors = e
        printf("[%02d:%02d:%02d]:Function %s exited with error 0x%x, subcode 0x%04x",time.h,time.m,time.s,function,exit_code,subcode);
        printf("\n");
        printf_("{/330:0,255,0}");
        get_time();
        
    }
    
    logEvent(&event);
    return exit_code;
    
      
   
    
    

}

void inspect_errors()
{
    if(errors.num_errors <= 1)
    {
        printf("\nThere has been %d error since boot",errors.num_errors);
    }
    else
    {
        printf("\nThere have been %d errors since boot",errors.num_errors);
    }
    
    if(errors.num_errors >= 1)
    {
        printf("\nErrors:\n");
        for (size_t i = 0; i < errors.num_errors; i++)
        {
            //printf("Error %d\n",i);
            error er = errors.error[i];
            //er.df(0x0019);
           
            printf("\nError code: 0x%04x --> %s\n",er.code,er.code_str);
            printf("Sub code: 0x%04x\n",er.sub_code);
            printf("Function: %s\n",er.function);
            printf("Time of error: %02d:%02d:%02d\n",er.time.h,er.time.m,er.time.s);
            
        }
        
    }
}

int init_complete()
{
    init_message = false;
}

#define MAX_EVENTS 100
event eventLog[MAX_EVENTS];

// Global variable to keep track of the number of events logged
int eventCount = 0;

// Function to log an event and store it in the array
void logEvent(const event* newEvent) {
    if (eventCount >= MAX_EVENTS) {
        printf("Event log is full. Cannot log more events.\n");
        return;
    }

    // Copy the new event to the event log
    eventLog[eventCount++] = *newEvent;
}
int compareTime(const TIME* time1, const TIME* time2) {
    if (time1->h != time2->h) return time1->h - time2->h;
    if (time1->m != time2->m) return time1->m - time2->m;
    return time1->s - time2->s;
}
void filterAndPrintEvents(const TIME* startTime, const TIME* endTime, bool isError, const char* functionName) {
    printf("Filtered Events:\n");
    for (int i = 0; i < eventCount; i++) {
        event currentEvent = eventLog[i];
        printf("%d\n",startTime->h);
         printf("%d\n",endTime->h);
        // Check the criteria for filteringinspe
        if ((startTime->h <= currentEvent.event_time.h ) && (endTime->h >= currentEvent.event_time.h)){
            // Print the event that matches the criteria
            printf("Event %d:\n", i + 1);
            printf("Exit Code: %d\n", currentEvent.exit_code);
            printf("Sub Code: %d\n", currentEvent.sub_code);
            printf("Function: %s\n", currentEvent.function);
            printf("Code String: %s\n", currentEvent.code_str);
            printf("Timestamp: %02d:%02d:%02d %04d-%02d-%02d\n",
                   currentEvent.event_time.h, currentEvent.event_time.m, currentEvent.event_time.s,
                   currentEvent.event_time.y, currentEvent.event_time.mo, currentEvent.event_time.d);
            printf("Is Error: %s\n", currentEvent.is_error ? "true" : "false");
            printf("\n");
        }
    }
}
void print_events()
{
    for (int i = 0; i < eventCount; i++) {
        event currentEvent = eventLog[i];
         printf("Event %d:\n", i + 1);
        printf("Exit Code: 0x%x\n", currentEvent.exit_code);
        printf("Sub Code: 0x%04x\n", currentEvent.sub_code);
        printf("Function: %s\n", currentEvent.function);
        printf("Code String: %s\n", currentEvent.code_str);
        printf("Timestamp: %02d:%02d:%02d %02d/%02d/%02d\n",
                currentEvent.event_time.h, currentEvent.event_time.m, currentEvent.event_time.s,
                currentEvent.event_time.d, currentEvent.event_time.mo,currentEvent.event_time.y);
        if(currentEvent.is_error == true) printf_("{/330:255,0,0}");
        printf("Is Error: %s\n", currentEvent.is_error ? "true" : "false");
        if(currentEvent.is_error == true) printf_("{/330:0,255,0}");
        printf("\n");
        }
}
