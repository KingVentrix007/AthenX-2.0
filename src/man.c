#include "string.h"
#include "printf.h"
#include "display.h"
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
// For the 'pci' command
void man_pci() {
    printf("PCI Command\n");
    printf("===========\n");
    printf("Description:\n");
    printf("    The 'pci' command is used to interact with PCI devices.\n");
    printf("\nSubcommands:\n");
    printf("    1. pci list\n");
    printf("        - Lists all available PCI devices.\n");
    printf("    2. pci reg\n");
    printf("        - Lists all registered PCI devices.\n");
}

// For the 'write' command
void man_write() {
    printf("Write Command\n");
    printf("=============\n");
    printf("Description:\n");
    printf("    The 'write' command is used to write data to a file.\n");
    printf("\nUsage:\n");
    printf("    write <filename> <file extension>\n");
}

// For the 'read' command
void man_read() {
    printf("Read Command\n");
    printf("============\n");
    printf("Description:\n");
    printf("    The 'read' command is used to read the contents of a file.\n");
    printf("\nUsage:\n");
    printf("    read <filename> <file extension>\n");
}

// For the 'ls' command
void man_ls() {
    printf("LS Command\n");
    printf("==========\n");
    printf("Description:\n");
    printf("    The 'ls' command is used to list files and directories in the current directory.\n");
    printf("\nUsage:\n");
    printf("    ls\n");
}

// For the 'rm' command
void man_rm() {
    printf("RM Command\n");
    printf("==========\n");
    printf("Description:\n");
    printf("    The 'rm' command is used to remove (delete) a file.\n");
    printf("\nUsage:\n");
    printf("    rm <filename>\n");
}

// For the 'cls' command
void man_cls() {
    printf("CLS Command\n");
    printf("===========\n");
    printf("Description:\n");
    printf("    The 'cls' command is used to clear the screen.\n");
    printf("\nUsage:\n");
    printf("    cls\n");
}

// For the 'bg' command
void man_bg() {
    printf("BG Command\n");
    printf("==========\n");
    printf("Description:\n");
    printf("    The 'bg' command is used to change the background colors or themes.\n");
    printf("\nUsage:\n");
    printf("    bg\n");
}

// For the 'logo' command
void man_logo() {
    printf("Logo Command\n");
    printf("============\n");
    printf("Description:\n");
    printf("    The 'logo' command is used to display a logo on the screen.\n");
    printf("\nUsage:\n");
    printf("    logo\n");
}

// For the 'mkdir' command
void man_mkdir() {
    printf("Mkdir Command\n");
    printf("==============\n");
    printf("Description:\n");
    printf("    The 'mkdir' command is used to create a new directory.\n");
    printf("\nUsage:\n");
    printf("    mkdir <directory_name>\n");
}

// For the 'exit' command
void man_exit() {
    printf("Exit Command\n");
    printf("============\n");
    printf("Description:\n");
    printf("    The 'exit' command is used to exit the application or system.\n");
    printf("\nUsage:\n");
    printf("    exit\n");
}

// For the 'set-xy' command
void man_set_xy() {
    printf("Set-xy Command\n");
    printf("==============\n");
    printf("Description:\n");
    printf("    The 'set-xy' command is used to set the screen coordinates (X and Y positions).\n");
    printf("\nUsage:\n");
    printf("    set-xy <x> <y>\n");
}

// For the 'song' command
void man_song() {
    printf("Song Command\n");
    printf("============\n");
    printf("Description:\n");
    printf("    The 'song' command is used to play a song or music.\n");
    printf("\nUsage:\n");
    printf("    song\n");
}

// For the 'format' command
void man_format() {
    printf("Format Command\n");
    printf("==============\n");
    printf("Description:\n");
    printf("    The 'format' command is used to format a disk or drive.\n");
    printf("\nUsage:\n");
    printf("    format <disk_number> <disk_name>\n");
}

// For the 'table' command
void man_table() {
    printf("Table Command\n");
    printf("==============\n");
    printf("Description:\n");
    printf("    The 'table' command is used to display partition table information.\n");
    printf("\nUsage:\n");
    printf("    table\n");
}

// For the '3d' command
void man_3d() {
    printf("3D Command\n");
    printf("===========\n");
    printf("Description:\n");
    printf("    The '3d' command is used to enable 3D rendering or display 3D graphics.\n");
    printf("\nUsage:\n");
    printf("    3d\n");
}

