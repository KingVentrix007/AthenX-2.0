
#include "../include/fat_filelib.h"
#include "../include/fat32.h"
#include "../include/installer.h"
#include "../include/errno.h"
#include "../include/version.h"
#include "../include/pci.h"
#include "../include/e1000.h"
#include "../include/background.h"
#include "../include/image.h"
#include "../include/printf.h"
#include "../include/vesa_display.h"
#include "../include/serial.h"
#include "../include/debug.h"
#include "../include/vesa.h"
#include "../include/vfs.h"
#include "../include/maths.h"
#include "../include/console.h"
#include "../include/kernel.h"
#include "../include/display.h"
#include "../include/panic.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/keyboard.h"
#include "../include/kheap.h"
#include "../include/multiboot.h"
#include "../include/pmm.h"
#include "../include/string.h"
#include "../include/ide.h"
#include "../include/keyboardmap.h"
#include "../include/types.h"
#include "../include/cmdhandler.h"
#include "../include/timer.h"
#include "../include/fs.h"
#include "../include/graphics.h"
#include "../include/fpu.h"
// #include "../include/ext2.h"
#include "../include/acpi.h"
#include "../include/ssfs.h"
#include "../include/fis.h"
#include "../include/ahci.h"
#include "../include/read.h"
#include "../include/write.h"
#include "../include/tui.h"
#include "../include/env.h"
// #include "../include/syscall.h"
#include "virt.h"
#include "../include/exe.h"
#include "../include/elf_exe.h"
#include "user.h"
#include "mod.h"
#include "qemu.h"
#include "sound.h"

/**
 * @brief Global variables used for kernel memory map, multi-boot information, programs, and initialization path.
 *
 * This section defines the following global variables:
 * - g_kmap: Represents the kernel memory map of type KERNEL_MEMORY_MAP.
 * - multi_boot_info: Represents the multi-boot information of type MULTIBOOT_INFO.
 * - programs: Represents an array of strings to store program names, with a maximum of MAX_PROGRAMS programs and each program name having a maximum length of 20 characters.
 * - init_path: Represents a string to store the initialization path with a maximum length of 100 characters.
 * - program_count: Represents the count of programs currently stored in the programs array.
 */
KERNEL_MEMORY_MAP g_kmap;
MULTIBOOT_INFO *multi_boot_info;
char programs[MAX_PROGRAMS][20];
char init_path[100];
int program_count = 0;
// void find_initramfs_location(MULTIBOOT_INFO *mb_info) {
//     if (mb_info->flags & (1 << 3)) {
//         uint32_t initramfs_start = mb_info->initramfs_start;
//         uint32_t initramfs_end = mb_info->initramfs_end;

//         // Print the initramfs location
//         printf_("Initramfs is loaded at addresses: 0x%X - 0x%X\n", initramfs_start, initramfs_end);
//     } else {
//         printf_("Initramfs location not found in Multiboot info\n");
//     }
// }

#define HBA_MEM_BASE_ADDRESS 0xFD0C0000

#define H2D_DATA_BASE_ADDRESS 0x1000000 // host to data を格納するアドレス
#define D2H_DATA_BASE_ADDRESS 0x1000000 // data to host を格納するアドレス

#define PORT_NUM 0

void testRead(HBA_MEM *ptr) {
	// 2018セクタをオフセット0~2017から読み込む, 512*2018 = 1M
	int result = sata_read(&(ptr->ports[PORT_NUM]), 0, 15, 16, (uint16_t*)D2H_DATA_BASE_ADDRESS);
	if (result) printf("testRead Success\n");
	else printf("test Read Error\n");
}

void testWrite(HBA_MEM *ptr) {
	int result = sata_write(&(ptr->ports[1]), 0, 2017, 2018, (uint16_t*)H2D_DATA_BASE_ADDRESS);
	if (result) printf("testWrite Success\n");
	else printf("testWrite Error\n");
}
void setup(HBA_MEM* ptr) {

	// sata clock program
	ptr->pctrl = 0x010200;

	// axis bus setup 
	ptr->paxic = 0x1;

	/* Port Phy Cfg register enables */
	ptr->pcfg = 0x2;

	/* Phy Control OOB timing parameters COMINIT */
	ptr->pp2c = 0x28184616;

	/* Phy Control OOB timing parameters COMWAKE */
	ptr->pp3c = 0x13081907;

	/* Phy Control Burst timing setting */
	ptr->pp4c = 0x064A0815;

	/* Rate Change Timer and Retry Interval Timer setting */
	ptr->pp5c = (0xB00 << 20);

	ptr->ports[0].sctl = (0x33 << 4);
	ptr->ports[0].serr = 0xffffffff;
}


