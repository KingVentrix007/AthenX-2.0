#ifndef MULTIBOOT_MODULE_H
#define MULTIBOOT_MODULE_H
#include "types.h"
/**
 * Structure to represent a loaded module's information
 */
typedef struct {
    uintptr_t start;  // Start address of the module
    uintptr_t end;    // End address of the module
} LoadedModule;

/**
 * Function Name: get_loaded_modules
 * Description: Retrieves information about the modules loaded by GRUB using the Multiboot header.
 *
 * Parameters:
 *   multiboot_header (uintptr_t) - The address of the Multiboot header provided by GRUB.
 *   num_modules (int*) - Pointer to an integer to store the number of loaded modules.
 *
 * Return:
 *   LoadedModule* - An array of LoadedModule structures containing start and end addresses of loaded modules.
 */
LoadedModule* get_loaded_modules(uintptr_t multiboot_header, int* num_modules);

/**
 * Function Name: get_module_value
 * Description: Extracts the value of a module based on its start and end addresses.
 *
 * Parameters:
 *   module (LoadedModule) - The LoadedModule structure representing the module.
 *
 * Return:
 *   void* - The value of the module as a string.
 */
void* get_module_value(LoadedModule module) ;

#endif // MULTIBOOT_MODULE_H