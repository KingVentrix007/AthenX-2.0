#include "kheap.h"
#include "string.h"
#include "env.h"
#include "fat_filelib.h"
#include "cmdhandler.h"
int saveEnvironmentVariables(const char* filename, const char* variables[], int numVariables) {
    FILE* file = fl_fopen(filename, "w");
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

int loadEnvironmentVariables(const char* filename, char*** variables, int maxVariables) {
    FILE* file = fl_fopen(filename, "r");
    if (file == NULL) {
        return -1; // Error opening the file.
    }

    int numLoaded = 0;
    char line[256]; // Assuming a maximum line length of 255 characters.

    *variables = (char**)malloc(sizeof(char*) * maxVariables);

    if (*variables == NULL) {
        fclose(file);
        return -1; // Memory allocation error.
    }

    while (numLoaded < maxVariables && fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove trailing newline.

        (*variables)[numLoaded] = (char*)malloc(strlen(line) + 1);
        if ((*variables)[numLoaded] == NULL) {
            // Handle memory allocation error.
            fclose(file);

            // Free memory allocated for previous variables.
            for (int i = 0; i < numLoaded; i++) {
                free((*variables)[i]);
            }
            free(*variables);

            return -1;
        }

        strcpy((*variables)[numLoaded], line);
        numLoaded++;

        // Resize the variables array if needed.
        if (numLoaded >= maxVariables) {
            maxVariables *= 2; // Double the size.
            *variables = (char**)realloc(*variables, sizeof(char*) * maxVariables);

            if (*variables == NULL) {
                // Handle memory allocation error.
                fclose(file);
                return -1;
            }
        }
    }

    fclose(file);
    return numLoaded;
}

void init_env(char* env_vars[], int numVars) {
    for (int i = 0; i < numVars; i++) {
        char* variable = env_vars[i];
        char* name = strtok(variable, "=");
        char* value = strtok(NULL, "=");

        if (name != NULL && value != NULL) {
            if (strcmp(name, "HOME") == 0) {
                // If the variable is "HOME," set the current working directory.
                //printf("set home");
                set_cwd(value);
            }

            // Optionally, you can also set other environmental variables as needed.
        }
    }
}