#ifndef ENVIRONMENT_MANAGER_H
#define ENVIRONMENT_MANAGER_H

/**
 * Function Name: saveEnvironmentVariables
 * Description: Saves environmental variables to a file.
 *
 * Parameters:
 *   filename (const char*) - The name of the file to save variables to.
 *   variables (const char* []) - An array of environmental variables in key-value format.
 *   numVariables (int) - The number of variables in the array.
 *
 * Return:
 *   int - 0 if successful, an error code if not.
 */
int saveEnvironmentVariables(const char* filename, const char* variables[], int numVariables);

/**
 * Function Name: loadEnvironmentVariables
 * Description: Loads environmental variables from a file.
 *
 * Parameters:
 *   filename (const char*) - The name of the file to load variables from.
 *   variables (char* []) - An array to store the loaded environmental variables.
 *   maxVariables (int) - The maximum number of variables to load.
 *
 * Return:
 *   int - The number of variables loaded, or -1 if there was an issue.
 */
int loadEnvironmentVariables(const char* filename, char* variables[], int maxVariables);

#endif
