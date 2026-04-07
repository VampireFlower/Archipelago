/* link.ld */
OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc)
/* ENTRY(entry) */

MEMORY
{
    xfb (rwx) : ORIGIN = 0x80127f60, LENGTH = 0xA020
}

SECTIONS
{
    . = ORIGIN(ram);

    .text : ALIGN(4)
    {
        *(.text)
        *(.text.*)
    } > xfb

    .rodata : ALIGN(4)
    {
        *(.rodata)
        *(.rodata.*)
    } > xfb

    .data : ALIGN(4)
    {
        *(.data)
        *(.data.*)
    } > xfb

    .bss (NOLOAD) : ALIGN(4)
    {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    } > xfb



    malloc    = 0x8001a670;
    free      = 0x8001a85c;
    OSReport  = 0x8001d20c;
    add_to_render_queue = 0x8001e924;

    _restgpr_29_x = 0x8002e20c;

    strcpy    = 0x80072600;
    strlen    = 0x80072678;
    strncmp   = 0x800726a0;
    memmove   = 0x80072710;
    memcmp    = 0x80072798;
    memset    = 0x800727d0;
    memcpy    = 0x800727f8;
}