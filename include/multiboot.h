#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "../include/types.h"
//!IGNORE THIS, IS HERE FOR PAGING ONLY
#define MULTIBOOT_SEARCH                        8192
#define MULTIBOOT_HEADER_ALIGN                  4

/* The magic field should contain this. */
#define MULTIBOOT_HEADER_MAGIC                  0x1BADB002

/* This should be in %eax. */
#define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002

/* Alignment of multiboot modules. */
#define MULTIBOOT_MOD_ALIGN                     0x00001000

/* Alignment of the multiboot info structure. */
#define MULTIBOOT_INFO_ALIGN                    0x00000004

/* Flags set in the 'flags' member of the multiboot header. */

/* Align all boot modules on i386 page (4KB) boundaries. */
#define MULTIBOOT_PAGE_ALIGN                    0x00000001

/* Must pass memory information to OS. */
#define MULTIBOOT_MEMORY_INFO                   0x00000002

/* Must pass video information to OS. */
#define MULTIBOOT_VIDEO_MODE                    0x00000004

/* This flag indicates the use of the address fields in the header. */
#define MULTIBOOT_AOUT_KLUDGE                   0x00010000

/* Flags to be set in the 'flags' member of the multiboot info structure. */

/* is there basic lower/upper memory information? */
#define MULTIBOOT_INFO_MEMORY                   0x00000001
/* is there a boot device set? */
#define MULTIBOOT_INFO_BOOTDEV                  0x00000002
/* is the command-line defined? */
#define MULTIBOOT_INFO_CMDLINE                  0x00000004
/* are there modules to do something with? */
#define MULTIBOOT_INFO_MODS                     0x00000008

/* These next two are mutually exclusive */

/* is there a symbol table loaded? */
#define MULTIBOOT_INFO_AOUT_SYMS                0x00000010
/* is there an ELF section header table? */
#define MULTIBOOT_INFO_ELF_SHDR                 0X00000020

/* is there a full memory map? */
#define MULTIBOOT_INFO_MEM_MAP                  0x00000040

/* Is there drive info? */
#define MULTIBOOT_INFO_DRIVE_INFO               0x00000080

/* Is there a config table? */
#define MULTIBOOT_INFO_CONFIG_TABLE             0x00000100

/* Is there a boot loader name? */
#define MULTIBOOT_INFO_BOOT_LOADER_NAME         0x00000200

/* Is there a APM table? */
#define MULTIBOOT_INFO_APM_TABLE                0x00000400

/* Is there video information? */
#define MULTIBOOT_INFO_VBE_INFO                 0x00000800
#define MULTIBOOT_INFO_FRAMEBUFFER_INFO         0x00001000
#define LOADBASE                     0x100000

// Define the base address of the kernel (after paging is enabled) - this define is used by the linker (See Linker->Advanced in project properties)
#define \
BASE_ADDRESS 0xC0000000
//BASE_ADDRESS 0x100000
//! OKAY YOU CAN TAKE NOTICE NOW
#define MULTIBOOT_MAGIC_HEADER      0x1BADB002
#define MULTIBOOT_MAGIC_HEADER_2    0xE85250D6
#define MULTIBOOT_BOOTLOADER_MAGIC  0x2BADB002

/* The Multiboot header. */
typedef struct {
    uint32 magic;
    uint32 flags;
    uint32 checksum;
    uint32 header_addr;
    uint32 load_addr;
    uint32 load_end_addr;
    uint32 bss_end_addr;
    uint32 entry_addr;
} MULTIBOOT_HEADER;

/* The symbol table for a.out. */
typedef struct {
    uint32 tabsize;
    uint32 strsize;
    uint32 addr;
    uint32 reserved;
} AOUT_SYMBOL_TABLE;

/* The section header table for ELF. */
typedef struct {
    uint32 num;
    uint32 size;
    uint32 addr;
    uint32 shndx;
} ELF_SECTION_HEADER_TABLE;

typedef struct __attribute__((packed)){
      uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;

} MULTIBOOT_INFO;


typedef enum {
    MULTIBOOT_MEMORY_AVAILABLE = 1,
    MULTIBOOT_MEMORY_RESERVED,
    MULTIBOOT_MEMORY_ACPI_RECLAIMABLE,
    MULTIBOOT_MEMORY_NVS,
    MULTIBOOT_MEMORY_BADRAM
} MULTIBOOT_MEMORY_TYPE;

typedef struct {
    uint32 size;
    uint32 addr_low;
    uint32 addr_high;
    uint32 len_low;
    uint32 len_high;
    MULTIBOOT_MEMORY_TYPE type;
} MULTIBOOT_MEMORY_MAP;

#endif
