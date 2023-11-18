#include "../include/fat_filelib.h"
#include "../include/con.h"

int parse_con_file(const char* filename, char parameters_array[][MAX_PARAMETERS][MAX_LINE_LENGTH]) {
    if (filename == NULL) {
        // fprintf(stderr, "Error: NULL filename provided.\n");
        return 1; // Return an error code
    }

    if (strlen(filename) < 4 || strcmp(filename + strlen(filename) - 4, ".con") != 0) {
        // fprintf(stderr, "Error: File is not a .con file.\n");
        return 2; // Return an error code
    }

    // Open the .con file for parsing
    FILE* con_file = fopen(filename, "r");
    if (con_file == NULL) {
        perror("Error opening .con file");
        return 3; // Return an error code
    }

    char line[MAX_LINE_LENGTH + 1];
    int line_count = 0;

    while (fgets(line, sizeof(line), con_file) != NULL) {
        char* token;
        char* rest = line;
        int param_count = 0;

        // Tokenize the line by '=' and ',' to extract parameters
        while ((token = strtok_r(rest, "=,\n", &rest)) != NULL) {
            if (param_count < MAX_PARAMETERS) {
                strncpy(parameters_array[line_count][param_count], token, MAX_LINE_LENGTH);
                param_count++;
            } else {
                // fprintf(stderr, "Error: Too many parameters in a line.\n");
                fclose(con_file);
                return 4; // Return an error code
            }
        }

        // Increment the line count
        line_count++;
    }

    // Close the file when done
    fclose(con_file);

    // Return the number of lines parsed
    return line_count;
}