#include "sim.h"
#include "stdio.h"
#include "string.h"



// Function to write a bytecode instruction to a file
void writeInstruction(FILE* bytecodeFile, int opcode, int operand) {
    fwrite(&opcode, sizeof(int), 1, bytecodeFile);
    fwrite(&operand, sizeof(int), 1, bytecodeFile);
}

// Function to parse and generate bytecode for a Fak source line
void parseAndGenerateBytecode(FILE* bytecodeFile, const char* line) {
    if (strstr(line, ".START") != NULL) {
        // Start of the program
        writeInstruction(bytecodeFile, START, 0);
    } else if (strstr(line, "print") != NULL) {
        // Print statement
        char* start = strchr(line, '"');
        char* end = strrchr(line, '"');
        if (start && end) {
            *end = '\0';
            printf("%s",start + 1);
            writeInstruction(bytecodeFile, PUSH_STRING, (int)(start + 1));
            writeInstruction(bytecodeFile, PRINT, 0);
        }
    } else if (strstr(line, "int") != NULL) {
        // Integer variable declaration
        // Implement logic to determine the variable name if needed
        writeInstruction(bytecodeFile, VAR_INT, 0);
    } else if (strchr(line, '=') != NULL) {
        // Variable assignment
        // Implement logic to extract the variable name if needed
        char* value = strchr(line, '=');
        if (value) {
            *value = '\0';  // Split the line at '='
            writeInstruction(bytecodeFile, READ_INT, 0);
            writeInstruction(bytecodeFile, PUSH_VAR, 0);
        }
    } else if (strstr(line, "input") != NULL) {
        // Read input statement
        // Implement logic to extract the variable name if needed
        writeInstruction(bytecodeFile, READ_INT, 0);
        // Implement logic to find the variable index based on the name
        int varIndex = 0;
        writeInstruction(bytecodeFile, PUSH_VAR, varIndex);
    } else if (strstr(line, ".END") != NULL) {
        // End of the program
        writeInstruction(bytecodeFile, HALT, 0);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <input_filename> [output_filename]\n", argv[0]);
        return 1;
    }

    char* inputFilename = argv[1];
    char* outputFilename = (argc == 3) ? argv[2] : NULL;

    // Open the Fak source file for reading
    FILE* fakSourceFile = fopen(inputFilename, "r");
    if (!fakSourceFile) {
        fprintf(stderr, "Failed to open the Fak source file: %s\n", inputFilename);
        return 1;
    }

    // Determine the output filename if not provided
    if (!outputFilename) {
        outputFilename = malloc(strlen(inputFilename) + 1);
        strcpy(outputFilename, inputFilename);
        char* extension = strrchr(outputFilename, '.');
        if (extension) {
            *extension = '\0';  // Remove the old extension
        }
        strcat(outputFilename, ".byt");
    }

    // Open the bytecode file for writing
    FILE* bytecodeFile = fopen(outputFilename, "wb");
    if (!bytecodeFile) {
        fclose(fakSourceFile);
        printf("Failed to create the bytecode file: %s\n", outputFilename);
        return 1;
    }

    // Write a custom header for identification (customize as needed)
    const char* header = "ATHENX_BYTECODE";
    fwrite(header, sizeof(char), strlen(header), bytecodeFile);

    char line[1000];
    while (fgets(line, sizeof(line), fakSourceFile)) {
        parseAndGenerateBytecode(bytecodeFile, line);
    }

    // Close the bytecode file
    fclose(bytecodeFile);

    // Close the Fak source file
    fclose(fakSourceFile);

    return 0;
}