#include "vesa.h"
#include "vfs.h"
#include "maths.h"
#include "console.h"
#include "kernel.h"
#include "display.h"
#include "panic.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "kheap.h"
#include "multiboot.h"
#include "pmm.h"
#include "string.h"
#include "ide.h"
#include "keyboardmap.h"
#include "types.h"
#include "cmdhandler.h"
#include "timer.h"
#include "fs.h"
#include "graphics.h"
#include "fpu.h"
#include "ext2.h"
KERNEL_MEMORY_MAP g_kmap;

void access_grub_module(MULTIBOOT_INFO* mbi) {
    if (!(mbi->flags & 0x00000008))
    printf("No Grub modules\n");
        return; // Module information not available

    uint32_t mods_count = mbi->mods_count;
    uint32_t mods_addr = mbi->mods_addr;

    struct module* modules = (struct module*)mods_addr;

    // Assuming you want to access the first module loaded by Grub
    if (mods_count >= 1) {
        uint32_t mod_start = modules[0].mod_start;
        uint32_t mod_end = modules[0].mod_end;
        printf("There are %d modules", mods_count);

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

    kmap->system.total_memory = mboot_info->mem_low + mboot_info->mem_high;

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
    printf("kernel:\n");
    printf("  kernel-start: 0x%x, kernel-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.k_start_addr, kmap->kernel.k_end_addr, kmap->kernel.k_len);
    printf("  text-start: 0x%x, text-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.text_start_addr, kmap->kernel.text_end_addr, kmap->kernel.text_len);
    printf("  data-start: 0x%x, data-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.data_start_addr, kmap->kernel.data_end_addr, kmap->kernel.data_len);
    printf("  rodata-start: 0x%x, rodata-end: 0x%x, TOTAL: %d\n",
           kmap->kernel.rodata_start_addr, kmap->kernel.rodata_end_addr, kmap->kernel.rodata_len);
    printf("  bss-start: 0x%x, bss-end: 0x%x, TOTAL: %d\n",
           kmap->kernel.bss_start_addr, kmap->kernel.bss_end_addr, kmap->kernel.bss_len);

    printf("total_memory: %d KB\n", kmap->system.total_memory);
    printf("available:\n");
    printf("  start_adddr: 0x%x\n  end_addr: 0x%x\n  size: %d\n",
           kmap->available.start_addr, kmap->available.end_addr, kmap->available.size);
}

void kmain(unsigned long magic, unsigned long addr) {
    MULTIBOOT_INFO *mboot_info;
    
    char *shell = "User@SimpleOS ";
    char *cwd = malloc(sizeof(*cwd)); //Current working directory, in it's path
    cwd = "/";
    char *pwd = "/"; //Previous working directory absolute path
    gdt_init();
    idt_init();
   
    //
    //ata_get_drive_by_model
    display_init(1,0,0,32);
     //timer_init();
     ata_init();
     //mouse_init();
    //console_init(COLOR_WHITE, COLOR_BLACK);

    keyboard_init();
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        mboot_info = (MULTIBOOT_INFO *)addr;
        memset(&g_kmap, 0, sizeof(KERNEL_MEMORY_MAP));
        if (get_kernel_memory_map(&g_kmap, mboot_info) < 0) {
            printf("error: failed to get kernel memory map\n");
            return;
        }
        // put the memory bitmap at the start of the available memory
        pmm_init(g_kmap.available.start_addr, g_kmap.available.size);
        // initialize atleast 1MB blocks of memory for our heap
        pmm_init_region(g_kmap.available.start_addr, PMM_BLOCK_SIZE * 256);
        // initialize heap 256 blocks(1MB)
        void *start = pmm_alloc_blocks(256);
        void *end = start + (pmm_next_free_frame(1) * PMM_BLOCK_SIZE);
        kheap_init(start, end);
        //@ Gets screen size from memory
        
        const int DRIVE = 0;
        const uint32 LBA = KERNEL_SECTOR_BASE+1;
        const uint8 NO_OF_SECTORS = 1;
        char buf[ATA_SECTOR_SIZE] = {0};
        struct screen_size{
            char checksum[32];
            int x;
            int y;
        };

            struct screen_size sc_size;
         memset(buf, 0, sizeof(buf));
        ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);
        memcpy(&sc_size, buf, sizeof(sc_size));
        int x;
        int y;
        if(strcmp(sc_size.checksum,"True") == 0)
        {
            x = sc_size.x;
            y = sc_size.y;
        }
        else
        {
            //cmd_handler("screen size");
            x = 1280;
            y = 1024;
        }

        int ret = display_init(0,x,y,32);
        
        char* mode = logo();
        set_screen_x(0);
        set_screen_y(0);
        clear_screen();
        if(strcmp(mode,"b") != 0)
        {
            printf("more boot options coming soon\n");
        }
        printf("%s\n",mode);
        cmd_handler("cls");
        //vbe_print_available_modes();
        initialize_file_system(0);
        init_alega_fs(0);
        
        //run_once();
        // #define CUSTOM_FS 0
        // #if CUSTOM_FS
        access_grub_module(mboot_info);
           
            
        //     //clean_fs_partition_table_main(46);
        //     //fs_partition_table_main_p();
        //     //run_once();
        // #else
        //     //printf("hello world");
       
            
            
        //#endif
        print_drives();
        // char *filedata = malloc(sizeof(*filedata));
        // char * path = "/test";
        // filedata = ext2_read_file(path);
        // printf("%s\n", filedata);
        // uint32 ino = ext2_path_to_inode(cwd);
        // char **names = ext2_ls(ino);
        // printf("\n");
        //fs_partition_table_main_p();
    //     fl_init();

    // // Attach media access functions to library
    // if (fl_attach_media(ide_read_sectors_fat, ide_write_sectors_fat) != FAT_INIT_OK)
    // {
    //     printf("ERROR: Failed to init file system\n");
    //     int i = fl_format(100,"file");
    //     //printf("\nI: %d", i);
    //     //return -1;
    // }
    // fl_listdirectory("/");
        //run_once();
        //kernel_command_handler("login set");
         //printf("X: %d, Y: %d",sc_size.x,sc_size.y);
        // for (size_t f1 = 0; f1 < 4; f1++)
        // {
        //     printf("%d",buf[f1]-'0');
        //     x[f1] = buf[f1]-'0';
        // }
        //  for (size_t f2 = 0; f2 < 4; f2++)
        // {
        //     printf("%d",buf[f2+5]-'0');
        //     y[f2] = buf[f2+5]-'0';
        // }
        // printf("\n");
        // printf("%d",x);
        // printf("\n");
        // printf("%d",y);
        
        
        timer_init();//!DO NOT PUT BEFORE INIT VESA
        
        

        if (ret < 0) {
            ERROR("failed to init vesa graphics\n");
            sleep(4);
            display_init(1,0,0,0);
            terminal_main();
            goto done;
        }
        if (ret == 1) {
            // scroll to top
            // for(int i = 0; i < MAXIMUM_PAGES; i++)
            //     console_scroll(SCROLL_UP);

            // while (1) {
            //     // add scrolling to view all modes
            //     char c = kb_get_scancode();
            //     if (c == SCAN_CODE_KEY_UP)
            //         console_scroll(SCROLL_UP);
            //     if (c == SCAN_CODE_KEY_DOWN)
            //         console_scroll(SCROLL_DOWN);
            // }
        } else {
            // fill some colors
    //          int rows, cols;
    
    // // Inline assembly to get the screen size using INT 10h BIOS interrupt
    //         asm("mov $0x13, %%ah \n"    // AH = 0x13 (function 13h - Get Video Mode Information)
    //             "int $0x10 \n"         // Call BIOS interrupt 0x10
    //             "mov %%ax, %0 \n"      // Store the number of rows in 'rows' variable
    //             "mov %%bx, %1"         // Store the number of columns in 'cols' variable
    //             : "=r" (rows), "=r" (cols) // Outputs
    //             :                        // Inputs (none)
    //             : "%ax", "%bx"           // Clobbered registers
    //         );

    //         printf("Screen size: %dx%d\n", cols, rows);
            fpu_enable();
            terminal_main();
            // uint32 x = 0;
            // for (uint32 c = 0; c < 267; c++) {
            //     for (uint32 i = 0; i < 600; i++) {
            //         vbe_putpixel(x, i, VBE_RGB(c % 255, 0, 0));
            //     }
            //     x++;
            // }
            // for (uint32 c = 0; c < 267; c++) {
            //     for (uint32 i = 0; i < 600; i++) {
            //         vbe_putpixel(x, i, VBE_RGB(0, c % 255, 0));
            //     }
            //     x++;
            // }
            // for (uint32 c = 0; c < 267; c++) {
            //     for (uint32 i = 0; i < 600; i++) {
            //         vbe_putpixel(x, i, VBE_RGB(0, 0, c % 255));
            //     }
            //     x++;
            // }
        }

done:
        pmm_free_blocks(start, 256);
        pmm_deinit_region(g_kmap.available.start_addr, PMM_BLOCK_SIZE * 256);
    } else {
        printf("error: invalid multiboot magic number\n");
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
            printf("Username>");
                while (1==1)
                {
                    //printf(">");
                    char c = kb_getchar();
                    char* s;
                    if (c == '\n')
                    {
                            printf("\n");
                            
                            
                            break;
                            //crude_song();
                    }
                    else
                    {
                        
                        
                        
                        s = ctos(s, c);
                        //printf(s);
                        printf(s);
                        //printf(".");
                        //printf(s);
                        
                        append(username,c);
                    }
                    
                }
                printf("Password>");
                while (1==1)
                {
                    char c = kb_getchar();
                    if (c == '\n')
                    {
                            printf("\n");
                            
                            
                            break;
                            //crude_song();
                    }
                    else
                    {
                        
                        
                        char* s;
                        s = ctos(s, c);
                        //printf(s);
                        //printf(s);
                        printf("#");
                        
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
void terminal_main()
{
   
    //login(1);
    #define LOGIN 0
    #if LOGIN
        printf("Login:\n");
        while (1 == 1)
        {
            if(login(0) == 1)
            {
                printf("Login successful\n");
                break;
            }
            else{printf("Invalid login\n");}
            
        }
    

    #endif
    
     printf(">");
    //sleep(10);
    //printV("\nHeight%s\n",vbe_get_height());
    uint8_t byte;
    char *buffer[512];
     buffer[0] = '\0';
    // int posx = 0;
    // int posy = 0;
    
        // printChar(posx,posy,'h');
        //       posx = posx +1;
        //       posy = posy +0;
    while(1 == 1)
    {    
            //beep();
            char c = kb_getchar();
            if(c == '\b')
            {
                //crude_song();
                if(backspace(buffer))
                {
                    printf("\b");
                    //set_cursor_x(get_cursor_x()-2);
                    //printf(" ");
                    //console_ungetchar();
                }
                else
                {
                    beep();
                }
                
                //printf("\b");
            }
            else if (c == '\n')
            {
                //printf("\n");
                cmd_handler(buffer);
                memset(buffer, 0,sizeof(buffer));
                next_line();
                set_screen_x(0);
                //set_terminal_colum(get_terminal_col()+16);
                //set_terminal_row(0);
                printf(">");
                //crude_song();
            }
            
            else
            {
                
                char* s;
                s = ctos(s, c);
                //printf(s);
                printf(s);
                //printf(s);
                
                append(buffer,c);
            }
            
            
    }
    
}