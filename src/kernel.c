#include "vesa_display.h"
#include "serial.h"
#include "debug.h"
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

// void find_initramfs_location(MULTIBOOT_INFO *mb_info) {
//     if (mb_info->flags & (1 << 3)) {
//         uint32_t initramfs_start = mb_info->initramfs_start;
//         uint32_t initramfs_end = mb_info->initramfs_end;

//         // Print the initramfs location
//         printf("Initramfs is loaded at addresses: 0x%X - 0x%X\n", initramfs_start, initramfs_end);
//     } else {
//         printf("Initramfs location not found in Multiboot info\n");
//     }
// }


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
void get_map(KERNEL_MEMORY_MAP *out)
{
    memcpy(&out, &g_kmap, sizeof(out));
}
void kmain(unsigned long magic, unsigned long addr) {
    MULTIBOOT_INFO *mboot_info;
     
    gdt_init();
    idt_init();
   
    //
    //ata_get_drive_by_model
    kassert(display_init(1,0,0,32),0,1);
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        mboot_info = (MULTIBOOT_INFO *)addr;
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
        void *start = pmm_alloc_blocks(256);
        void *end = start + (pmm_next_free_frame(1) * PMM_BLOCK_SIZE);
        kheap_init(start, end);
        //@ Gets screen size from memory
        
        ata_init();
        int x = 1280;
        int y = 1024;
        
        keyboard_init();
        kassert(init_serial(DEFAULT_COM_DEBUG_PORT),0,2);
        int ret = kassert(display_init(0,x,y,32),0,1);
        
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
        //printf("%s,%d,%s",__FILE__,__LINE__,__FUNCTION__);
        timer_init();//!DO NOT PUT BEFORE INIT VESA
        print_drives();
        // FUNC_ADDR_NAME(&kmain);
        // ADDER_NAME_LIST test;
        // get_name_addr(&test);
        // printf("\n%s : 0x%x",test.names,test.addr);
        
        
        
        

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
 
            fpu_enable();

            terminal_main();
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
void getstr_bound(char *buffer, uint8 bound) {
    if (!buffer) return;
    while(1) {
        char ch = kb_getchar();
        if (ch == '\n') {
            printf("\n");
            return ;
        } else if(ch == '\b') {
            if(backspace(*buffer))
            {
                printf("\b");
                
            }

        } else {
            *buffer++ = ch;
            printf("%c", ch);
        }
    }
}


void terminal_main()
{
    DEBUG("terminal_main");
    FUNC_ADDR_NAME(&terminal_main);
    // ADDER_NAME_LIST test;
    // get_name_addr(&test);
    // printf("\n%s : 0x%x",test.names,test.addr);
    // //printf("0x%x",&terminal_main);
    // while(1)
    // {
    //     char buffer[512];
    //     char *shell = "\nUser@AthenX-2.0 ";
    //     printf(shell);
    //     //printf(cwd);
    //     printf(">");
    //     memset(buffer, 0, sizeof(buffer));
    //     getstr_bound(buffer, strlen(shell));
    //     cmd_handler(buffer);
    // }
    //login(1);
    // #define LOGIN 0
    // #if LOGIN
    //     printf("Login:\n");
    //     while (1 == 1)
    //     {
    //         if(login(0) == 1)
    //         {
    //             printf("Login successful\n");
    //             break;
    //         }
    //         else{printf("Invalid login\n");}
            
    //     }
    

    // #endif
    
     printf(">");
    //sleep(10);
    //printV("\nHeight%s\n",vbe_get_height());
    uint8_t byte;
    char *buffer[512];
     buffer[0] = '\0';
    
    while(1)
    {    
            //beep();
            draw_square_cursor(get_screen_x(),get_screen_y(),VBE_RGB(255, 255, 255));
            char c = kb_getchar();
            undraw_square(get_screen_x(),get_screen_y());
           
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
                //undraw_square(get_screen_x()-10,get_screen_y());
                printf(s);
                 
                 //printf(s);
                //printf(s);
                
                append(buffer,c);
                
            }
            //undraw_square(get_screen_x(),get_screen_y());
            
             //printf("CAY");
            
            
    }
    
}