MULTIBOOT_INFO *get_mb_info()
{
    return multi_boot_info;
}

/**
 * @brief locates an intird in memory
 * 
 * @param mbi 
 * @param size 
 * @return void* 
 */
static void* locate_initrd(MULTIBOOT_INFO *mbi, uint32_t* size)
{
    // If there are modules, the initramfs is located at the start of the
    // modules array, which is the first 32-bit word after the multiboot header.
    if (mbi->mods_count > 0)
    {
        // Get the start location of the modules array.
        uint32_t start_location = *((uint32_t*)mbi->mods_addr);
        // Get the end location of the modules array.
        uint32_t end_location = *(uint32_t*)(mbi->mods_addr + 4);

        // Calculate the size of the initramfs.
        *size = end_location - start_location;

        // Return the start location of the initramfs.
        return (void*)start_location;
    }

    // If there are no modules, return NULL.
    return NULL;
}
/*
  This code defines a function mac_cmp() that compares a hardcoded QEMU MAC address
  "52:54:00:12:34:56" with a given MAC address and returns 0 if they match.
*/

/**
 * @brief Compares a given MAC address with a hardcoded QEMU MAC address.
 *
 * This function compares the given MAC address with the MAC address "52:54:00:12:34:56"
 * obtained from QEMU command line. It returns 0 if the MAC addresses match.
 *
 * @return 0 if the MAC addresses match, otherwise a non-zero value.
 */
int mac_cmp() {
    const char* qemuMac = "52:54:00:12:34:56";
    return 0;
}


/**
 * @brief opens a grub module
 * 
 * @param mbi 
 */
void access_grub_module(MULTIBOOT_INFO* mbi) {
    printf_("GHERE\n");
    if (!(mbi->flags & 0x00000008))
    {
        printf_("No Grub modules\n");
        return; // Module information not available
    }
    

    uint32_t mods_count = mbi->mods_count;
    uint32_t mods_addr = mbi->mods_addr;

    struct module* modules = (struct module*)mods_addr;

    // Assuming you want to access the first module loaded by Grub
    if (mods_count >= 1) {
        uint32_t mod_start = modules[0].mod_start;
        uint32_t mod_end = modules[0].mod_end;
        printf_("There are %d modules", mods_count);

        // Now you can access the data in the module using the addresses mod_start and mod_end.
        // Be sure to check if the addresses are valid and properly aligned before accessing the data.
    }
}
//{{%code%}}
/**
 * @brief Get the kernel memory map object
 * 
 * @param kmap 
 * @param mboot_info 
 * @return int 
 */
