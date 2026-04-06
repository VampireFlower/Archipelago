/* link.ld */
OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc)
/* ENTRY(entry) */

MEMORY
{
    /* Example fixed load address for your blob */
    ram (rwx) : ORIGIN = 0x80480000, LENGTH = 0x00008C00
}

SECTIONS
{
    . = ORIGIN(ram);

    .text : ALIGN(4)
    {
        *(.text)
        *(.text.*)
    } > ram

    .rodata : ALIGN(4)
    {
        *(.rodata)
        *(.rodata.*)
    } > ram

    .data : ALIGN(4)
    {
        *(.data)
        *(.data.*)
    } > ram

    .bss (NOLOAD) : ALIGN(4)
    {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    } > ram

    malloc    = ABSOLUTE(0x8001a670);
    free      = ABSOLUTE(0x8001a85c);
    OSReport  = ABSOLUTE(0x8001d20c);

    _restgpr_29_x = ABSOLUTE(0x8002e20c);

    strcpy    = ABSOLUTE(0x80072600);
    strlen    = ABSOLUTE(0x80072678);
    strncmp   = ABSOLUTE(0x800726a0);
    memmove   = ABSOLUTE(0x80072710);
    memcmp    = ABSOLUTE(0x80072798);
    memset    = ABSOLUTE(0x800727d0);
    memcpy    = ABSOLUTE(0x800727f8);
}