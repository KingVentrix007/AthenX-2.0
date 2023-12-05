
/**
 * @file syscall.c
 * @brief Source file containing the implementation of the syscall function.
 * @author KingVentrix007
 */



/**
 * @brief Perform a system call with the given syscall number and parameters.
 *
 * This function performs a system call by using inline assembly to trigger an interrupt with the specified syscall number and parameters.
 * The result of the system call is stored in the 'result' variable and returned.
 *
 * @param syscall_number The number of the system call.
 * @param param1 The first parameter of the system call.
 * @param param2 The second parameter of the system call.
 * @return The result of the system call.
 */
int syscall(int syscall_number, int param1, int param2) {
    int result;
    asm volatile ( 
        "int $42"
        : "=a" (result)
        : "a" (syscall_number), "b" (param1), "c" (param2)
    );
    // if(syscall_number == SYS_GET_X)
    // {
    // }
    return result;
}