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
    . = ORIGIN(xfb);

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

}


malloc    = 0x8001a670;
free      = 0x8001a85c;
OSReport  = 0x8001d20c;
add_to_render_queue = 0x8001e924;


TextBlockDelete    = 0x80024c98;
TextBlockConfigure = 0x80024cfc;
TextBlockCreate    = 0x80024e04;
TextBlockDraw      = 0x80027ecc;


_restgpr_29_x = 0x8002e20c;


PSMTXIdentity = 0x8003c354;
C_MTXOrtho = 0x8003e684;


strcpy    = 0x80072600;
strlen    = 0x80072678;
strncmp   = 0x800726a0;
memmove   = 0x80072710;
memcmp    = 0x80072798;
memset    = 0x800727d0;
memcpy    = 0x800727f8;


GXSetTexCoordGen2 = 0x80068814;
GXSetNumTexGens   = 0x80068a38;
GXSetCullMode     = 0x8006982c;
GXSetNumChans     = 0x8006afc4;
GXSetChanCtrl     = 0x8006afe8;
GXSetTevOrder     = 0x8006dba0;
GXSetNumTevStages = 0x8006dd18;
GXSetBlendMode    = 0x8006e154;
GXSetZMode        = 0x8006e23c;
GXSetProjection   = 0x8006fcc0;
GXLoadPosMtxImm   = 0x8006fe10;

