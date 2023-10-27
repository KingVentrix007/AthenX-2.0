/**
 * Header Name: custom_con_parser.h
 * Description: Custom parser for .con files with a specific format.
 */

#ifndef CUSTOM_CON_PARSER_H
#define CUSTOM_CON_PARSER_H

// Define the maximum number of parameters and line length
#define MAX_PARAMETERS 20
#define MAX_LINE_LENGTH 100

/**
 * Function Name: parse_con_file
 * Description: Parse a .con file with the specified format.
 *
 * Parameters:
 *   filename (const char*) - The name of the .con file to parse.
 *  parameters_array (const char*) - The array of parameters retrieved form the .con file
 * Return:
 *   int - Return 0 for success, or a non-zero value for errors.
 */
int parse_con_file(const char* filename, char parameters_array[][MAX_PARAMETERS][MAX_LINE_LENGTH]);

#endif /* CUSTOM_CON_PARSER_H */