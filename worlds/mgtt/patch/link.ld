INCLUDE auto_symbols.ld

OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc)
/* ENTRY(entry) */

MEMORY
{
    ram (rwx) : ORIGIN = 0x805247b4, LENGTH = 0x2B84C
    xfb (rwx) : ORIGIN = 0x80127f60, LENGTH = 0xA020
    eva (rwx) : ORIGIN = 0x800014B0, LENGTH = 592
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

}

cos = 0x80009618;

malloc_from = 0x8001a628;
malloc      = 0x8001a670;
free        = 0x8001a85c;
OSReport    = 0x8001d20c;
add_to_render_queue = 0x8001e924;


sin = 0x8002157c;


TextBlockStateMachine = 0x80024698;
TextBlockDelete       = 0x80024c98;
TextBlockConfigure    = 0x80024cfc;
TextBlockActivate     = 0x80024e04;
TextBlockDrawGlyphs   = 0x80027ecc;



_savefpr_    = 0x8002e184;
_savefpr_14  = 0x8002e184;
_savefpr_15  = 0x8002e188;
_savefpr_16  = 0x8002e18c;
_savefpr_17  = 0x8002e190;
_savefpr_18  = 0x8002e194;
_savefpr_19  = 0x8002e198;
_savefpr_20  = 0x8002e19c;
_savefpr_21  = 0x8002e1a0;
_savefpr_22  = 0x8002e1a4;
_savefpr_23  = 0x8002e1a8;
_savefpr_24  = 0x8002e1ac;
_savefpr_25  = 0x8002e1b0;
_savefpr_26  = 0x8002e1b4;
_savefpr_27  = 0x8002e1b8;
_savefpr_28  = 0x8002e1bc;
_savefpr_29  = 0x8002e1c0;
_savefpr_30  = 0x8002e1c4;
_savefpr_31  = 0x8002e1c8;

_savegpr_    = 0x8002e1d0;
_savegpr_14  = 0x8002e1d0;
_savegpr_15  = 0x8002e1d4;
_savegpr_16  = 0x8002e1d8;
_savegpr_17  = 0x8002e1dc;
_savegpr_18  = 0x8002e1e0;
_savegpr_19  = 0x8002e1e4;
_savegpr_20  = 0x8002e1e8;
_savegpr_21  = 0x8002e1ec;
_savegpr_22  = 0x8002e1f0;
_savegpr_23  = 0x8002e1f4;
_savegpr_24  = 0x8002e1f8;
_savegpr_25  = 0x8002e1fc;
_savegpr_26  = 0x8002e200;
_savegpr_27  = 0x8002e204;
_savegpr_28  = 0x8002e208;
_savegpr_29  = 0x8002e20c;
_savegpr_30  = 0x8002e210;
_savegpr_31  = 0x8002e214;

_restfpr_    = 0x8002e21c;
_restfpr_14  = 0x8002e21c;
_restfpr_15  = 0x8002e220;
_restfpr_16  = 0x8002e224;
_restfpr_17  = 0x8002e228;
_restfpr_18  = 0x8002e22c;
_restfpr_19  = 0x8002e230;
_restfpr_20  = 0x8002e234;
_restfpr_21  = 0x8002e238;
_restfpr_22  = 0x8002e23c;
_restfpr_23  = 0x8002e240;
_restfpr_24  = 0x8002e244;
_restfpr_25  = 0x8002e248;
_restfpr_26  = 0x8002e24c;
_restfpr_27  = 0x8002e250;
_restfpr_28  = 0x8002e254;
_restfpr_29  = 0x8002e258;
_restfpr_30  = 0x8002e25c;
_restfpr_31  = 0x8002e260;

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
_restgpr_26 = 0x8002e294;
_restgpr_27 = 0x8002e298;
_restgpr_28 = 0x8002e29c;
_restgpr_29 = 0x8002e2a0;
_restgpr_30 = 0x8002e2a4;
_restgpr_31 = 0x8002e2a8;

_restfpr_x    = 0x8002e2b4;
_restfpr_14_x = 0x8002e2b4;
_restfpr_15_x = 0x8002e2b8;
_restfpr_16_x = 0x8002e2bc;
_restfpr_17_x = 0x8002e2c0;
_restfpr_18_x = 0x8002e2c4;
_restfpr_19_x = 0x8002e2c8;
_restfpr_20_x = 0x8002e2cc;
_restfpr_21_x = 0x8002e2d0;
_restfpr_22_x = 0x8002e2d4;
_restfpr_23_x = 0x8002e2d8;
_restfpr_24_x = 0x8002e2dc;
_restfpr_25_x = 0x8002e2e0;
_restfpr_26_x = 0x8002e2e4;
_restfpr_27_x = 0x8002e2e8;
_restfpr_28_x = 0x8002e2ec;
_restfpr_29_x = 0x8002e2f0;
_restfpr_30_x = 0x8002e2f4;
_restfpr_31_x = 0x8002e2f8;

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


TextBlocks = 0x802cc050;


ShotReplayCount = 0x805052a4;