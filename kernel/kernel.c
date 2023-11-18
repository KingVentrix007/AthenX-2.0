
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
#include "sb16.h"

// #include "flanterm.h"
// #include "fb.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "../include/stb_image.h"
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
static void* locate_initrd(MULTIBOOT_INFO *mbi, uint32_t* size)
{
    if (mbi->mods_count > 0)
    {
        uint32_t start_location = *((uint32_t*)mbi->mods_addr);
        uint32_t end_location = *(uint32_t*)(mbi->mods_addr + 4);

        *size = end_location - start_location;

        return (void*)start_location;
    }

    return NULL;
}

int mac_cmp() {
    // MAC address obtained from QEMU command line
    beep();
    const char* qemuMac = "52:54:00:12:34:56";
    
    // Read the MAC address from your custom OS
    // MacAddress osMac = read_mac_address();
    //  print_mac_address(&osMac);
    // Compare the MAC addresses
    // if (compare_mac_addresses(qemuMac, &osMac)) {
    //    // beep();
    //     printf_("MAC addresses match!\n");
    // } else {
        
    //     printf("MAC addresses do not match!\n");
    //     printf("MAC address of card:");
       
    //     printf("\n");
    //     printf("MAC address of set: %s\n ",qemuMac);
    // }

    return 0;
}

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

