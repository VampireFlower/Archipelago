INCLUDE hook_symbols.ld

OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc)

MEMORY /* list of known available freespace */
{
    
    /* https://mariokartwii.com/showthread.php?tid=1106 */
    eva  (rwx) : ORIGIN = 0x800014C0, LENGTH = 0x240
    
    /* the first XFB is 32 lines too long. 
    it is 640*480 when the game is 640*448 */
    xfb  (rwx) : ORIGIN = 0x80127f60, LENGTH = 0xA020
    
    /*the space between the end of BSS of the largest overlay and the XFB */
    ram  (rwx) : ORIGIN = 0x805247c0, LENGTH = 0x2B840
    
    /* the game carves many arenas out of a large heap allocation.
    there is space left over in the parent OS heap it uses */
    heap (rwx) : ORIGIN = 0x816a00a0, LENGTH = 0xAFF60
}

REGION_ALIAS("freespace", ram)
recording = ORIGIN(xfb);


SECTIONS
{
    . = ORIGIN(freespace);

    ctors = .; /* so C code knows its address */

    .ctors : ALIGN(4)
    {
        KEEP(*(.ctors))
        KEEP(*(.ctors.*))
        LONG(0)
    } > freespace


/* program never exits, so destructors never run
    dtors = .;

    .dtors : ALIGN(4)
    {
        KEEP(*(.dtors))
        KEEP(*(.dtors.*))
        LONG(0)
    } > freespace

    . += 4;
*/

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


/* FUNCTIONS */


PlaySoundEffect = 0x80011b88;

cosf   = 0x80009618;

hypotf  = 0x80019da4;
hypot3f = 0x80019de8;
atan2f  = 0x80019e30;

SetCurrentArena = 0x8001a5c4;

malloc_from = 0x8001a628;
malloc      = 0x8001a670;
free        = 0x8001a85c;

GetCurrentArena = 0x8001a8c8;
GetRandomInt    = 0x8001a8f0;
GetRandomFloat  = 0x8001aa54;


sprintf = 0x8001c480;

OSReport    = 0x8001d20c;
add_to_render_queue = 0x8001e924;
ProjectOnScreen = 0x8001ebac;

sinf = 0x8002157c;


TextBlockStateMachine = 0x80024698;
TextBlockDelete       = 0x80024c98;
TextBlockConfigure    = 0x80024cfc;
TextBlockCreate       = 0x80024e04;
TextBlockDrawGlyphs   = 0x80027ecc;


OSAllocFromHeap = 0x80032320;

PSMTXIdentity  = 0x8003c354;
PSMTXCopy      = 0x8003c3ec;
PSMTXConcat    = 0x8003c6c8;
PSMTXTranspose = 0x8003c9f0;
PSMTXInverse   = 0x8003ccec;
PSMTXInvXpose  = 0x8003d000;
PSMTXMultVec   = 0x8003df20;
PSMTXMultVecSR = 0x8003e280;

C_MTXOrtho = 0x8003e684;

PSVECAdd            = 0x8003fb4c;
PSVECSubtract       = 0x8003fba4;
PSVECScale          = 0x8003fbf0;
PSVECNormalize      = 0x8003fcd4;
PSVecSquareMag      = 0x8003fd3c;
PSVECMag            = 0x8003fde8;
PSVECDotProduct     = 0x8003fe5c;
PSVECCrossProduct   = 0x8003fec8;
PSVECSquareDistance = 0x800400ec;
PSVECDistance       = 0x800401c0;





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


strcpy  = 0x80072600;
strlen  = 0x80072678;
strncmp = 0x800726a0;
memmove = 0x80072710;
memcmp  = 0x80072798;
memset  = 0x800727d0;
memcpy  = 0x800727f8;
tanf    = 0x80072828;
acosf   = 0x80072940;
asinf   = 0x80072a28;

CopyRestToFlying = 0x80414970;
CopyFlyingToRest = 0x80414a1c;


ResolveCameraCollision = 0x804557d4;


/* DATA */
camera = 0x8026c7c0;

TextBlocks = 0x802cc050;


__OSCurrHeap = 0x802d7b10;



DrawShotEndText = 0x80431610;

BallRestSimulated       = 0x80502768;
TotalBallFlightDuration = 0x80502800;

ShotReplayCount = 0x805052a4;

SecondaryCamera = 0x80505408;

FlyingCameraAngle = 0x8050f128;
TRY_BEHIND_CUP_CAMERA = 0x8050f14c;
TRY_ACTION_CUP_CAMERA = 0x8050f150;