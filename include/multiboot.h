#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "../include/types.h"

#define MULTIBOOT_MAGIC_HEADER      0x1BADB002
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
