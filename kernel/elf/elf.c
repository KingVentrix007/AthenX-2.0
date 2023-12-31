#include "elf.h"
#include "fat_filelib.h"
#include "printf.h"
#include "kheap.h"

char* read_txt_section(const char* file_path) {
    FILE* file = fopen(file_path, "rb");

    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Read the ELF header
    Elf64_Ehdr elf_header;
    fread(&elf_header, sizeof(Elf64_Ehdr), 1, file);

    // Check if it's a valid ELF file
    if (elf_header.e_ident[EI_MAG0] != ELFMAG0 || elf_header.e_ident[EI_MAG1] != ELFMAG1 ||
        elf_header.e_ident[EI_MAG2] != ELFMAG2 || elf_header.e_ident[EI_MAG3] != ELFMAG3) {
        printf("Not a valid ELF file\n");
        fclose(file);
        return NULL;
    }

    // Find the section header string table
    fseek(file, elf_header.e_shoff + elf_header.e_shentsize * elf_header.e_shstrndx, SEEK_SET);
    Elf64_Shdr shstrtab_header;
    fread(&shstrtab_header, sizeof(Elf64_Shdr), 1, file);

    // Read the section names
    char* section_names = malloc(shstrtab_header.sh_size);
    fseek(file, shstrtab_header.sh_offset, SEEK_SET);
    fread(section_names, shstrtab_header.sh_size, 1, file);

    // Find the .txt section
    Elf64_Shdr txt_section_header;
    int txt_section_found = 0;

    for (int i = 0; i < elf_header.e_shnum; i++) {
        fseek(file, elf_header.e_shoff + i * elf_header.e_shentsize, SEEK_SET);
        fread(&txt_section_header, sizeof(Elf64_Shdr), 1, file);

        if (txt_section_header.sh_type == SHT_PROGBITS && strcmp(section_names + txt_section_header.sh_name, ".txt") == 0) {
            txt_section_found = 1;
            break;
        }
    }

    // Read the contents of the .txt section
    char* txt_section_contents = NULL;

    if (txt_section_found) {
        txt_section_contents = malloc(txt_section_header.sh_size);
        fseek(file, txt_section_header.sh_offset, SEEK_SET);
        fread(txt_section_contents, txt_section_header.sh_size, 1, file);
    } else {
        printf("No .txt section found\n");
    }

    // Clean up and close the file
    free(section_names);
    fclose(file);

    return txt_section_contents;
}