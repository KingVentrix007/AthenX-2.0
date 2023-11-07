#include "../include/kheap.h"
#include "../include/string.h"
#include "../include/env.h"
#include "../include/fat_filelib.h"
#include "../include/cmdhandler.h"
#include "../include/kernel.h"
char **executable_paths;
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
            else if (strcmp(name,"INIT") == 0)
            {
                set_init_path(value);
            }
            else if (strcmp(name,"PATH") == 0)
            {
                executable_paths = kmalloc(sizeof(char*) * 100);
                int count = 0;
                split_paths(value, &executable_paths, &count);
                for (int i = 0; i < count; i++) {
                printf("Path %d: %s\n", i + 1, executable_paths[i]);
                // free(paths[i]); // Free allocated memory for each path
            }

                // free(paths); // Free the paths array
                    /* code */
            }
            
            

            // Optionally, you can also set other environmental variables as needed.
        }
    }
}

/**
 * Function Name: split_paths
 * Description: Split a string of paths separated by commas into an array of individual paths.
 *
 * Parameters:
 *   path_string (char*) - A string containing paths separated by commas.
 *   paths (char***) - A pointer to an array of strings to store individual paths.
 *   count (int*) - A pointer to an integer to store the number of paths found.
 *
 * Return:
 *   void
 */
void split_paths(const char* path_string, char*** paths, int* count) {
    // Make a copy of the input string as strtok modifies the original string
    char* copy = strdup(path_string);

    char* token = strtok(copy, ",");
    int num_paths = 0;
    while (token != NULL) {
        // Allocate memory for each path and copy the token into it
        (*paths)[num_paths] = strdup(token);
        num_paths++;
        token = strtok(NULL, ",");
    }

    // Update the count
    *count = num_paths;

    // Clean up the copy
    free(copy);
}

int get_executable_paths(char **path)
{
    path = executable_paths;
}