int get_kernel_memory_map(KERNEL_MEMORY_MAP *kmap, MULTIBOOT_INFO *mboot_info) {
    uint32 i;

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

    kmap->system.total_memory = mboot_info->mem_lower + mboot_info->mem_upper;

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

void display_kernel_memory_map(KERNEL_MEMORY_MAP *kmap) {
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

    printf_("total_memory: %d KB\n", kmap->system.total_memory);
    printf_("available:\n");
    printf_("  start_adddr: 0x%x\n  end_addr: 0x%x\n  size: %d\n",
           kmap->available.start_addr, kmap->available.end_addr, kmap->available.size);
}
void get_map(KERNEL_MEMORY_MAP *out)
{
    memcpy(&out, &g_kmap, sizeof(out));
}
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

void kmain(unsigned long magic, unsigned long addr) {
    //  enable_paging_c();
    FUNC_ADDR_NAME(&kmain,2,"ui");
    MULTIBOOT_INFO *mboot_info;
   
    gdt_init();
    idt_init();
    init_serial(DEFAULT_COM_DEBUG_PORT);
    //
    //ata_get_drive_by_model
    set_scroll_mode(1);
    kassert(display_init(1,0,0,32),0,1);
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        mboot_info = (MULTIBOOT_INFO *)addr;
        //multi_boot_info = mboot_info;
        memset(&g_kmap, 0, sizeof(KERNEL_MEMORY_MAP));
        if (get_kernel_memory_map(&g_kmap, mboot_info) < 0) {
            printf_("error: failed to get kernel memory map\n");
            kassert(1,0,4);
            return;
        }
        // put the memory bitmap at the start of the available memory
        pmm_init(g_kmap.available.start_addr, g_kmap.available.size);
        // initialize atleast 1MB blocks of memory for our heap
        pmm_init_region(g_kmap.available.start_addr, PMM_BLOCK_SIZE * 256);
        
        // initialize heap 256 blocks(1MB)
        //multi_boot_info = mboot_info;
        //memcpy(multi_boot_info, mboot_info,sizeof(multi_boot_info));
        void *start = pmm_alloc_blocks(50);
        void *end = start + (pmm_next_free_frame(1) * PMM_BLOCK_SIZE);
        kheap_init(start, end);
        
        //@ Gets screen size from memory
        
        
        int x = 1280;
        int y = 768;
        
        
        //kassert(init_serial(DEFAULT_COM_DEBUG_PORT),0,2);
        int ret = display_init(0,x,y,32);
        keyboard_init();
        printf("\nTotal memory size = %d",end-start);
        // char* cmdline = (char*)(uintptr_t)mboot_info->cmdline;
        ata_init();
        clear_screen();
        set_screen_x(0);
        set_screen_y(0);
        set_scroll_mode(0);
        // init_virt();enable_paging()
        char* cmdlines = (char*)(uintptr_t)mboot_info->cmdline;
        printf("args = %s\n",cmdlines);

        printf("kernel memory start address this postion = %p\n",mboot_info->addr);
        // printf("args->%s\n",cmdline);
         fl_init();
          if (fl_attach_media(ide_read_sectors_fat, ide_write_sectors_fat) != FAT_INIT_OK)
        {
            printf("ERROR: Failed to init file system\n");
            //return -1;
        }
        else
        {
            
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
            int maxVariables = 10; // Set the initial maximum number of variables.

            int numLoaded = loadEnvironmentVariables(filename, &env_vars, maxVariables);
            init_env(env_vars,numLoaded);
            if (numLoaded > 0) {
                printf("Loaded %d environmental variables:\n", numLoaded);
                for (int i = 0; i < numLoaded; i++) {
                    //printf("%s\n", env_vars[i]);
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
            // Add more environmental variables as needed.
        };

        int saveResult = saveEnvironmentVariables(filename, env_vars, sizeof(env_vars) / sizeof(env_vars[0]));
        
        if (saveResult == 0) {
            printf("Environmental variables saved successfully.\n");
        } else {
            printf("Error saving environmental variables.\n");
        }
        }
          
        //  if (loadAndDrawImage("/gui/sunset.tga", 0, 0) == 0) {
        // printf("succsess\n");
        // set_screen_x(0);
        // set_screen_y(0);
        // } else {
        //     // Error occurred
        // }
         char output[71629] = {0};
        if(mboot_info->mods_count >= -100)
        {
            int mod_count = 0;
            LoadedModule * modules = get_loaded_modules(mboot_info,&mod_count);
            void *buffer = get_module_value(modules[0]);
            printf("%s\n",buffer);
            free(buffer);
            //  printf("Mod count: %d\n", mboot_info->mods_count);
            // uint32_t initrd_size = 0;
            // uint8_t* initrd_location = locate_initrd(mboot_info, &initrd_size);
            // uint8_t* initrd_end_location = initrd_location + initrd_size;
            // printf("Initrd found at %x - %x (%d bytes)\n", initrd_location, initrd_end_location, initrd_size);
           
            // memcpy(output, initrd_location, initrd_size);
        }
        else
        {
            printf("No modules found\n");
            //  int mod_count = 0;
            // LoadedModule * modules = get_loaded_modules(mboot_info,&mod_count);
            // void *buffer = get_module_value(modules[0]);
            // printf("%s\n",buffer);
            // free(buffer);

        }
            // unsigned char* bios_start = (unsigned char*)0xE0000;
            // unsigned int bios_length = 0x20000;  // 128 KB

            // struct XSDP_t* rsdp = find_rsdp(bios_start, bios_length);

            // if (rsdp) {
            //     printf("RSDP version %d found at address: 0x%p\n",rsdp->Revision, rsdp);
            //     struct ACPISDTHeader* RSDT = (struct ACPISDTHeader*)rsdp->RsdtAddress;
                
            //     bool valid = doChecksum(RSDT);
            //     if(valid == TRUE)
            //     {
            //         printf("Creator ID: %d\n",RSDT->CreatorID);
            //     }
            //     else
            //     {
            //         printf("[ERROR]Invalid RSDT Table found\n");
            //     }
            //     //printf("Version: %d\n",rsdp->Revision);
            
            //     // Access other information in the RSDP as needed
            // } else {
            //     printf("RSDP not found.\n");
            // }
        // initAcpi();
        // acpiEnable();
        
        timer_init();//!DO NOT PUT BEFORE INIT VESA
        // sleep(10); 
    //     SB16_Init();
    //   uint16_t sampleRate = 44100;  // 44.1 kHz
    // uint8_t sampleSize = 16;      // 16-bit sample size
    //     uint32_t duration = 5000000;  // 5 seconds (in microseconds)

    //     // Calculate the number of samples based on sample rate and duration
    //     uint32_t numSamples = (sampleRate * duration) / 1000000;

    //     // Allocate memory for the sound data (assuming 16-bit samples)
    //     uint8_t* soundData = (uint8_t*)malloc(numSamples * (sampleSize / 8));

    //     // Generate a simple sine wave for the sound data
    //     for (uint32_t i = 0; i < numSamples; ++i) {
    //         double t = (double)i / sampleRate;
    //         double sineValue = 0.5 * sin(2.0 * PI * 440.0 * t);  // 440 Hz sine wave
    //         int16_t sample = (int16_t)(sineValue * INT16_MAX);
    //         memcpy(&soundData[i * (sampleSize / 8)], &sample, sampleSize / 8);
    //     }

    //     // Play the generated sound
    //     SB16_Write(0xD1); // turn speaker on if it's off
    //     SB16_Play(soundData, numSamples * (sampleSize / 8), 11025);

    //     // Free the allocated memory
    //     free(soundData);
    //     sleep(10);

        // if(strstr(output,"Memory Configuration") != NULL)
        // {
        //     //printf("\nFound initrid\n");
        //     get_adder_map(output);
        // }
        // else if (strstr(output,"INSTALL") != NULL)
        // {
           

        //      clear_screen();
        //     int size = getSizeInSectors(output);
        //     printf("%d\n",2000);
        //      clear_screen();
        //     //(0,1,size);
        //     printf("\n\nSHUT DOWN THE PC AND REMOVE THE INSTALL MEDIUM\n");
        //     for(;;);
        // }
        
        init_pci_device();
        const char *directory = "/sys/"; //Path to program directory
        program_count = find_programs(directory);
        printf("%d programs found\n",program_count);
        // printf("Programs:\n");
        char list[100][20];

    // Call the populate_list_from_filenames function to populate the list
    populate_list_from_filenames(list, 100);
    // printf("TEST UPDATE FUNCTION\n");
    // Print the contents of the list
    for (int i = 0; i < program_count; i++) {
        printf("Program %d: %s\n", i, list[i]);
    }
    // for (int i = 0; i < program_count; i++) {
    //     printf("%d: %s\n",i+1, programs[i]);
    // }
        // load_elf_file("/sys/shell");
        // clear_screen();
        // set_screen_x(0);
        // set_screen_y(0);
        // char **argv = (char**)kmalloc(2 * sizeof(char*));
        // argv[1] = (char*)kmalloc(strlen(get_cwd()) + 1);
        // strcpy(argv[1], get_cwd());
        // argv[0] = (char*)kmalloc(strlen("user") + 1);
        // strcpy(argv[0], "user");
        // printf("%s\n",argv[0]);
        // printf("%s\n",argv[1]);
        // pre_terminal(2,argv);
        // if(is_file("/tmp/AthenX.bin") == 0)
        // {

        //     FILE *install_file = fopen("/tmp/AthenX.bin", "r");
        //     FILE *main_file = fopen("/tmp/AthenX.bin", "r");
        //     size_t install_file_size = get_file_size(install_file);
        //     size_t main_file_size = get_file_size(main_file);
        //     char * install_file_buffer = (char *)kmalloc(install_file_size);
        //      char * main_file_buffer = (char *)kmalloc(main_file_size);
        //      if(install_file_buffer == NULL || main_file_buffer == NULL)
        //      {
        //         printf("malloc error");
        //      }
        //      else
        //      {
        //         fl_fread(install_file_buffer,sizeof(char),install_file_size,install_file);
        //         fl_fread(main_file_buffer,sizeof(char),main_file_size,main_file);
        //         if(install_file_buffer == main_file_buffer)
        //         {

        //         }
        //         else
        //         {
        //              printf("There is an update available. Type update to install\n");
        //         }
        //      }
        //     fclose(main_file);
        //     fclose(install_file);
           
        // }
        if (ret < 0) {
            ERROR("failed to init vesa graphics\n");
            sleep(4);
            display_init(1,0,0,0);
            terminal_main();
            goto done;
        }
        if (ret == 1) {
            terminal_main();
        } else {
 
            //printf("Terminal initialization (%d)",ret);
            //  init_paging();
            fpu_enable();
            // printf("\033[100;500m");
            asm("int $45");
            printf("Initialization complete\nAthenX %s:%d.%d.%d started successfully with %d errors\n",VERSION_MAIN,VERSION_SUB_MAIN,VERSION_SUB_MINOR,VERSION_SUB_PATCH,get_num_errors());
            printf("Welcome to AthenX-2.0. This is the default terminal, please run load <name> to execute your terminal or <name> to run a script\n");
            
            
            //char *dumbb = "DUMB";
            //beep();
            //dummy_start();
            //init_fat_32(0);
            
            //printf("made it");
            terminal_main();
        }

done:
        pmm_free_blocks(start, 256);
        pmm_deinit_region(g_kmap.available.start_addr, PMM_BLOCK_SIZE * 256);
    } else {
        printf_("error: invalid multiboot magic number\n");
    }
}




int login(int skip)
{
    set_scroll_mode(0);
    FILE *fp = fopen("/sec/user","r");
    if(get_file_size(fp) <= 1)
    {   
        fclose(fp);
        printf("NO USERS\n");
        char *new_home = "/home/tristan";
        // User *user;
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
                //crude_song();
                if(backspace(username))
                {
                    printf_("\b");
                    
                    //set_cursor_x(get_cursor_x()-2);
                    //printf_(" ");
                    //console_ungetchar();
                }
                else
                {
                    ///beep();
                }
                
                //printf_("\b");
            }
            
            else if (c == '\n')
            {
                // printf("\n");
                break;
                //printf_("\n");
                // undraw_square(get_screen_y(),get_screen_x());
                // cmd_handler(username,programs);
                // memset(buffer, 0,sizeof(buffer));
                
                //next_line();
                //printf("code");
                //set_screen_x(0);
                //set_terminal_colum(get_terminal_col()+16);
                //set_terminal_row(0);
                //  strcpy(cwd_at,get_cwd());
                // printf("\n%s>",cwd_at);
                //printf("looped\n");
                
                //crude_song();
            }
            // else if(c == '\n')
            // {
            //     undraw_square(get_screen_y(),get_screen_x());
            //     printf("\n");
            //     memset(buffer,0,sizeof(buffer));
            // }
            else if (c == '\0')
            {
                //DEBUG("NULL");
                
                //beep();
            }
            else if((int)c != 0x48 && (int)c != 0x50 && (int)c != 0x4D && (int)c != 0x4B )
            {
                
                char* s;
                s = ctos(s, c);
                //printf_(s);
                //undraw_square(get_screen_x(),get_screen_y());
                // printf_(s);
                // undraw_square(get_screen_y(),get_screen_x());
                printf(s);
                
                //printf_("X{}");
                //undraw_square(get_screen_x()-10,get_screen_y());
                //printf_(s);
                 
                 //printf_(s);
                //printf_(s);
                
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
                //crude_song();
                if(backspace(password))
                {
                    printf_("\b");
                    
                    //set_cursor_x(get_cursor_x()-2);
                    //printf_(" ");
                    //console_ungetchar();
                }
                else
                {
                    ///beep();
                }
                
                //printf_("\b");
            }
            
            else if (c == '\n')
            {
                // printf("\n");
                break;
                //printf_("\n");
                // undraw_square(get_screen_y(),get_screen_x());
                // cmd_handler(username,programs);
                // memset(buffer, 0,sizeof(buffer));
                
                //next_line();
                //printf("code");
                //set_screen_x(0);
                //set_terminal_colum(get_terminal_col()+16);
                //set_terminal_row(0);
                //  strcpy(cwd_at,get_cwd());
                // printf("\n%s>",cwd_at);
                //printf("looped\n");
                
                //crude_song();
            }
            // else if(c == '\n')
            // {
            //     undraw_square(get_screen_y(),get_screen_x());
            //     printf("\n");
            //     memset(buffer,0,sizeof(buffer));
            // }
            else if (c == '\0')
            {
                //DEBUG("NULL");
                
                //beep();
            }
            else if(c != '\0')
            {
                
                char* s;
                s = ctos(s, c);
                //printf_(s);
                //undraw_square(get_screen_x(),get_screen_y());
                // printf_(s);
                // undraw_square(get_screen_y(),get_screen_x());
                printf("*");
                
                //printf_("X{}");
                //undraw_square(get_screen_x()-10,get_screen_y());
                //printf_(s);
                 
                 //printf_(s);
                //printf_(s);
                
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
bool cursor_visible = true;

void toggle_cursor_visibility() {
    cursor_visible = !cursor_visible;
}


void set_init_path(char * path)
{
    strcpy(init_path,path);
}

int pre_terminal(int argc, char **argv)
{
    if(init_path != "\0")
    {
        load_elf_file(init_path,argc,argv);
    }
    else
    {
        return -1;
    }
    
}
void terminal_main()
{
    // //beep();
    // uint32 *vesa_buffer;
    // vesa_buffer = get_g_vbe_buffer();
//     struct flanterm_context *ft_ctx = flanterm_fb_simple_init(
//     vesa_buffer, vbe_get_width(), vbe_get_height(), get_pitch()
// );
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
    // if(attempts >= 9)
    // {
    //     printf("INVALID LOGIN, MAX ATTEMPTS\n");
    //     for(;;);

    // }
    
    
    // flanterm_write(ft_ctx, msg, sizeof(msg));
    //DEBUG("terminal_main");
    FUNC_ADDR_NAME(&terminal_main,0,"");
    //printf("HERE\n");
    // WINDOW *test_win;
    // test_win->color = VBE_RGB(0,255,125);
    // test_win->start_x = 500;
    // test_win->start_y = 10;
    // test_win->width = 300;
    // test_win->height = 100;
    // test_win->filled = false;
    //init_window(test_win);
    // ADDER_NAME_LIST test;
    // get_name_addr(&test);
    // printf_("\n%s : 0x%x",test.names,test.addr);
    // //printf_("0x%x",&terminal_main);
    // while(1)
    // {
    //     char buffer[512];
    //     char *shell = "\nUser@AthenX-2.0 ";
    //     printf_(shell);
    //     //printf_(cwd);
    //     printf_(">");
    //     memset(buffer, 0, sizeof(buffer));
    //     getstr_bound(buffer, strlen(shell));
    //     cmd_handler(buffer);
    // }
    //login(1);
    // #define LOGIN 0
    // #if LOGIN
    //     printf_("Login:\n");
    //     while (1 == 1)
    //     {
    //         if(login(0) == 1)
    //         {
    //             printf_("Login successful\n");
    //             break;
    //         }
    //         else{printf_("Invalid login\n");}
            
    //     }
    

    // #endif
    char cwd_at[FATFS_MAX_LONG_FILENAME] = "";
    strcpy(cwd_at,get_cwd());
    
     printf("%s>",cwd_at);
     //int x = 1/0;
     
     //printf_("{/330:0,255,0");
    //sleep(10);
    //printV("\nHeight%s\n",vbe_get_height());
    uint8_t byte;
    char buffer[512];
    memset(buffer,0,sizeof(buffer));
    buffer[0] = '\0';
    //DEBUG("HERE?");
    //beep();
    while(1)
    {    
            //beep();
           
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
             
            //printf_("%d",c);
            
           
            if(c == '\b')
            {
                //crude_song();
                if(backspace(buffer))
                {
                    printf_("\b");
                    
                    //set_cursor_x(get_cursor_x()-2);
                    //printf_(" ");
                    //console_ungetchar();
                }
                else
                {
                    ///beep();
                }
                
                //printf_("\b");
            }
            
            else if (c == '\n')
            {
                //printf_("\n");
                undraw_square(get_screen_y(),get_screen_x());
                cmd_handler(buffer,programs);
                memset(buffer, 0,sizeof(buffer));
                
                //next_line();
                //printf("code");
                //set_screen_x(0);
                //set_terminal_colum(get_terminal_col()+16);
                //set_terminal_row(0);
                 strcpy(cwd_at,get_cwd());
                printf("\n%s>",cwd_at);
                //printf("looped\n");
                
                //crude_song();
            }
            // else if(c == '\n')
            // {
            //     undraw_square(get_screen_y(),get_screen_x());
            //     printf("\n");
            //     memset(buffer,0,sizeof(buffer));
            // }
            else if (c == '\0')
            {
                //DEBUG("NULL");
                
                //beep();
            }
            else if((int)c != 0x48 && (int)c != 0x50 && (int)c != 0x4D && (int)c != 0x4B )
            {
                
                char* s;
                s = ctos(s, c);
                //printf_(s);
                //undraw_square(get_screen_x(),get_screen_y());
                // printf_(s);
                undraw_square(get_screen_y(),get_screen_x());
                printf(s);
                
                //printf_("X{}");
                //undraw_square(get_screen_x()-10,get_screen_y());
                //printf_(s);
                 
                 //printf_(s);
                //printf_(s);
                
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
            //undraw_square(get_screen_x(),get_screen_y());
            
             //printf_("CAY");
             
            
            
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
int fake_err(int z, int y)
{
    FUNC_ADDR_NAME(&fake_err,2,"ii");
    int o = 1893;
    int x = z/y;
    return x;
}

