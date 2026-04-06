/* link.ld */
OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc)
ENTRY(entry)

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

    /* Fixed-address function supplied by the host game / main.dol */
    OSReport = ABSOLUTE(0x800056E0);
}