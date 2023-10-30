#include "ctype.h"
#include "stdbool.h" // Include the stdbool.h header for the "bool" type.

bool isprint(int c) {
    // Check if the character is within the ASCII printable range.
    return (c >= 32 && c <= 126);
}