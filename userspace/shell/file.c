#include "../libc/include/stdio.h"
#include "file.h"


/**
 * Function Name: change_directory
 * Description: Change the current working directory based on user input.
 *
 * Parameters:
 *   arguments (char* []) - An array of strings containing the command arguments.
 *   arg_count (int) - The number of arguments in the array.
 *   cwd (char*) - The current working directory path.
 *
 * Return:
 *   None
 */
void change_directory(char* arguments[], int arg_count, char* cwd) {
    if (strcmp(arguments[0], "cd") == 0) {
        if (arg_count >= 2) {
            if (strcmp(arguments[1], "..") == 0) {
                // Check if the current directory is the rootfs directory
                if (strcmp(cwd, "root") == 0 || strcmp(cwd, "/root/") == 0) {
                    // Allow going one level above rootfs
                    strcpy(cwd, "/");
                } else if (strstr(cwd, "/") != 0) {
                    // Find the last '/' character in the current directory path
                    char* last_slash = strrchr(cwd, '/');
                    
                    if (last_slash != NULL) {
                        // Remove the last directory from the current path
                        *last_slash = '\0';
                        last_slash = strrchr(cwd, '/');
                        
                        if (last_slash != NULL) {
                            // Remove the last directory from the current path
                            *last_slash = '\0';
                        }
                    } else {
                        // If there is no '/', set the current directory to rootfs
                        strcpy(cwd, "root");
                    }
                }
                if (cwd[strlen(cwd) - 1] == '/' && cwd[strlen(cwd) - 2] == '/') {
                    cwd[strlen(cwd) - 1] = '\0';
                }
                if (cwd[strlen(cwd) - 1] != '/') {
                    strcat(cwd, "/");
                }
            } else { 
                char tmp[260];
                strcpy(tmp, cwd);
                if (tmp[strlen(tmp) - 1] != '/') {
                    strcat(tmp, "/");
                }
                strcat(tmp, arguments[1]);
                
                if (fl_is_dir(tmp) == 1) {
                    strcpy(cwd, tmp);
                } else {
                    printf("\n%s is not a valid path\n", tmp);
                }
                if (cwd[strlen(cwd) - 1] == '/' && cwd[strlen(cwd) - 2] == '/') {
                    cwd[strlen(cwd) - 1] = '\0';
                }
                if (cwd[strlen(cwd) - 1] != '/') { 
                    strcat(cwd, "/");
                }
            }
        }
    }
}