// For the 'rle' command
void man_rle() {
    printf("RLE Command\n");
    printf("============\n");
    printf("Description:\n");
    printf("    The 'rle' command is used for a specific purpose. Please refer to the documentation.\n");
    printf("\nUsage:\n");
    printf("    rle\n");
}

// For the 'fs' command
void man_fs() {
    printf("FS Command\n");
    printf("===========\n");
    printf("Description:\n");
    printf("    The 'fs' command is used for file system-related operations. Please refer to the documentation.\n");
    printf("\nUsage:\n");
    printf("    fs\n");
}

// For the 'exe' command
void man_exe() {
    printf("Exe Command\n");
    printf("============\n");
    printf("Description:\n");
    printf("    The 'exe' command is used to execute a command or program.\n");
    printf("\nUsage:\n");
    printf("    exe <command_or_program_name>\n");
}

// For the 'sect' command
void man_sect() {
    printf("raw Command\n");
    printf("============\n");
    printf("Description:\n");
    printf("    The 'raw' command is used for sector-related operations. Please refer to the documentation.\n");
    printf("\nUsage:\n");
    printf("    raw <argument> <argument>\n");

}
void man_raw() {
    printf("Raw Command\n");
    printf("===========\n");
    printf("Description:\n");
    printf("    The 'raw' command is used for low-level disk sector operations.\n");
    printf("\nSubcommands:\n");
    
    printf("1. raw read <drive ><lba>\n");
    printf("   - Read a sector from the disk.\n");
    printf("   - <drive>: The drive to read from to read from.\n");
    printf("   - <lba>: The logical block address to read from.\n");
    printf("2. raw write <lba> <data> <file size>\n");
    printf("   - Write data to a sector on the disk.\n");
    printf("   - <lba>: The logical block address to write to.\n");
    printf("   - <data>: The data to write to the sector.\n");
    printf("   - <file size>: The number of chars you think you will write.\n");
}
// For the 'edit' command
void man_edit() {
    printf("Edit Command\n");
    printf("============\n");
    printf("Description:\n");
    printf("    The 'edit' command is used to edit a file. Please refer to the documentation.\n");
    printf("\nUsage:\n");
    printf("    edit <filename>\n");
}

void man_main(const char* functionName) {
    if (strcmp(functionName, "pci") == 0) {
        man_pci();
    } else if (strcmp(functionName, "write") == 0) {
        man_write();
    } else if (strcmp(functionName, "read") == 0) {
        man_read();
    } else if (strcmp(functionName, "ls") == 0) {
        man_ls();
    } else if (strcmp(functionName, "rm") == 0) {
        man_rm();
    } else if (strcmp(functionName, "cls") == 0) {
        man_cls();
    } else if (strcmp(functionName, "bg") == 0) {
        man_bg();
    } else if (strcmp(functionName, "logo") == 0) {
        man_logo();
    } else if (strcmp(functionName, "mkdir") == 0) {
        man_mkdir();
    } else if (strcmp(functionName, "exit") == 0) {
        man_exit();
    } else if (strcmp(functionName, "set-xy") == 0) {
        man_set_xy();
    } else if (strcmp(functionName, "song") == 0) {
        man_song();
    } else if (strcmp(functionName, "format") == 0) {
        man_format();
    } else if (strcmp(functionName, "table") == 0) {
        man_table();
    } else if (strcmp(functionName, "3d") == 0) {
        man_3d();
    } else if (strcmp(functionName, "rle") == 0) {
        man_rle();
    } else if (strcmp(functionName, "fs") == 0) {
        man_fs();
    } else if (strcmp(functionName, "exe") == 0) {
        man_exe();
    } else if (strcmp(functionName, "sect") == 0) {
        man_sect();
    } else if (strcmp(functionName, "edit") == 0) {
        man_edit();
    }else if (strcmp(functionName,"raw") == 0)
    {
        man_raw();
    }
     else if(strcmp(functionName,"inspect") == 0){
        man_inspect();

    }else {
        printf("No manual page available for '%s'\n", functionName);
    }
}

int man_list()
{
    char *cmds[] = {"inspect","raw","edit","sect","exe","fs","rle","3d","table","format","song","set-xy","exit","mkdir","logo","bg","cls","rm","ls","read","write","pci"};
    int option  = printArrayWithSelection(cmds,22);
    man_main(cmds[option]);
}