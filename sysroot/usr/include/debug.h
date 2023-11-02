#ifndef DEBUG_H
#define DEBUG_H
#include "types.h"
#include "stdbool.h"
#include "isr.h"
#define MAX_LIST_SIZE 600
#define MAX_FUNCTIONS 10000
typedef struct
{
    char names[200];
    uint32 addr;
    uint32 addr_end;
    char file_name[200];
    uint32 line;
    int num_params;
    char addrtype[10];
    uint32_t esp;
    uintptr_t stack_ptr;
    int esp_in;
}ADDER_NAME_LIST;
typedef struct {
    ADDER_NAME_LIST data;
} Node;

typedef struct {
    Node nodes[MAX_LIST_SIZE];
    int size;
} List;
struct Function {
    char name[100]; // Assuming function names won't exceed 99 characters
    uint64_t address;
};


/*
 * Similar to PRINT, but output is enhanced by some
 * debugging information
 */
#define DEBUG(...) do {printf("\nHERE at %s@%d (%s): ", __FILE__, __LINE__, __FUNCTION__); \
                     printf("%s\n\n",__VA_ARGS__); } while (0)
#define LOG() {printf("\nLOGED at %s@%d (%s): ", __FILE__, __LINE__, __FUNCTION__);} while (0)

#define FUNC_ADDR_NAME(addr,num_params,param_list) do {append_name_addr(__FUNCTION__,addr,num_params,param_list,__LINE__-1,__FILE__);} while(0)
int screen_of_death();
void get_name_addr(ADDER_NAME_LIST *out);
void append_name_addr(char name[20],uint32 addr,int num_parms,char param_list[10],uint32 line,char filename[20]);
void get_name_adder_list(List *out);
bool find_by_address(uint32_t* targetAddr);
ADDER_NAME_LIST *print_list(uintptr_t num);
double check_similarity(double num1, double num2);
void print_stack_trace(void *ebp);
double check_similarity(double num1, double num2);
void debug_terminal(REGISTERS *regs);
int kassert(int ret,int expected_ret,int level);
const ADDER_NAME_LIST *find_largest_smaller_address(const List *list, uint32 search_addr);
void getRegisters(REGISTERS* regs);
int get_adder_map(char output[71629]);
char* findNextSpaceOrNewline(char* ptr);
int parseMemoryMap(const char* mapData, struct Function* functions);
unsigned long long parseHex(const char* str);
const struct Function* resolve_function_name(uintptr_t address, const struct Function* functions, int numFunctions);

// #define assert(condition) ((void)0)
#endif