

#include "../include/fat_filelib.h"
#include "../include/fat32.h"
#include "../include/installer.h"
#include "../include/errno.h"
#include "../include/version.h"
#include "../include/pci.h"
#include "../include/ethernet.h"
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
#include "../include/syscall.h"
#include "virt.h"
#include "../include/exe.h"
#include "../include/elf_exe.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "../include/stb_image.h"
KERNEL_MEMORY_MAP g_kmap;
MULTIBOOT_INFO *multi_boot_info;
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
    MacAddress osMac = read_mac_address();
     print_mac_address(&osMac);
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
void kmain(unsigned long magic, unsigned long addr) {
    FUNC_ADDR_NAME(&kmain,2,"ui");
    MULTIBOOT_INFO *mboot_info;
     
    gdt_init();
    idt_init();
    //
    //ata_get_drive_by_model
    kassert(display_init(1,0,0,32),0,1);
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        mboot_info = (MULTIBOOT_INFO *)addr;
        //multi_boot_info = mboot_info;
        memset(&g_kmap, 0, sizeof(KERNEL_MEMORY_MAP));
        if (get_kernel_memory_map(&g_kmap, mboot_info) < 0) {
            printf("error: failed to get kernel memory map\n");
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
        void *start = pmm_alloc_blocks(20);
        void *end = start + (pmm_next_free_frame(1) * PMM_BLOCK_SIZE);
        kheap_init(start, end);
        
        //@ Gets screen size from memory
        
        
        int x = 1280;
        int y = 768;
        
         keyboard_init();
        //kassert(init_serial(DEFAULT_COM_DEBUG_PORT),0,2);
        int ret = display_init(0,x,y,32);
        printf("\nTotal memory size = %d",end-start);
        ata_init();
        clear_screen();
        set_screen_x(0);
        set_screen_y(0);
        init_virt();
        printf("kernel memory start address = %p\n",mboot_info->addr);
         fl_init();
          if (fl_attach_media(ide_read_sectors_fat, ide_write_sectors_fat) != FAT_INIT_OK)
        {
            printf("ERROR: Failed to init file system\n");
            //return -1;
        }
        else
        {
            
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
            "JUNK=FILES"
            // Add more environmental variables as needed.
        };

        int saveResult = saveEnvironmentVariables(filename, env_vars, sizeof(env_vars) / sizeof(env_vars[0]));
        
        if (saveResult == 0) {
            printf("Environmental variables saved successfully.\n");
        } else {
            printf("Error saving environmental variables.\n");
        }
        }
         if (loadAndDrawImage("/gui/sunset.tga", 0, 0) == 0) {
        printf("succsess\n");
        set_screen_x(0);
        set_screen_y(0);
        } else {
            // Error occurred
        }
       
        //'fl_listdirectory("/");
         printf("Mod count: %d\n", mboot_info->mods_count);
        uint32_t initrd_size = 0;
        uint8_t* initrd_location = locate_initrd(mboot_info, &initrd_size);
        uint8_t* initrd_end_location = initrd_location + initrd_size;
        printf("Initrd found at %x - %x (%d bytes)\n", initrd_location, initrd_end_location, initrd_size);
        char output[71629] = {0};
        memcpy(output, initrd_location, initrd_size);
        unsigned char* bios_start = (unsigned char*)0xE0000;
            unsigned int bios_length = 0x20000;  // 128 KB

            struct XSDP_t* rsdp = find_rsdp(bios_start, bios_length);

            if (rsdp) {
                printf("RSDP version %d found at address: 0x%p\n",rsdp->Revision, rsdp);
                //printf("Version: %d\n",rsdp->Revision);
            
                // Access other information in the RSDP as needed
            } else {
                printf("RSDP not found.\n");
            }
        // perror("this is a test");
        // uint8_t* initrd_location2 = locate_initrd(mboot_info, &initrd_size);
        // uint8_t* initrd_end_location2 = initrd_location2 + initrd_size;
        // printf("Initrd found at %x - %x (%d bytes)\n", initrd_location2, initrd_end_location2, initrd_size);
        //int boot_device = mboot_info->boot_device;
        // display_init(0,1280,1024,32);
        timer_init();//!DO NOT PUT BEFORE INIT VESA
        //sleep(100000);
        // char* cmdline = ((uint32_t*)mboot_info->cmdline);
        // while (*cmdline != '\0') {
        // // Output or store the character (e.g., using a serial port or video buffer)
        // // ...
        // printf("%c",cmdline);
        // cmdline++;
        // }
        if(strstr(output,"Memory Configuration") != NULL)
        {
            //printf("\nFound initrid\n");
            get_adder_map(output);
        }
        else if (strstr(output,"INSTALL") != NULL)
        {
           
            // printf("HERE");
            // tui_main(1,"install");
             clear_screen();
            int size = getSizeInSectors(output);
            printf("%d\n",2000);
             clear_screen();
            //(0,1,size);
            printf("\n\nSHUT DOWN THE PC AND REMOVE THE INSTALL MEDIUM\n");
            for(;;);
        }
         
        //isr_register_interrupt_handler((IRQ_BASE+10),sys_handler);
       //isr_register_interrupt_handler((IRQ_BASE+11),int_print);
       
    
        // Call your custom system call with the desired arguments
        
        
        //printf("Custom syscall result: %d\n", result);

        //char* mode = logo();
        //  printf("HELLO");
        // set_screen_x(0);
        // set_screen_y(0);
        // clear_screen();
        // if(strcmp(mode,"u") == 0)
        // {
        //    clear_screen();
        //     tui_main(0,"");
        // }
        // printf("boot %d\n",mboot_info->boot_device);
       
        //printf_("%s\n",mode);
        //cmd_handler("cls");
        
       
        // clear_screen();
        // tui_main(0,"");
       
        //ext2_init();
        //read_root_directory_inode();
        //read_root_directory_inode();
         //clear_screen();
         //vbe_print_available_modes();
        //printf_("%s,%d,%s",__FILE__,__LINE__,__FUNCTION__);
        
        //sleep(190000);
        //print_drives();
        //DEBUG(" ");
        //init_fs();
        //printf_("hello");
        //init_ssfs();
        partition_table tb;
        //format_disk(0);
        //init_fs();
        //write_file(40);
        HBA_MEM* hba_mem_ptr = (HBA_MEM*)HBA_MEM_BASE_ADDRESS;
        // printf("SATA\n");
        // setup(hba_mem_ptr);
        // hba_mem_ptr->ports[PORT_NUM].clb = 0x2000000;
        // hba_mem_ptr->ports[PORT_NUM].fb  = 0x3000000;

        // hba_mem_ptr->ports[PORT_NUM].cmd = (1 << 4);
        // //while ((hba_mem_ptr->ports[PORT_NUM].cmd & ((1<<15))) == 0);
        // hba_mem_ptr->ports[PORT_NUM].cmd = 1;

        // debug_HBA_MEM(hba_mem_ptr);
        //testRead(hba_mem_ptr);
        init_pci_device();
       // printf("%s\n",__TIME__);
        //printf("GOT HERE");
       
         //printf("H\n");
         //printf("0x%16x\n",&kmain);
       
         //printf("T\n");
         //printf("0x%x\n",addr);
         //printf("MADE IT HERE");
        //init_e82540EM_ethernet_card();
        //printf_("CMP:");
        // MacAddress osMac = read_mac_address();
        // printf("\nMac Address: ");
        // print_mac_address(&osMac);
        // MacAddress eepMAc = read_eep_mac();
        // printf("Eep Address: ");
        // print_mac_address(&eepMAc);
        //printf_("DONE");
        //sleep(2000);
        //access_grub_module(mboot_info);
        // IMAGE img;
        // img.ptrs.ptr = &athenx_logo;
        // img.Bpp = 8;
        // img.width = 1000;
        // img.height = 700;
        // strcpy(img.name,"Low resolution logo image" );
        // //draw_low_res_img(img);
        // IMAGE bg_img;
        
        // char** loaded_vars = NULL; // Adjust the size as needed.
        // int numLoaded = loadEnvironmentVariables(filename, loaded_vars, 10);
        

       

        //f_chdir("rootfs/");
        // FILE *file;
        // file = fl_fopen("/rootfs/cat.txt", "w");

        // file = fl_fopen("/grub/grub.cfg", "r");
        // if (file == NULL) {
        //     printf("Error opening file");
        // }

        //  char ch;
        //  while ((ch = fgetc(file)) != EOF) {
        //     printf("%c",ch); // Print each character
        // }
        //  fclose(file);
        //  printf("\n");
        // bg_img.ptrs.ptr = &background;
        // bg_img.Bpp = 8;
        // bg_img.width = 1980;
        // bg_img.height = 1080;
        // strcpy(bg_img.name, "Low resolution background image" );
        //printf("\n%d\n",sizeof(background)/512);
        //char buffer_test[2073600] = {0};
        //DEBUG("HERE");
        //memset(buffer_test, 0, sizeof(buffer_test));
        //memcpy(buffer_test,background,sizeof(buffer_test));
        //DEBUG("HERE 2");
        //ide_write_sectors(0,(2073600/512),900,(uint32)buffer_test);
        //draw_low_res_img(bg_img);
        //draw_hi_res_img(1980,1080);
        //sleep(10000);
        //ext2_init();
        // FUNC_ADDR_NAME(&kmain);
        // ADDER_NAME_LIST test;
        // get_name_addr(&test);
        // printf_("\n%s : 0x%x",test.names,test.addr);
        
        
        
        // char *path = "/sys/shell";
        // load_elf_file(path);
        // ProgramEntry programs[MAX_PROGRAMS];
        ProgramEntry programs[MAX_PROGRAMS];
        int program_count = 0;
        const char *directory = "/sys/"; //Path to program directory
        find_programs(directory,programs,&program_count);
        printf("%d programs found\n",program_count);
        // printf("Programs:\n");
       printf("ELF Programs in %s:\n", directory);
    for (int i = 0; i < program_count; i++) {
        printf("%d: %s\n",i+1, programs[i].name);
    }
        load_elf_file("/sys/shell");
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
            fpu_enable();
            printf("Initialization complete\nAthenX %s:%d.%d.%d started successfully with %d errors\n",VERSION_MAIN,VERSION_SUB_MAIN,VERSION_SUB_MINOR,VERSION_SUB_PATCH,get_num_errors());
            
            
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
    struct login_info
    {
        char checksum[32];
        char username_stored[32];
        char password_stored[32];
    };
    const int DRIVE = 0;
    const uint32 LBA = KERNEL_SECTOR_BASE+3;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};
    

    struct login_info login_i;
    memset(buf, 0, sizeof(buf));
    ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);
    memcpy(&login_i, buf, sizeof(login_i));
    if(strcmp(login_i.checksum,"True") == 0)
    {
        char *username[512];
        username[0] = '\0';
        char *password[512];
        password[0] = '\0';
        if(skip == 0)
        {
            printf_("Username>");
                while (1==1)
                {
                    //printf_(">");
                    char c = kb_getchar();
                    char* s;
                    if (c == '\n')
                    {
                            printf_("\n");
                            
                            
                            break;
                            //crude_song();
                    }
                    else
                    {
                        
                        
                        
                        s = ctos(s, c);
                        //printf_(s);
                        printf_(s);
                        //printf_(".");
                        //printf_(s);
                        
                        append(username,c);
                    }
                    
                }
                printf_("Password>");
                while (1==1)
                {
                    char c = kb_getchar();
                    if (c == '\n')
                    {
                            printf_("\n");
                            
                            
                            break;
                            //crude_song();
                    }
                    else
                    {
                        
                        
                        char* s;
                        s = ctos(s, c);
                        //printf_(s);
                        //printf_(s);
                        printf_("#");
                        
                        append(password,c);
                    }
                    
                }
                int login_username_true = 0;
                int login_password_true = 0;
                if(strcmp(username,login_i.username_stored) == 0)
                {
                    login_username_true = 1;
                }
                else
                {
                    return -1;
                }
                if (strcmp(password,login_i.password_stored) == 0)
                {
                    login_password_true = 1;
                }
                else
                {
                    return -1;
                }
                if(login_username_true == 1 && login_password_true == 1)
                {
                    return 1;
                }
                else{return -1;}
        }
        else{return 89;}
   
    

    }
    else{kernel_command_handler("login set");}
    
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

void terminal_main()
{
    //beep();
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
                cmd_handler(buffer);
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

