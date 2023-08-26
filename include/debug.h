#ifndef DEBUG_H
#define DEBUG_H
#include "types.h"
#include "stdbool.h"
#define MAX_LIST_SIZE 600
typedef struct
{
    char names[200];
    uint32 addr;
    uint32 addr_end;
    char file_name[200];
    uint32 line;
}ADDER_NAME_LIST;
typedef struct {
    ADDER_NAME_LIST data;
} Node;

typedef struct {
    Node nodes[MAX_LIST_SIZE];
    int size;
} List;

/*
 * Similar to PRINT, but output is enhanced by some
 * debugging information
 */
#define DEBUG(...) do {printf("\nHERE at %s@%d (%s): ", __FILE__, __LINE__, __FUNCTION__); \
                     printf("%s\n\n",__VA_ARGS__); } while (0)

#define FUNC_ADDR_NAME(...) do {append_name_addr(__FUNCTION__,__VA_ARGS__,__LINE__-1,__FILE__);} while(0)
int screen_of_death();
void get_name_addr(ADDER_NAME_LIST *out);
void append_name_addr(char name[20],uint32 addr,uint32 line,char filename[20]);
void get_name_adder_list(List *out);
bool find_by_address(uint32_t* targetAddr);
void print_list(uint32 num);
double check_similarity(double num1, double num2);
void print_stack_trace(void *ebp);
double check_similarity(double num1, double num2);
void debug_terminal();
int kassert(int ret,int expected_ret,int level);
#endif