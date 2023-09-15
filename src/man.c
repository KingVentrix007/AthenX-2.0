#include "strings.h"
#include "printf.h"

void man_inspect()
{
    printf("Inspect Command\n");
    printf("==============\n");
    printf("Description:\n");
    printf("    The 'inspect' command is used to inspect various system information.\n");
    printf("\nSubcommands:\n");
    printf("    1. inspect err\n");
    printf("        - Displays error information.\n");
    printf("    2. inspect log\n");
    printf("        - Displays event log information.\n");
    printf("        - Options:\n");
    printf("            a. --all\n");
    printf("                - Displays all events in the log.\n");
    printf("            b. --find\n");
    printf("                - Searches for specific events based on criteria.\n");
    printf("                - Prompts the user for start and end times, error status,\n");
    printf("                  and function name.\n");
    printf("    3. inspect pci\n");
    printf("        - Displays PCI device information.\n");
    printf("        - Options:\n");
    printf("            a. --list\n");
    printf("                - Lists all PCI devices.\n");
    printf("            b. --find <device>\n");
    printf("                - Searches for a specific PCI device by name.\n");
    printf("    4. inspect stack\n");
    printf("        - Displays stack trace information (currently disabled).\n");
    printf("    5. inspect drivers\n");
    printf("        - Displays driver information.\n");
    printf("        - Options:\n");
    printf("            a. --all\n");
    printf("                - Lists all registered devices.\n");
    printf("            b. --live\n");
    printf("                - Lists all currently running devices.\n");
}