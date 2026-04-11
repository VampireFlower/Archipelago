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

_restgpr_   = 0x8002e264;
_restgpr_15 = 0x8002e268;
_restgpr_16 = 0x8002e26c;
_restgpr_17 = 0x8002e270;
_restgpr_18 = 0x8002e274;
_restgpr_19 = 0x8002e278;
_restgpr_20 = 0x8002e27c;
_restgpr_21 = 0x8002e280;
_restgpr_22 = 0x8002e284;
_restgpr_23 = 0x8002e288;
_restgpr_24 = 0x8002e28c;
_restgpr_25 = 0x8002e290;
_restgpr_25 = 0x8002e294;
_restgpr_26 = 0x8002e298;
_restgpr_27 = 0x8002e29c;
_restgpr_28 = 0x8002e2a0;
_restgpr_29 = 0x8002e2a4;
_restgpr_30 = 0x8002e2a8;
_restgpr_31 = 0x8002e2ac;

_restgpr_x    = 0x8002e30c;
_restgpr_15_x = 0x8002e310;
_restgpr_16_x = 0x8002e314;
_restgpr_17_x = 0x8002e318;
_restgpr_18_x = 0x8002e31c;
_restgpr_19_x = 0x8002e320;
_restgpr_20_x = 0x8002e324;
_restgpr_21_x = 0x8002e328;
_restgpr_22_x = 0x8002e32c;
_restgpr_23_x = 0x8002e330;
_restgpr_24_x = 0x8002e334;
_restgpr_25_x = 0x8002e338;
_restgpr_26_x = 0x8002e33c;
_restgpr_27_x = 0x8002e340;
_restgpr_28_x = 0x8002e344;
_restgpr_29_x = 0x8002e348;
_restgpr_30_x = 0x8002e34c;
_restgpr_31_x = 0x8002e350;



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

