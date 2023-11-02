#include "stdio.h"
#include "byte.h"
// Function to read a bytecode instruction from a file
void readInstruction(FILE* bytecodeFile, int* opcode, int* operand) {
    fread(opcode, sizeof(int), 1, bytecodeFile);
    fread(operand, sizeof(int), 1, bytecodeFile);
}

// Function to check if a bytecode file is valid
int isBytecodeFileValid(FILE* bytecodeFile) {
    // Verify the header (customize as needed)
    const char* expectedHeader = "ATHENX_BYTECODE";
    char headerBuffer[strlen(expectedHeader) + 1];
    fread(headerBuffer, sizeof(char), strlen(expectedHeader), bytecodeFile);
    headerBuffer[strlen(expectedHeader)] = '\0';

    return strcmp(headerBuffer, expectedHeader) == 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <bytecode_file>\n", argv[0]);
        return 1;
    }

    char* bytecodeFilename = argv[1];

    // Open the bytecode file for reading
    FILE* bytecodeFile = fopen(bytecodeFilename, "rb");
    if (!bytecodeFile) {
        printf("Failed to open the bytecode file: %s\n", bytecodeFilename);
        return 1;
    }

    // Check if the bytecode file is valid
    if (!isBytecodeFileValid(bytecodeFile)) {
        printf("Invalid bytecode file format.\n");
        fclose(bytecodeFile);
        return 1;
    }

    int stack[100];
    int stackPtr = 0;
    int variables[100];
    int varCount = 0;

    int opcode, operand;

    while (1) {
        readInstruction(bytecodeFile, &opcode, &operand);
        printf("opcode = %d\n",opcode);
        switch (opcode) {
            case PUSH_STRING: {
                char* str = (char*)operand;
                stack[stackPtr++] = (int)str;
                break;
            }
            case PRINT: {
                char *str = (char*)stack[--stackPtr];
                printf("out%s\n", str);
                break;
            }
            case VAR_INT: {
                variables[varCount++] = 0;
                break;
            }
            case READ_INT: {
                int value;
                printf("Enter an integer: ");
                scanf("%d", &value);
                stack[stackPtr++] = value;
                break;
            }
            case PUSH_VAR: {
                int varIndex = operand;
                if (varIndex >= 0 && varIndex < varCount) {
                    stack[stackPtr++] = variables[varIndex];
                } else {
                    fprintf(stderr, "Invalid variable index: %d\n", varIndex);
                }
                break;
            }
            case JUMP: {
                // Not implemented in this simple interpreter
                fprintf(stderr, "JUMP instruction not supported.\n");
                break;
            }
            case HALT:
                // End of the program
                fclose(bytecodeFile);
                return 0;
            case START:
                printf("Starting execution\n");
                break;
            default:
                fprintf(stderr, "Invalid opcode: %d\n", opcode);
                break;
        }
    }
}