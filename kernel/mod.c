#include "../include/multiboot.h"
#include "../include/printf.h"
#include "../include/kernel.h"
#include "../include/mod.h"
#include "../include/string.h"







LoadedModule* get_loaded_modules(uintptr_t multiboot_header, int* num_modules) {
    // Ensure multiboot_header is valid
    if (multiboot_header == 0) {
        *num_modules = 0;
        return NULL;
    }

    // Extract the module information from the Multiboot header
    struct module* multiboot_modules = (struct module*)(multiboot_header + 16);
    *num_modules = *((int*)(multiboot_header + 12));

    // Allocate memory for LoadedModule array
    LoadedModule* modules = (LoadedModule*)malloc(sizeof(LoadedModule) * (*num_modules));

    // Populate the LoadedModule array
    for (int i = 0; i < *num_modules; ++i) {
        modules[i].start = multiboot_modules[i].mod_start;
        modules[i].end = multiboot_modules[i].mod_end;
    }

    return modules;
}


void* get_module_value(LoadedModule module) {
    void *buffer = kmalloc(module.end-module.start);
    memcpy(buffer, module.start, module.end - module.start);
    return buffer;   
}