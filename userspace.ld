SECTIONS
{
    .text 0x51000 : 
    {
        _start = .;
        *(.text)
    }

    . = ALIGN(4096);
    .rodata :
    {
        *(.rodata)
    }

    . = ALIGN(4096);
    .data :
    {
        *(.data)
    }

    . = ALIGN(4096);
    /* .bss section */
    .bss :
    {
        _bss_start = .;
        *(.bss)
        _bss_end = .;
    }
}
