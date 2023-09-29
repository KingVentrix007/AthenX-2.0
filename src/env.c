#include "kheap.h"
#include "string.h"
#include "env.h"
#include "fat_filelib.h"
int saveEnvironmentVariables(const char* filename, const char* variables[], int numVariables) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return -1; // Error opening the file.
    }

    for (int i = 0; i < numVariables; i++) {
        if (fputs(variables[i], file) == EOF || fputs("\n", file) == EOF) {
            // Error writing to the file.
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0; // Success.
}

int loadEnvironmentVariables(const char* filename, char* variables[], int maxVariables) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1; // Error opening the file.
    }

    int numLoaded = 0;
    char line[256]; // Assuming a maximum line length of 255 characters.

    while (numLoaded < maxVariables && fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove trailing newline.

        // Allocate memory for the variable and copy it.
        variables[numLoaded] = (char*)malloc(strlen(line) + 1);
        if (variables[numLoaded] == NULL) {
            // Handle memory allocation error.
            fclose(file);
            return -1;
        }

        strcpy(variables[numLoaded], line);
        numLoaded++;
    }

    fclose(file);
    return numLoaded;
}
