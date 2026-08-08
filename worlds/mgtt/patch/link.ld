INCLUDE auto_symbols.ld

OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc)

MEMORY
{
    ram (rwx) : ORIGIN = 0x80539680, LENGTH = 0x16980
}

REGION_ALIAS("freespace", ram)

SECTIONS
{
    . = ORIGIN(freespace);

    .text : ALIGN(4)
    {
        *(.text)
        *(.text.*)
    } > freespace

    .rodata : ALIGN(4)
    {
        *(.rodata)
        *(.rodata.*)
    } > freespace

    .data : ALIGN(4)
    {
        *(.data)
        *(.data.*)
    } > freespace

    .bss (NOLOAD) : ALIGN(4)
    {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    } > freespace

}

recording = .;


/* FUNCTIONS */


cosf = 0x80009828;
sinf = 0x80021810;

hypotf = 0x8001a038;
hypot3f = 0x8001a07c;
atan2f = 0x8001a0c4;

malloc_from = 0x8001a8bc;
malloc = 0x8001a904;
free = 0x8001aa98;

GetHeapID = 0x8001ab70;
GetRandomInt = 0x8001ab98;
GetRandomFloat = 0x8001acfc;


PSVECMag = 0x800400c8;
PSVECSubtract = 0x8003fe84;
PSVECNormalize = 0x8003ffb4;
PSVECDotProduct = 0x8004013c;

CopyFlyingToRest = 0x8041532c;



ResolveCameraCollision = 0x80459188;



/* DATA */

camera = 0x8026fbc0;

BallRestSimulated = 0x80514be8;

ShotReplayCount = 0x8051a16c;
TotalBallFlightDuration = 0x80514c80;
TRY_ACTION_CUP_CAMERA = 0x80523fb4;
TRY_BEHIND_CUP_CAMERA = 0x80523fb0;