OUTPUT_FORMAT("elf32-i386");
ENTRY(main);

SECTIONS {
    . = 9M; /* Set the load address to 1MB (adjust as needed) */

    .text : {
        *(.text)

    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    /* Add any additional sections here as needed */

    /DISCARD/ : {
        *(.eh_frame)
        *(.comment)
    }
}