int get_kernel_memory_map(KERNEL_MEMORY_MAP *kmap, MULTIBOOT_INFO *mboot_info) {
    uint32 i;

    // set kernel memory map
    if (kmap == NULL) return -1;
    kmap->kernel.k_start_addr = (uint32)&__kernel_section_start;
    kmap->kernel.k_end_addr = (uint32)&__kernel_section_end;
    kmap->kernel.k_len = ((uint32)&__kernel_section_end - (uint32)&__kernel_section_start);

    kmap->kernel.text_start_addr = (uint32)&__kernel_text_section_start;
    kmap->kernel.text_end_addr = (uint32)&__kernel_text_section_end;
    kmap->kernel.text_len = ((uint32)&__kernel_text_section_end - (uint32)&__kernel_text_section_start);

    kmap->kernel.data_start_addr = (uint32)&__kernel_data_section_start;
    kmap->kernel.data_end_addr = (uint32)&__kernel_data_section_end;
    kmap->kernel.data_len = ((uint32)&__kernel_data_section_end - (uint32)&__kernel_data_section_start);

    kmap->kernel.rodata_start_addr = (uint32)&__kernel_rodata_section_start;
    kmap->kernel.rodata_end_addr = (uint32)&__kernel_rodata_section_end;
    kmap->kernel.rodata_len = ((uint32)&__kernel_rodata_section_end - (uint32)&__kernel_rodata_section_start);

    kmap->kernel.bss_start_addr = (uint32)&__kernel_bss_section_start;
    kmap->kernel.bss_end_addr = (uint32)&__kernel_bss_section_end;
    kmap->kernel.bss_len = ((uint32)&__kernel_bss_section_end - (uint32)&__kernel_bss_section_start);

    // set system memory map
    kmap->system.total_memory = mboot_info->mem_lower + mboot_info->mem_upper;

    // search for available memory
    for (i = 0; i < mboot_info->mmap_length; i += sizeof(MULTIBOOT_MEMORY_MAP)) {
        MULTIBOOT_MEMORY_MAP *mmap = (MULTIBOOT_MEMORY_MAP *)(mboot_info->mmap_addr + i);
        if (mmap->type != MULTIBOOT_MEMORY_AVAILABLE) continue;
        // make sure kernel is loaded at 0x100000 by bootloader(see linker.ld)
        if (mmap->addr_low == kmap->kernel.text_start_addr) {
            // set available memory starting from end of our kernel, leaving 1MB size for functions exceution
            kmap->available.start_addr = kmap->kernel.k_end_addr + 1024 * 1024;
            kmap->available.end_addr = mmap->addr_low + mmap->len_low;
            // get availabel memory in bytes
            kmap->available.size = kmap->available.end_addr - kmap->available.start_addr;
            return 0;
        }
    }

    return -1;
}
/**
 * @brief print the kernel memory map
 * 
 * @param kmap 
 */
