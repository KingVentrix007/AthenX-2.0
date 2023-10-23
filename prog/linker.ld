OUTPUT_FORMAT("elf32-i386");
ENTRY(main);
STACK_SIZE = 4K;
SECTIONS {
   
    . = 15M; /* Set the load address to 1MB (adjust as needed) */

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
    stack :
    {
        PROVIDE (_stack_start = .);
        *(.stack)
        . += STACK_SIZE;
        PROVIDE (_stack_end = .);
    }
}