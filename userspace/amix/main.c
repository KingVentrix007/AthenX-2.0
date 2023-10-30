/**
 * File: AMIX.c
 * Description: AMIX shell script parser for AthenX.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "search.h"
#include "terminal.h"
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <cmd> <cmd arguments>\n", argv[0]);
        return -1;
    } else if (strcmp(argv[1], "search") == 0) {
        if (argc <= 3) {
            printf("Usage: %s search <file_path> <search_text> [case_sensitive]\n", argv[0]);
            return -1;
        }

        const char *file_path = argv[2];
        const char *search_text = argv[3];
        int case_sensitive = 0;  // Default to case-insensitive

        // Check if a case-sensitive flag is provided and set it accordingly
        // if (argc > 4) {
        //     if (strcmp(argv[4], "1") == 0) {
        //         case_sensitive = 1;
        //     }
        // }
        case_sensitive = 1;

        // Concatenate and pass extra parameters as search text
        if (argc > 5) {
            for (int i = 4; i < argc; i++) {
                strcat(search_text, " ");  // Add a space separator
                strcat(search_text, argv[i]);
            }
        }

        int matches = search_file(file_path, search_text, case_sensitive);

        if (matches >= 0) {
            printf("Number of matches: %d\n", matches);
        } else {
            printf("An error occurred while opening the file.\n");
        }
    }
    else if (strcmp(argv[1], "fsearch") == 0) {
        if (argc <= 3) {
            printf("Usage: %s fsearch <file_path> <search_text> [case_sensitive]\n", argv[0]);
            return -1;
        }

        const char *file_path = argv[2];
        const char *search_text = argv[3];
        int case_sensitive = 0;  // Default to case-insensitive

        // Check if a case-sensitive flag is provided and set it accordingly
        if (argc > 4) {
            if (strcmp(argv[4], "1") == 0) {
                case_sensitive = 1;
            }
        }

        search_first_match(file_path, search_text, case_sensitive);
    }
    else if (strcmp(argv[1], "csearch") == 0) {
        // AthenXTerminal term;
        // term.auto_scroll = false;
        // set_term_attr(term);
        if (argc <= 3) {
            printf("Usage: %s csearch <file_path> <search_text> [case_sensitive]\n", argv[0]);
            return -1;
        }

        const char *file_path = argv[2];
        const char *search_text = argv[3];
        int case_sensitive = 0;  // Default to case-insensitive

        // Check if a case-sensitive flag is provided and set it accordingly
        if (argc > 4) {
            if (strcmp(argv[4], "1") == 0) {
                case_sensitive = 1;
            }
        }

        csearch_file(file_path, search_text, case_sensitive);
        // term.auto_scroll = true; //
        // set_term_attr(term); //
        // fclose(file);
    }

    return 0;
}