void display_kernel_memory_map(KERNEL_MEMORY_MAP *kmap) {
    // Print the kernel memory map
    printf_("kernel:\n");
    printf_("  kernel-start: 0x%x, kernel-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.k_start_addr, kmap->kernel.k_end_addr, kmap->kernel.k_len);
    printf_("  text-start: 0x%x, text-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.text_start_addr, kmap->kernel.text_end_addr, kmap->kernel.text_len);
    printf_("  data-start: 0x%x, data-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.data_start_addr, kmap->kernel.data_end_addr, kmap->kernel.data_len);
    printf_("  rodata-start: 0x%x, rodata-end: 0x%x, TOTAL: %d\n",
           kmap->kernel.rodata_start_addr, kmap->kernel.rodata_end_addr, kmap->kernel.rodata_len);
    printf_("  bss-start: 0x%x, bss-end: 0x%x, TOTAL: %d\n",
           kmap->kernel.bss_start_addr, kmap->kernel.bss_end_addr, kmap->kernel.bss_len);

    // Print the system memory map
    printf_("total_memory: %d KB\n", kmap->system.total_memory);
    printf_("available:\n");
    printf_("  start_adddr: 0x%x\n  end_addr: 0x%x\n  size: %d\n",
           kmap->available.start_addr, kmap->available.end_addr, kmap->available.size);
}
/**
 * @brief Copies the kernel memory map into a provided structure.
 *
 * This function takes a pointer to a KERNEL_MEMORY_MAP structure as input and
 * copies the contents of the global variable g_kmap into the provided pointer
 * using the memcpy function.
 *
 * @param[out] out Pointer to the target KERNEL_MEMORY_MAP structure.
 */
void get_map(KERNEL_MEMORY_MAP *out)
{
    memcpy(out, &g_kmap, sizeof(*out));
}
/**
 * @brief Get the Size In Sectors object
 * 
 * @param buffer 
 * @return unsigned int 
 */
// Function to extract the size in bytes from a char buffer
// and return the number of sectors (512 bytes per sector)
unsigned int getSizeInSectors(const char* buffer) {
    // Look for the line starting with "#SIZE"
    const char* sizeLine = strstr(buffer, "#SIZE");
    if (sizeLine == NULL) {
        // Return an error value or handle the absence of #SIZE line
        return 0;
    }

    // Move the pointer to the beginning of the number
    sizeLine += strlen("#SIZE");

    // Parse the size as an integer
    unsigned int sizeInBytes = atoi(sizeLine);

    // Calculate the number of sectors (512 bytes per sector)
    unsigned int sectors = (sizeInBytes + 511) / 512;

    return sectors;
}
// This should go outside any function..
/**
 * @brief The main entry point of the OS
 * 
 * @param magic 
 * @param addr 
 */
void kmain(unsigned long magic, unsigned long addr) {
    FUNC_ADDR_NAME(&kmain,2,"ui");
    MULTIBOOT_INFO *mboot_info;
    gdt_init();
    idt_init();
    init_irq_manager();
    init_serial(DEFAULT_COM_DEBUG_PORT);
    set_scroll_mode(1);
    kassert(display_init(1,0,0,32),0,1);
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        mboot_info = (MULTIBOOT_INFO *)addr;
        memset(&g_kmap, 0, sizeof(KERNEL_MEMORY_MAP));
        if (get_kernel_memory_map(&g_kmap, mboot_info) < 0) {
            printf_("error: failed to get kernel memory map\n");
            kassert(1,0,4);
            return;
        }
        pmm_init(g_kmap.available.start_addr, g_kmap.available.size);
        pmm_init_region(g_kmap.available.start_addr, PMM_BLOCK_SIZE * 256);
        void *start = pmm_alloc_blocks(50);
        void *end = start + (pmm_next_free_frame(1) * PMM_BLOCK_SIZE);
        kheap_init(start, end);
        int x = 1280;
        int y = 768;
        int ret = display_init(0,x,y,32);
        keyboard_init();
        printf("\nTotal memory size = %d",end-start);
        ata_init();
        clear_screen();
        set_screen_x(0);
        set_screen_y(0);
        set_scroll_mode(0);
        char* cmdlines = (char*)(uintptr_t)mboot_info->cmdline;
        printf("args = %s\n",cmdlines);
        printf("kernel memory start address this postion = %p\n",mboot_info->addr);
        fl_init();
        if (fl_attach_media(ide_read_sectors_fat, ide_write_sectors_fat) != FAT_INIT_OK)
        {
            printf("ERROR: Failed to init file system\n");
        }
        if(strstr(cmdlines,"update") != NULL)
        {
            clear_screen();
            cmd_handler("update","");
            clear_display();
            printf("Update complete\n");
            printf("restart device\n");
            reboot_qemu(0x60);
        }
         const char* filename = "/var/env.txt";
         FILE *f = fl_fopen(filename,"r");
        if(f != NULL)
        {
            fl_fclose(f);
            char** env_vars = NULL;
            int maxVariables = 10;
            int numLoaded = loadEnvironmentVariables(filename, &env_vars, maxVariables);
            init_env(env_vars,numLoaded);
            if (numLoaded > 0) {
                printf("Loaded %d environmental variables:\n", numLoaded);
                for (int i = 0; i < numLoaded; i++) {
                    free(env_vars[i]);
                }
                free(env_vars);
            } else if (numLoaded == -1) {
                printf("Error opening the file.\n");
            } else {
                printf("Memory allocation error.\n");
            }
        }
        else
        {
            const char* filename = "/var/env.txt";
            const char* env_vars[] = {
            "HOME=/root/",
            "BOOT=42",
            "JUNK=FILES",
            "INIT=/sys/shell",
            "PATH=/sys/,/sys/bin/,/sysroot/"
        };
        int saveResult = saveEnvironmentVariables(filename, env_vars, sizeof(env_vars) / sizeof(env_vars[0]));
        if (saveResult == 0) {
            printf("Environmental variables saved successfully.\n");
        } else {
            printf("Error saving environmental variables.\n");
        }
        }
        char output[71629] = {0};
        if(mboot_info->mods_count >= -100)
        {
            int mod_count = 0;
            LoadedModule * modules = get_loaded_modules(mboot_info,&mod_count);
            void *buffer = get_module_value(modules[0]);
            printf("%s\n",buffer);
            free(buffer);
        }
        else
        {
            printf("No modules found\n");
        }
        timer_init();
        reserve_irq(42);
        init_pci_device();
        const char *directory = "/sys/";
        program_count = find_programs(directory);
        printf("%d programs found\n",program_count);
        char list[100][20];
        populate_list_from_filenames(list, 100);
        for (int i = 0; i < program_count; i++) {
            printf("Program %d: %s\n", i, list[i]);
        }
        fpu_enable();
        asm("int $45");
        printf("Initialization complete\nAthenX %s:%d.%d.%d started successfully with %d errors\n",VERSION_MAIN,VERSION_SUB_MAIN,VERSION_SUB_MINOR,VERSION_SUB_PATCH,get_num_errors());
        printf("Welcome to AthenX-2.0. This is the default terminal, please run load <name> to execute your terminal or <name> to run a script\n");
        terminal_main();
done:
        pmm_free_blocks(start, 256);
        pmm_deinit_region(g_kmap.available.start_addr, PMM_BLOCK_SIZE * 256);
    } else {
        printf_("error: invalid multiboot magic number\n");
    }
}


int login(int skip)
{
    #ifdef NO_PASSWORD
    return 0;
    #endif
    
    set_scroll_mode(0);
    
    FILE *fp = fopen("/sec/user","r");
    if(get_file_size(fp) <= 1)
    {   
        fclose(fp);
        printf("NO USERS\n");
        char *new_home = "/home/tristan";
        User tristan = create_user("Tristan Kuhn", "tristan", "/home/tristan",
                               "/sys/shell", 1, "tristanjkuhn007@gmail.com",
                               "/home/tristan/config.conf", "123");
        add_user(&tristan);
    }
    else
    {
        fclose(fp);
    }
    
    char username[100] = {0};
    char password[100] = {0};
    memset(username,0,sizeof(username));
    username[0] = '\0';
    memset(password,0,sizeof(password));
    password[0] = '\0';
    printf("Enter username: ");
    
    while (1)
    {
         char *c = kb_getchar_w();
         if(c == '\b')
            {
                if(backspace(username))
                {
                    printf_("\b");
                }
                else
                {
                    ///beep();
                }
            }
            else if (c == '\n')
            {
                break;
            }
            else if (c == '\0')
            {
                //DEBUG("NULL");
                //beep();
            }
            else if((int)c != 0x48 && (int)c != 0x50 && (int)c != 0x4D && (int)c != 0x4B )
            {
                char* s;
                s = ctos(s, c);
                printf(s);
                append(username,c);
            }
            else
            {
            }
    }
    
    #define OVERRIDE
    #ifdef OVERRIDE
    if(strcmp(username,"override") == 0 || strcmp(username,"over") == 0)
    {
        return 1;
    }
    #endif
    
    printf("\nEnter password: ");
    
    while (1)
    {
         char *c = kb_getchar_w();
         if(c == '\b')
            {
                if(backspace(password))
                {
                    printf_("\b");
                }
                else
                {
                    ///beep();
                }
            }
            else if (c == '\n')
            {
                break;
            }
            else if (c == '\0')
            {
                //DEBUG("NULL");
                //beep();
            }
            else if(c != '\0')
            {
                char* s;
                s = ctos(s, c);
                printf("*");
                append(password,c);
            }
            else
            {
            }
    }
    
    printf("\n");
    
    if(validate_credentials(username,password) == 0 )
    {
        User user;
        get_user(username,&user);
        printf("home->%s\n",user.home_dir);
        fl_createdirectory(user.home_dir);
        set_cwd(user.home_dir);
        return 1;
    }
    else
    {
        printf("invalid\n");
        return -1;
    }
}
/**
 * @brief User login function.
 *
 * This function handles the user login process. It prompts the user to enter a username and password, validates the credentials, and performs necessary actions based on the result. The function also handles the case where no users are present by creating a default user. If the NO_PASSWORD macro is defined, the function returns 0 without performing any login process.
 *
 * @param skip An integer value indicating whether to skip the login process. If set to 1, the function returns 0 without performing any login process.
 * @return An integer value indicating the login status. Returns 1 if the login is successful, -1 if the credentials are invalid, and 0 if the NO_PASSWORD macro is defined.
 */
int login(int skip)
{
    #ifdef NO_PASSWORD
    return 0;
    #endif
    
    set_scroll_mode(0);
    FILE *fp = fopen("/sec/user","r");
    
    // Check if no users exist
    if(get_file_size(fp) <= 1)
    {   
        fclose(fp);
        printf("NO USERS\n");
        
        // Create a default user
        char *new_home = "/home/tristan";
        User tristan = create_user("Tristan Kuhn", "tristan", "/home/tristan",
                               "/sys/shell", 1, "tristanjkuhn007@gmail.com",
                               "/home/tristan/config.conf", "123");
        add_user(&tristan);
    }
    else
    {
        fclose(fp);
    }
    
    char username[100] = {0};
    char password[100] = {0};
    memset(username,0,sizeof(username));
    username[0] = '\0';
    memset(password,0,sizeof(password));
    password[0] = '\0';
    
    printf("Enter username: ");
    
    // Prompt for username
    while (1)
    {
         char *c = kb_getchar_w();
         
         if(c == '\b')
         {
            if(backspace(username))
            {
                printf_("\b");
            }
            else
            {
            }
         }
         else if (c == '\n')
         {
            break;
         }
         else if (c == '\0')
         {
         }
         else if((int)c != 0x48 && (int)c != 0x50 && (int)c != 0x4D && (int)c != 0x4B )
         {
            char* s;
            s = ctos(s, c);
            printf(s);
            append(username,c);
         }
         else
         {
         }
    }
    
    #define OVERRIDE
    #ifdef OVERRIDE
    if(strcmp(username,"override") == 0 || strcmp(username,"over") == 0)
    {
        return 1;
    }
    #endif
    
    printf("\nEnter password: ");
    
    // Prompt for password
    while (1)
    {
         char *c = kb_getchar_w();
         
         if(c == '\b')
         {
            if(backspace(password))
            {
                printf_("\b");
            }
            else
            {
            }
         }
         else if (c == '\n')
         {
            break;
         }
         else if (c == '\0')
         {
         }
         else if(c != '\0')
         {
            char* s;
            s = ctos(s, c);
            printf("*");
            append(password,c);
         }
         else
         {
         }
    }
    
    printf("\n");
    
    // Validate credentials
    if(validate_credentials(username,password) == 0 )
    {
        User user;
        get_user(username,&user);
        printf("home->%s\n",user.home_dir);
        fl_createdirectory(user.home_dir);
        set_cwd(user.home_dir);
        return 1;
    }
    else
    {
        printf("invalid\n");
        return -1;
    }
}
/**
 * @brief Reads input from the keyboard and stores it in the given buffer, up to the specified bound.
 * 
 * @param buffer Pointer to the buffer where the input will be stored.
 * @param bound The maximum number of characters that can be read.
 * 
 * @note If the buffer pointer is NULL, the function will return immediately.
 * @note The function will stop reading when a newline character is encountered or when the maximum number of characters is reached.
 * @note The function also handles backspace characters, allowing the user to delete characters from the input.
 */
void getstr_bound(char *buffer, uint8 bound) {
    if (!buffer) return;
    while(1) {
        char ch = kb_getchar();
        if (ch == '\n') {
            printf_("\n");
            return ;
        } else if(ch == '\b') {
            if(backspace(*buffer))
            {
                printf_("\b");
                
            }

        } else {
            *buffer++ = ch;
            printf_("%c", ch);
        }
    }
}
/**
 * @brief A boolean variable indicating the visibility of the cursor.
 * 
 * @note The value of this variable determines whether the cursor is currently visible or not.
 */
bool cursor_visible = true;

/**
 * @brief Toggles the visibility of the cursor.
 * 
 * @note The function changes the value of the  `cursor_visible`  variable, which determines whether the cursor is visible or not.
 */
void toggle_cursor_visibility() {
    cursor_visible = !cursor_visible;
}


/** 
 * @brief Set the initialization path. 
 * 
 * This function sets the initialization path to the specified  path . The initialization path is used in the  pre_terminal()  function to load an ELF file. The  path  parameter is a string representing the path to the desired file. 
 * 
 * @param path A null-terminated string representing the path to the initialization file. 
 * @return void 
 */ 
void set_init_path(char * path) 
{ 
    strcpy(init_path, path); 
} 


/**
 * @brief Pre-terminal initialization function.
 *
 * This function is responsible for initializing the pre-terminal environment. It takes command-line arguments  `argc`  and  `argv`  as parameters.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of strings representing the command-line arguments.
 * 
 * @return An integer value indicating the success or failure of the initialization process. If  `init_path`  is not set, the function returns -1 indicating failure.
 */
int pre_terminal(int argc, char **argv)
{
    if(init_path != "\0")
    {
        load_elf_file(init_path, argc, argv);
    }
    else
    {
        return -1;
    }
}

/** 
 * @brief The main function for the terminal program. 
 * 
 * This function implements the main functionality of the terminal program. It includes user login, handling user input, executing commands, interacting with the file system, and displaying the current working directory. The function uses various conditional statements and loops to control the program flow. 
 * 
 * @return void 
 */
void terminal_main()
{
    const char msg[] = "Hello world\nThis text is a message\nFOr u";
    int valid_login = 0;
    int attempts = 1;
    while (login(1) != 1)
    {
        valid_login = 1;
        attempts++;
        if(valid_login != 1)
        {
            printf("User credentials are not valid.\n");
        }
    }
    FUNC_ADDR_NAME(&terminal_main,0,"");
    char cwd_at[FATFS_MAX_LONG_FILENAME] = "";
    strcpy(cwd_at,get_cwd());
    printf("%s>",cwd_at);
    uint8_t byte;
    char buffer[512];
    memset(buffer,0,sizeof(buffer));
    buffer[0] = '\0';
    while(1)
    {    
        char *c = get_chr();
        int ticks = get_ticks();
        if(ticks >= 500)
        {
            undraw_square(get_screen_x(),get_screen_y());
            reset_ticks();
        }
        if (ticks >= 50) {
            toggle_cursor_visibility();
            reset_ticks();
        }
        if(c == '\b')
        {
            if(backspace(buffer))
            {
                printf_("\b");
            }
        }
        else if (c == '\n')
        {
            undraw_square(get_screen_y(),get_screen_x());
            cmd_handler(buffer,programs);
            memset(buffer, 0,sizeof(buffer));
            strcpy(cwd_at,get_cwd());
            printf("\n%s>",cwd_at);
        }
        else if (c == '\0')
        {
            
        }
        else if((int)c != 0x48 && (int)c != 0x50 && (int)c != 0x4D && (int)c != 0x4B )
        {
            char* s;
            s = ctos(s, c);
            undraw_square(get_screen_y(),get_screen_x());
            printf(s);
            append(buffer,c);
        }
        else
        {
        }
        if (cursor_visible) {
            draw_square_cursor(get_screen_y(),get_screen_x(),VBE_RGB(255, 255, 255));
        } else {
            undraw_square(get_screen_y(),get_screen_x());
        }
    }
}
int is_all_one_type(const char* str) {
    // Check if the string is empty
    if (str[0] == '\0') {
        return 1; // An empty string is considered to be all one type
    }

    char first_char = str[0]; // Store the first character

    // Iterate through the string
    for (int i = 1; str[i] != '\0'; i++) {
        // If any character is different from the first character, return 0
        if (str[i] != first_char) {
            return 0;
        }
    }

    // If we reach this point, all characters are the same
    return 1;
}
struct fake
{
    int v1;
    int v2;
    int v3;
};
int dummy_start()
{
    FUNC_ADDR_NAME(&dummy_start,0," ");
    dummy2();
}
int dummy2()
{
    FUNC_ADDR_NAME(&dummy2,0," ");
    struct fake ff;
    ff.v1 = 1;
    ff.v2 = 2;
    ff.v3 = 3;
    dummy3();
}
int dummy3()
{
    FUNC_ADDR_NAME(&dummy3,0," ");
    int q = 76;
    dummy4("CAT");
}

int dummy4(char *word) {
    FUNC_ADDR_NAME(&dummy4,1,"s");
    int fake_e = fake_err(100,2);
    sound_error();
    /* function_end variable */
    
}
/**
 * @brief Generates a fake error
 * 
 * @param z 
 * @param y 
 * @return int 
 */
int fake_err(int z, int y)
{
    // This function is used to test the error reporting
    FUNC_ADDR_NAME(&fake_err,2,"ii");
    int o = 1893;
    int x = z/y;
    return x;
}

