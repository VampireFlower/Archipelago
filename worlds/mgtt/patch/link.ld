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

recording = ORIGIN(xfb);

REGION_ALIAS("freespace", ram)

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

/* DOL */
PlaySoundEffect = 0x80011b88;

cosf   = 0x80009618;

ClearFlag = 0x80018b28;
SetFlag   = 0x80018b50;
GetFlag   = 0x80018b78;


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



/* GOLF */
CopyRestToFlying = 0x80414970;
CopyFlyingToRest = 0x80414a1c;

ResolveCameraCollision = 0x804557d4;




/* DATA */
camera = 0x8026c7c0;

TextBlocks = 0x802cc050;

__OSCurrHeap = 0x802d7b10;


/* GOLF */
DrawShotEndText = 0x80431610;

BallRestSimulated       = 0x80502768;
TotalBallFlightDuration = 0x80502800;

ShotReplayCount = 0x805052a4;

SecondaryCamera = 0x80505408;

FlyingCameraAngle = 0x8050f128;
TRY_BEHIND_CUP_CAMERA = 0x8050f14c;
TRY_ACTION_CUP_CAMERA = 0x8050f150;


/* MENU */
MAIN_MENU_BUTTON_COUNT  = 0x80445db8;
MAIN_MENU_BUTTON_LAYOUT = 0x8044c328;







/* __start.o from os.a */
__check_pad3       = 0x80003100;
__start            = 0x80003140;
__copy_rom_section = 0x80003278;
__init_bss_section = 0x800032d4; memclear = 0x800032d4;
__init_registers   = 0x80003304;
__init_data        = 0x80003320;
/* __ppc_eabi_init.o section .init */
__init_hardware    = 0x80003570;
__flush_cache      = 0x80003590;


cosf   = 0x80009618;


AssetJobReconcile        = 0x8000981c; _ZN8AssetJob9ReconcileEv = 0x8000981c;
AssetJob                 = 0x80009828; _ZN8AssetJobC1Ev = 0x80009828;
DecompressFileChunk      = 0x8000a16c;
CancelAndNotifyLoaders   = 0x8000a31c; _ZN8AssetJob22CancelAndNotifyLoadersEv = 0x8000a31c;
NotDone                  = 0x8000b524; _ZN8AssetJob7NotDoneE8JobStateS0_ = 0x8000b524;
SubmitLow                = 0x8000b56c; _ZN8AssetJob9SubmitLowEv = 0x8000b56c;
SubmitHigh               = 0x8000b5d8; _ZN8AssetJob10SubmitHighEv = 0x8000b5d8;
isCurrentLoaderCancelled = 0x8000a2cc;
SuspendLoaders           = 0x8000ba78;
SubmitPrefetchJob        = 0x8000baac;

LoadFileIntoMemory = 0x8000b214;


PlaySoundEffect = 0x80011b88;




ManageCamera = 0x800179f0;
SetupMainLoop = 0x80018004;

ResetGame = 0x80018338;
ResetGameThreadFunc = 0x80018344;

ClearFlag = 0x80018b28;
SetFlag   = 0x80018b50;
GetFlag   = 0x80018b78;

RunFrame  = 0x80018ed0;

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

UpdateControllerFromPad = 0x8001aaa4;
UpdateControllerRepeat = 0x8001af60;


TranslateFileIDToPath = 0x8001bbf0;

LoadFileIntoMemoryWrapper = 0x8001bcd0;

sprintf  = 0x8001c480;
tolower  = 0x8001d1c4;
vsprintf = 0x8001d784;

OSReport = 0x8001d20c;

/* important for decomp */
DEMOEnableGPHangWorkaround = 0x8001fe10;
__DEMODiagnoseHang         = 0x80020088;

add_to_render_queue = 0x8001e924;
ProjectOnScreen = 0x8001ebac;

sinf = 0x8002157c;

TBRSAllocate          = 0x80022564; _ZN13TBRenderState8AllocateEv = 0x80022564;
TBRSFreeGlyphs        = 0x80022708; _ZN13TBRenderState10FreeGlyphsEv = 0x80022708;
TBASAllocate          = 0x80022790; _ZN16TBAnimationState8AllocateEv = 0x80022790;
AssessLayout          = 0x80023320; _ZN9TextBlock12AssessLayoutEPv = 0x80023320;
TextBlockDeleteAll    = 0x800243c0;
TextBlockStateMachine = 0x80024698;
TBRSFreeImageData     = 0x80024c2c; _ZN13TBRenderState13FreeImageDataEv = 0x80024c2c;
TextBlockDelete       = 0x80024c98;
TextBlockConfigure    = 0x80024cfc; _ZN9TextBlock9ConfigureEiiii = 0x80024cfc;
TextBlockCreate       = 0x80024e04; _ZN9TextBlock6CreateEssiiii = 0x80024e04;
TextBlockSpawnMessage = 0x80024f1c; _ZN9TextBlock12SpawnMessageEisshi = 0x80024f1c;
TBRSDrawGlyphs        = 0x80027ecc; _ZN13TBRenderState10DrawGlyphsEPA4_fib = 0x80027ecc;

LoadFileModule = 0x80028240;

_savefpr_    = 0x8002e184; _savegpr_    = 0x8002e1d0; _restfpr_    = 0x8002e21c; _restgpr_   = 0x8002e264; _restfpr_x    = 0x8002e2b4; _restgpr_x    = 0x8002e30c;
_savefpr_14  = 0x8002e184; _savegpr_14  = 0x8002e1d0; _restfpr_14  = 0x8002e21c; _restgpr_14 = 0x8002e264; _restfpr_14_x = 0x8002e2b4; _restgpr_14_x = 0x8002e30c;
_savefpr_15  = 0x8002e188; _savegpr_15  = 0x8002e1d4; _restfpr_15  = 0x8002e220; _restgpr_15 = 0x8002e268; _restfpr_15_x = 0x8002e2b8; _restgpr_15_x = 0x8002e310;
_savefpr_16  = 0x8002e18c; _savegpr_16  = 0x8002e1d8; _restfpr_16  = 0x8002e224; _restgpr_16 = 0x8002e26c; _restfpr_16_x = 0x8002e2bc; _restgpr_16_x = 0x8002e314;
_savefpr_17  = 0x8002e190; _savegpr_17  = 0x8002e1dc; _restfpr_17  = 0x8002e228; _restgpr_17 = 0x8002e270; _restfpr_17_x = 0x8002e2c0; _restgpr_17_x = 0x8002e318;
_savefpr_18  = 0x8002e194; _savegpr_18  = 0x8002e1e0; _restfpr_18  = 0x8002e22c; _restgpr_18 = 0x8002e274; _restfpr_18_x = 0x8002e2c4; _restgpr_18_x = 0x8002e31c;
_savefpr_19  = 0x8002e198; _savegpr_19  = 0x8002e1e4; _restfpr_19  = 0x8002e230; _restgpr_19 = 0x8002e278; _restfpr_19_x = 0x8002e2c8; _restgpr_19_x = 0x8002e320;
_savefpr_20  = 0x8002e19c; _savegpr_20  = 0x8002e1e8; _restfpr_20  = 0x8002e234; _restgpr_20 = 0x8002e27c; _restfpr_20_x = 0x8002e2cc; _restgpr_20_x = 0x8002e324;
_savefpr_21  = 0x8002e1a0; _savegpr_21  = 0x8002e1ec; _restfpr_21  = 0x8002e238; _restgpr_21 = 0x8002e280; _restfpr_21_x = 0x8002e2d0; _restgpr_21_x = 0x8002e328;
_savefpr_22  = 0x8002e1a4; _savegpr_22  = 0x8002e1f0; _restfpr_22  = 0x8002e23c; _restgpr_22 = 0x8002e284; _restfpr_22_x = 0x8002e2d4; _restgpr_22_x = 0x8002e32c;
_savefpr_23  = 0x8002e1a8; _savegpr_23  = 0x8002e1f4; _restfpr_23  = 0x8002e240; _restgpr_23 = 0x8002e288; _restfpr_23_x = 0x8002e2d8; _restgpr_23_x = 0x8002e330;
_savefpr_24  = 0x8002e1ac; _savegpr_24  = 0x8002e1f8; _restfpr_24  = 0x8002e244; _restgpr_24 = 0x8002e28c; _restfpr_24_x = 0x8002e2dc; _restgpr_24_x = 0x8002e334;
_savefpr_25  = 0x8002e1b0; _savegpr_25  = 0x8002e1fc; _restfpr_25  = 0x8002e248; _restgpr_25 = 0x8002e290; _restfpr_25_x = 0x8002e2e0; _restgpr_25_x = 0x8002e338;
_savefpr_26  = 0x8002e1b4; _savegpr_26  = 0x8002e200; _restfpr_26  = 0x8002e24c; _restgpr_26 = 0x8002e294; _restfpr_26_x = 0x8002e2e4; _restgpr_26_x = 0x8002e33c;
_savefpr_27  = 0x8002e1b8; _savegpr_27  = 0x8002e204; _restfpr_27  = 0x8002e250; _restgpr_27 = 0x8002e298; _restfpr_27_x = 0x8002e2e8; _restgpr_27_x = 0x8002e340;
_savefpr_28  = 0x8002e1bc; _savegpr_28  = 0x8002e208; _restfpr_28  = 0x8002e254; _restgpr_28 = 0x8002e29c; _restfpr_28_x = 0x8002e2ec; _restgpr_28_x = 0x8002e344;
_savefpr_29  = 0x8002e1c0; _savegpr_29  = 0x8002e20c; _restfpr_29  = 0x8002e258; _restgpr_29 = 0x8002e2a0; _restfpr_29_x = 0x8002e2f0; _restgpr_29_x = 0x8002e348;
_savefpr_30  = 0x8002e1c4; _savegpr_30  = 0x8002e210; _restfpr_30  = 0x8002e25c; _restgpr_30 = 0x8002e2a4; _restfpr_30_x = 0x8002e2f4; _restgpr_30_x = 0x8002e34c;
_savefpr_31  = 0x8002e1c8; _savegpr_31  = 0x8002e214; _restfpr_31  = 0x8002e260; _restgpr_31 = 0x8002e2a8; _restfpr_31_x = 0x8002e2f8; _restgpr_31_x = 0x8002e350;

/* os.a */
/* OS.o */
__OSIsDebuggerPresent     = 0x80030d6c;
__OSFPRInit               = 0x80030d78;
DisableWriteGatherPipe    = 0x80030dfc;
OSGetConsoleType          = 0x80030e24;
ClearArena                = 0x80030e4c;
InquiryCallback           = 0x80030f74;
OSInit                    = 0x80030fb0;
OSExceptionInit           = 0x80031328;
__OSDBIntegrator          = 0x800315a8;
__OSDBJump                = 0x800315cc;
__OSSetExceptionHandler   = 0x800315d0;
__OSGetExceptionHandler   = 0x800315ec;
OSExceptionVector         = 0x80031600;
__DBVECTOR                = 0x80031658;
__OSEVSetNumber           = 0x80031668;
__OSEVEnd                 = 0x80031698;
OSDefaultExceptionHandler = 0x8003169c;
__OSPSInit                = 0x800316f4;
__OSGetDIConfig           = 0x8003172c;
OSRegisterVersion         = 0x80031740;

/* OSAddress.o is missing */

/* OSAlarm.o */
OSCheckAlarmQueue            = 0x8003176c;
SetTimer                     = 0x800318a8;
OSInitAlarm                  = 0x80031940;
OSCreateAlarm                = 0x8003198c;
InsertAlarm                  = 0x8003199c;
OSSetAlarm                   = 0x80031bec;
OSSetAbsAlarm                = 0x80031c54;
OSSetPeriodicAlarm           = 0x80031cc4;
OSCancelAlarm                = 0x80031d40;
DecrementerExceptionCallback = 0x80031e5c;
DecrementerExceptionHandler  = 0x8003208c;
OSSetAlarmTag                = 0x800320dc;
OSCancelAlarms               = 0x800320e4;
/* OnReset is not present */

/* OSAlloc.o */
DLAddFront        = 0x80032194;
DLLookup          = 0x800321b4;
DLExtract         = 0x800321d4;
DLInsert          = 0x80032208;
DLOverlap         = 0x800322b4;
DLSize            = 0x800322fc;
OSAllocFromHeap   = 0x80032320;
OSAllocFixed      = 0x8003241c;
OSFreeToHeap      = 0x800326a0;
OSSetCurrentHeap  = 0x8003271c;
OSInitAlloc       = 0x8003272c;
OSCreateHeap      = 0x8003279c;
OSDestroyHeap     = 0x80032808;
OSAddToHeap       = 0x8003281c;
OSCheckHeap       = 0x8003287c;
OSReferentSize    = 0x80032bdc;
OSDumpHeap        = 0x80032be8;
OSVisitAllocated  = 0x80032cec;

/* OSArena.o */
OSGetArenaHi       = 0x80032d88;
OSGetArenaLo       = 0x80032d90;
OSSetArenaHi       = 0x80032d98;
OSSetArenaLo       = 0x80032da0;
OSAllocFromArenaLo = 0x80032da8;
OSAllocFromArenaHi = 0x80032dd4;

/* OSAudioSystem.o */
__OSInitAudioSystem = 0x80032df4;
__OSStopAudioSystem = 0x80032fb0;

/* OSCache.o */
DCFlashInvalidate   = 0x80033088;
DCEnable            = 0x80033098;
DCDisable           = 0x800330ac;
DCFreeze            = 0x800330c0;
DCUnfreeze          = 0x800330d4;
DCTouchLoad         = 0x800330e4;
DCBlockZero         = 0x800330ec;
DCBlockStore        = 0x800330f4;
DCBlockFlush        = 0x800330fc;
DCBlockInvalidate   = 0x80033104;
DCInvalidateRange   = 0x8003310c;
DCFlushRange        = 0x80033138;
DCStoreRange        = 0x80033168;
DCFlushRangeNoSync  = 0x80033198;
DCStoreRangeNoSync  = 0x800331c4;
DCZeroRange         = 0x800331f0;
DCTouchRange        = 0x8003321c;
ICInvalidateRange   = 0x80033248;
ICFlashInvalidate   = 0x8003327c;
ICEnable            = 0x8003328c;
ICDisable           = 0x800332a0;
ICFreeze            = 0x800332b4;
ICUnfreeze          = 0x800332c8;
ICBlockInvalidate   = 0x800332d8;
ICSync              = 0x800332e0;
__LCEnable          = 0x800332e8;
LCEnable            = 0x800333b4;
LCDisable           = 0x800333ec;
LCAllocOneTag       = 0x80033414;
LCAllocTags         = 0x80033428;
LCLoadBlocks        = 0x80033468;
LCStoreBlocks       = 0x8003348c;
LCAlloc             = 0x800334b0;
LCAllocNoInvalidate = 0x80033520;
LCLoadData          = 0x80033590;
LCStoreData         = 0x8003363c;
LCQueueLength       = 0x800336e8;
LCQueueWait         = 0x800336f4;
LCFlushQueue        = 0x80033708;
L2Init              = 0x80033750;
L2Enable            = 0x8003379c;
L2Disable           = 0x800337c8;
L2GlobalInvalidate  = 0x800337f8;
L2SetDataOnly       = 0x80033890;
L2SetWriteThrough   = 0x800338d0;
DMAErrorHandler     = 0x80033910;
__OSCacheInit       = 0x80033a70;

/* OSContext.o */
__OSLoadFPUContext  = 0x80033b64;
__OSSaveFPUContext  = 0x80033c88;
OSLoadFPUContext    = 0x80033db0;
OSSaveFPUContext    = 0x80033db8;
OSSetCurrentContext = 0x80033dc0;
OSGetCurrentContext = 0x80033e1c;
OSSaveContext       = 0x80033e28;
OSLoadContext       = 0x80033ea8;
OSGetStackPointer   = 0x80033f80;
OSSwitchStack       = 0x80033f88;
OSSwitchFiber       = 0x80033f98;
OSClearContext      = 0x80033fc8;
OSInitContext       = 0x80033fec;
OSDumpContext       = 0x800340a8;
OSSwitchFPUContext  = 0x80034350;
__OSContextInit     = 0x800343d4;
OSFillFPUContext    = 0x8003441c;

/* OSError.o */
PROVIDE(OSReport = 0x80034548);
OSVReport              = 0x800345c8;
OSPanic                = 0x800345e8;
OSSetErrorHandler      = 0x80034714;
__OSUnhandledException = 0x8003492c;

/* OSExec.o is missing */
/* OSFatal.o is missing */
/* OSFont.o is missing */

/* OSInterrupt.o */
OSDisableInterrupts      = 0x80034c14;
OSEnableInterrupts       = 0x80034c28;
OSRestoreInterrupts      = 0x80034c3c;
__OSSetInterruptHandler  = 0x80034c60;
__OSGetInterruptHandler  = 0x80034c7c;
__OSInterruptInit        = 0x80034c90;
SetInterruptMask         = 0x80034d04;
OSGetInterruptMask       = 0x80034fdc;
OSSetInterruptMask       = 0x80034fe8;
__OSMaskInterrupts       = 0x80035078;
__OSUnmaskInterrupts     = 0x80035100;
__OSDispatchInterrupt    = 0x80035188;
ExternalInterruptHandler = 0x800354cc;

/* OSLink.o */
OSNotifyLink     = 0x8003551c;
OSNotifyUnlink   = 0x80035520;
OSSetStringTable = 0x80035524;
Relocate         = 0x80035530;
Link             = 0x800357ec;
OSLink           = 0x80035acc;
OSLinkFixed      = 0x80035af0;
Undo             = 0x80035b2c;
OSUnlink         = 0x80035d64;
__OSModuleInit   = 0x80035f38;
OSSearchModule   = 0x80035f50;

/* OSMessage.o */
OSInitMessageQueue = 0x80035ff0;
OSSendMessage      = 0x80036050;
OSReceiveMessage   = 0x80036118;
OSJamMessage       = 0x800361f4;

/* OSMemory.o */
OSGetPhysicalMemSize         = 0x800362c8;
OSGetConsoleSimulatedMemSize = 0x800362d4;
OnReset                      = 0x800362e0;
MEMIntrruptHandler           = 0x8003631c;
OSProtectRange               = 0x80036388;
Config24MB                   = 0x8003644c;
Config48MB                   = 0x800364cc;
RealMode                     = 0x8003654c;
__OSInitMemoryProtection     = 0x80036564;

/* OSMutex.o */
OSInitMutex        = 0x80036684;
OSLockMutex        = 0x800366bc;
OSUnlockMutex      = 0x80036798;
__OSUnlockAllMutex = 0x80036860;
OSTryLockMutex     = 0x800368d0;
OSInitCond         = 0x8003698c;
OSWaitCond         = 0x800369ac;
OSSignalCond       = 0x80036a80;
IsMember           = 0x80036aa0;
__OSCheckMutex     = 0x80036acc;
__OSCheckDeadLock  = 0x80036bcc;
__OSCheckMutexes   = 0x80036c04;

/* OSReboot.o? */
IsStreamEnabled  = 0x80036c78;
Run              = 0x80036cb0;
ReadApploader    = 0x80036cc0;
Callback         = 0x80036d84;
__OSReboot       = 0x80036d90;
OSSetSaveRegion  = 0x800370d0;
OSGetSaveRegion  = 0x800370dc;
OSGetSavedRegion = 0x800370f0;

/* OSReset.o */
OSRegisterResetFunction   = 0x80037104;
OSUnregisterResetFunction = 0x80037188;
__OSCallResetFunctions    = 0x800371c0;
KillThreads               = 0x800372bc;
__OSDoHotReset            = 0x80037324;
/* __OSShutdownDevices is not present */
OSResetSystem             = 0x8003736c;
OSGetResetCode            = 0x800375e4;
/* OSSetBootDol is not present */

/* OSResetSW.o */
__OSResetSWInterruptHandler = 0x80037614;
OSSetResetCallback          = 0x80037708;
OSGetResetButtonState       = 0x8003777c;
OSGetResetSwitchState       = 0x80037a14;
__OSSetResetButtonTimer     = 0x80037a34;

/* OSRtc.o */
GetRTC               = 0x80037a90;
__OSGetRTC           = 0x80037ba8;
__OSSetRTC           = 0x80037dd4;
ReadSram             = 0x80037edc;
WriteSramCallback    = 0x80037ff8;
WriteSram            = 0x80038058;
__OSInitSram         = 0x80038170;
LockSram             = 0x800382a4;
__OSLockSram         = 0x8003830c;
__OSLockSramEx       = 0x80038368;
UnlockSram           = 0x800383c4;
__OSUnlockSram       = 0x800386cc;
__OSUnlockSramEx     = 0x800386f0;
__OSSyncSram         = 0x80038714;
__OSCheckSram        = 0x80038724;
__OSReadROM          = 0x80038834;
__OSReadROMCallback  = 0x80038958;
__OSReadROMAsync     = 0x800389b4;
OSGetSoundMode       = 0x80038ac4;
OSSetSoundMode       = 0x80038b44;
OSGetProgressiveMode = 0x80038be8;
OSSetProgressiveMode = 0x80038c58;
OSGetVideoMode       = 0x80038cfc;
OSSetVideoMode       = 0x80038d78;
OSGetLanguage        = 0x80038e28;
OSSetLanguage        = 0x80038e94;
__OSGetBootMode      = 0x80038f28;
__OSSetBootMode      = 0x80038f94;
OSGetEuRgb60Mode     = 0x80039038;
OSGetWirelessID      = 0x8003914c;
OSSetWirelessID      = 0x800391d0;
/* OSGetGbsMode is not present */
/* OSsetGbsMode is not present */

/* OSSemaphore.o is missing */

/* OSStopwatch.o */
OSInitStopwatch  = 0x8003927c;
OSStartStopwatch = 0x800392a8;
OSStopStopwatch  = 0x800392e4;
OSCheckStopwatch = 0x800393b0;
OSResetStopwatch = 0x80039420;
OSDumpStopWatch  = 0x80039448;

/* OSSync.o */
SystemCallVector        = 0x800395ec;
__OSSystemCallVectorEnd = 0x80039608;
__OSInitSystemCall      = 0x8003960c;

/* OSThread.o */
DefaultSwitchThreadCallback = 0x80039670;
OSSetSwitchThreadCallback   = 0x80039674;
__OSThreadInit              = 0x800396e8;
OSInitThreadQueue           = 0x80039840;
OSGetCurrentThread          = 0x80039850;
__OSSwitchThread            = 0x8003985c;
OSIsThreadSuspended         = 0x800398b8;
OSIsThreadTerminated        = 0x800398d4;
__OSIsThreadActive          = 0x80039908;
OSDisableScheduler          = 0x8003994c;
OSEnableScheduler           = 0x8003998c;
SetRun                      = 0x800399cc;
UnsetRun                    = 0x80039a38;
__OSGetEffectivePriority    = 0x80039a38;
SetEffectivePriority        = 0x80039adc;
UpdatePriority              = 0x80039c9c;
__OSPromoteThread           = 0x80039d14;
SelectThread                = 0x80039d64;
__OSReschedule              = 0x80039f8c;
OSYieldThread               = 0x80039fbc;
OSCreateThread              = 0x80039ff8;
OSExitThread                = 0x8003a1e0;
OSCancelThread              = 0x8003a2c4;
OSJoinThread                = 0x8003a480;
OSDetachThread              = 0x8003a5c0;
OSResumeThread              = 0x8003a660;
OSSuspendThread             = 0x8003a8e8;
OSSleepThread               = 0x8003aa58;
OSWakeupThread              = 0x8003ab44;
OSSetThreadPriority         = 0x8003ac48;
OSGetThreadPriority         = 0x8003ad08;
OSSetIdleFunction           = 0x8003ad10;
OSGetIdleFunction           = 0x8003ada0;
CheckThreadQueue            = 0x8003adbc;
IsMember                    = 0x8003ae58;
OSCheckActiveThreads        = 0x8003ae84;
OSClearStack                = 0x8003b5d4;
OSSetThreadSpecific         = 0x8003b680;
OSGetThreadSpecific         = 0x8003b6b0;

/* OSTime.o */
OSGetTime             = 0x8003b6e8;
OSGetTick             = 0x8003b700;
__SetTime             = 0x8003b708;
__OSSetTime           = 0x8003b71c;
__OSGetSystemTime     = 0x8003b7a0;
__OSTimeToSystemTime  = 0x8003b804;
__OSSetTick           = 0x8003b85c;
IsLeapYear            = 0x8003b864;
GetYearDays           = 0x8003b8ec;
GetLeapDays           = 0x8003b994;
GetDates              = 0x8003b9e4;
OSTicksToCalendarTime = 0x8003bb80;
OSCalendarTimeToTicks = 0x8003bd84;

/* OSTimer.o is missing */
/* OSUtf.o is missing */
/* time.dolphin.o is missing */
/* __start.o linked at the start of the dol */

/* __ppc_eabi_init.o */
__init_cpp   = 0x8003c084;
__fini_cpp   = 0x8003c0d8;
abort        = 0x8003c12c;
exit         = 0x8003c14c;
_ExitProcess = 0x8003c1a0;

/* end os.a */

/* db.a */
/* db.o */
DBInit                      = 0x8003c1c0;
DBIsDebuggerPresent         = 0x8003c1e8;
__DBExceptionDestinationAux = 0x8003c204;
__DBExceptionDestination    = 0x8003c24c;
__DBIsExceptionMarked       = 0x8003c25c;
__DBMarkException           = 0x8003c278;
__DBSetPresent              = 0x8003c2bc;
DBPrintf                    = 0x8003c2c8;

/* end db.a */

/* start mtx.a */

/* mtx.o */
C_MTXIdentity         = 0x8003c318; PSMTXIdentity    = 0x8003c354;
C_MTXCopy             = 0x8003c380; PSMTXCopy        = 0x8003c3ec;
C_MTXConcat           = 0x8003c420; PSMTXConcat      = 0x8003c6c8;
C_MTXConcatArray      = 0x8003c794; PSMTXConcatArray = 0x8003c7fc;
C_MTXTranspose        = 0x8003c954; PSMTXTranspose   = 0x8003c9f0;
C_MTXInverse          = 0x8003ca40; PSMTXInverse     = 0x8003ccec;
C_MTXInvXpose         = 0x8003cde4; PSMTXInvXpose    = 0x8003d000;
C_MTXRotRad           = 0x8003d0c8; PSMTXRotRad      = 0x8003d138;
C_MTXRotTrig          = 0x8003d1a8; PSMTXRotTrig     = 0x8003d2ac;
C_MTXRotAxisRad       = 0x8003d35c;  __PSMTXRotAxisRadInternal = 0x8003d474; PSMTXRotAxisRad = 0x8003d524;
C_MTXTrans            = 0x8003d594; PSMTXTrans      = 0x8003d5d0;
C_MTXTransApply       = 0x8003d604; PSMTXTransApply = 0x8003d67c;
C_MTXScale            = 0x8003d6c8; PSMTXScale      = 0x8003d700;
C_MTXScaleApply       = 0x8003d728; PSMTXScaleApply = 0x8003d7bc;
C_MTXQuat             = 0x8003d814; PSMTXQuat       = 0x8003d904;
C_MTXReflect          = 0x8003d9a8; PSMTXReflect    = 0x8003daa8;
C_MTXLookAt           = 0x8003db18;
C_MTXLightFrustum     = 0x8003dca4;
C_MTXLightPerspective = 0x8003dd38;
C_MTXLightOrtho       = 0x8003de04;

/* mtxvec.o */
C_MTXMultVec        = 0x8003de8c; PSMTXMultVec        = 0x8003df20;
C_MTXMultVecArray   = 0x8003df78; PSMTXMultVecArray   = 0x8003e178;
C_MTXMultVecSR      = 0x8003e204; PSMTXMultVecSR      = 0x8003e280;
C_MTXMultVecArraySR = 0x8003e2d8; PSMTXMultVecArraySR = 0x8003e490;

/* mtxstack.o missing */

/* mtx44.o */
C_MTXFrustum      = 0x8003e518;
C_MTXPerspective  = 0x8003e5b4;
C_MTXOrtho        = 0x8003e684;
C_MTX44Identity   = 0x8003e71c; PSMTX44Identity   = 0x8003e768;
C_MTX44Copy       = 0x8003e79c; PSMTX44Copy       = 0x8003e828;
C_MTX44Concat     = 0x8003e86c; PSMTX44Concat     = 0x8003ecbc;
C_MTX44Transpose  = 0x8003edc0; PSMTX44Transpose  = 0x8003ee84;
C_MTX44Inverse    = 0x8003eee8;
C_MTX44Trans      = 0x8003f2d8; PSMTX44Trans      = 0x8003f324;
C_MTX44TransApply = 0x8003f360; PSMTX44TransApply = 0x8003f3f8;
C_MTX44Scale      = 0x8003f454; PSMTX44Scale      = 0x8003f4a0;
C_MTX44ScaleApply = 0x8003f4d4; PSMTX44ScaleApply = 0x8003f588;
C_MTX44RotRad     = 0x8003f5f0; PSMTXRotRad       = 0x8003f660;
C_MTX44RotTrig    = 0x8003f6d0; PSMTXRotTrig      = 0x8003f7ec;
C_MTX44RotAxisRad = 0x8003f8c0; __PSMTX44RotAxisRadInternal = 0x8003f9ec; PSMTX44RotAxisRad = 0x8003faa8;

/* mtx44vec.o missing */

/* vec.o */
C_VECAdd            = 0x8003fb18; PSVECAdd          = 0x8003fb4c;
C_VECSubtract       = 0x8003fb70; PSVECSubtract     = 0x8003fba4;
C_VECScale          = 0x8003fbc8; PSVECScale        = 0x8003fbf0;
C_VECNormalize      = 0x8003fc0c; PSVECNormalize    = 0x8003fcd4;
C_VECSquareMag      = 0x8003fd18; PSVecSquareMag    = 0x8003fd3c;
C_VECMag            = 0x8003fd54; PSVECMag          = 0x8003fde8;
C_VECDotProduct     = 0x8003fe2c; PSVECDotProduct   = 0x8003fe5c;
C_VECCrossProduct   = 0x8003fe7c; PSVECCrossProduct = 0x8003fec8;
C_VECHalfAngle      = 0x8003ff04;
C_VECReflect        = 0x8003ffdc;
C_VECSquareDistance = 0x800400b0; PSVECSquareDistance = 0x800400ec;
C_VECDistance       = 0x80040114; PSVECDistance       = 0x800401c0;

/* quat.o */
C_QUATAdd         = 0x80040214; PSQUATAdd        = 0x80040258;
C_QUATSubtract    = 0x8004027c; PSQUATSubtract   = 0x800402c0;
C_QUATMultiply    = 0x800402e4; PSQUATMultiply   = 0x80040438;
C_QUATScale       = 0x80040494; PSQUATScale      = 0x800404c8;
C_QUATDotProduct  = 0x800404e4; PSQUATDotProduct = 0x80040524;
C_QUATNormalize   = 0x80040544; PSQUATNormalize  = 0x8004064c;
C_QUATInverse     = 0x800406a0; PSQUATInverse    = 0x80040724;
C_QUATDivide      = 0x8004077c; PSQUATDivide     = 0x80040950;
C_QUATExp         = 0x800409a0;
C_QUATLogN        = 0x80040aac;
C_QUATMakeClosest = 0x80040bc0;
C_QUATRotAxisRad  = 0x80040c5c;
C_QUATMtx         = 0x80040ce8;
C_QUATLerp        = 0x80040f40;
C_QUATSlerp       = 0x80040fa4;
C_QUATSquad       = 0x80041118;
C_QUATCompA       = 0x800411b8;

/* psmtx.o */
PSMTXReorder           = 0x800415ec;
PSMTXROMultVecArray    = 0x80041638;
PSMTXROSkin2VecArray   = 0x80041750;
PSMTXROMultS16VecArray = 0x800418f8;
PSMTXMultS16VecArray   = 0x80041a18;

/* end mtx.a */

/* start dvd.a */
/* dvdfs.o */
__DVDFSInit              = 0x80041ac8;
isSame                   = 0x80041b00;
DVDConvertPathToEntrynum = 0x80041ba0;
DVDFastOpen              = 0x80041e94;
DVDOpen                  = 0x80041f08;
DVDClose                 = 0x80041fd0;
myStrncpy                = 0x80041ff4;
entryToPath              = 0x8004202c;
DVDConvertEntrynumToPath = 0x8004218c;
DVDGetCurrentDir         = 0x800422e0;
DVDChangeDir             = 0x800423a4;
DVDReadAsyncPrio         = 0x80042404;
cbForReadAsync           = 0x800424c4;
DVDReadPrio              = 0x800424f4;
cbForReadSync            = 0x8004260c;
DVDSeekAsyncPrio         = 0x80042630;
cbForSeekAsync           = 0x800426c8;
DVDSeekPrio              = 0x800426f8;
cbForSeekSync            = 0x800427a4;
DVDFastOpenDir           = 0x800427e8;
DVDOpenDir               = 0x80042850;
DVDReadDir               = 0x80042910;
DVDCloseDir              = 0x800429b4;
/* DVDRewindDir not present */
DVDGetFSTLocation        = 0x800429bc;
DVDPrepareStreamAsync    = 0x800429c8;
cbForReadAsync           = 0x80042ab4;
DVDPrepareStream         = 0x80042ae4;
cbForPrepareStreamSync   = 0x80042c28;
DVDGetTransferredSize    = 0x80042c4c;

/* dvd.o */
defaultOptionalCommandChecker  = 0x80042cb8;
__DVDSetOptionalCommandChecker = 0x80042cbc;
DVDInit                        = 0x80042cc4;
stateReadingFST                = 0x80042d90;
cbForStateReadingFST           = 0x80042e24;
cbForStateError                = 0x80042eb0;
stateError                     = 0x80042f5c;
stateTimeout                   = 0x80042f88;
stateGettingError              = 0x80042fbc;
CategorizeError                = 0x80042fe4;
CheckCancel                    = 0x80043098;
cbForStateGettingError         = 0x80043134;
cbForUnrecoveredError          = 0x800433c8;
cbForUnrecoveredErrorRetry     = 0x80043430;
stateGoToRetry                 = 0x800434c8;
cbForStateGoToRetry            = 0x800434f0;
stateCheckID                   = 0x80043648;
stateCheckID3                  = 0x80043728;
stateCheckID2a                 = 0x8004375c;
cbForStateCheckID2a            = 0x80043790;
stateCheckID2                  = 0x80043804;
cbForStateCheckID1             = 0x8004383c;
cbForStateCheckID2             = 0x80043950;
cbForStateCheckID3             = 0x80043a34;
AlarmHandler                   = 0x80043b30;
stateCoverClosed               = 0x80043b74;
stateCoverClosed_CMD           = 0x80043c40;
cbForStateCoverClosed          = 0x80043c70;
stateMotorStopped              = 0x80043ce0;
cbForStateMotorStopped         = 0x80043d08;
stateReady                     = 0x80043dec;
stateBusy                      = 0x8004401c;
__DVDSetImmCommand             = 0x8004433c;
__DVDSetDmaCommand             = 0x80044360;
IsImmCommandWithResult         = 0x8004437c;
IsDmaCommand                   = 0x800443d8;
cbForStateBusy                 = 0x80044418;
__DVDGetIssueCommandAddr       = 0x80044a50;
issueCommand                   = 0x80044a5c;
DVDReadAbsAsyncPrio            = 0x80044b18;
DVDSeekAbsAsyncPrio            = 0x80044bf4;
DVDReadAbsAsyncForBS           = 0x80044cc0;
DVDReadDiskID                  = 0x80044d90;
DVDPrepareStreamAbsAsync       = 0x80044e64;
DVDCancelStreamAsync           = 0x80044f28;
DVDCancelStream                = 0x80044fe4;
cbForCancelStreamSync          = 0x80045084;
DVDStopStreamAtEndAsync        = 0x800450ac;
DVDStopStreamAtEnd             = 0x80045168;
cbForStopStreamAtEndSync       = 0x80045208;
DVDGetStreamErrorStatus        = 0x800452ec;
cbForGetStreamErrorStatusSync  = 0x8004538c;
DVDGetStreamPlayAddrAsync      = 0x800453b4;
DVDGetStreamPlayAddr           = 0x80045470;
cbForGetStreamPlayAddrSync     = 0x80045510;
DVDGetStreamStartAddr          = 0x800455f4;
cbForGetStreamStartAddrSync    = 0x80045694;
DVDGetStreamLengthAsync        = 0x800456bc;
DVDGetStreamLength             = 0x80045778;
cbForGetStreamLengthSync       = 0x80045818;
__DVDAudioBufferConfig         = 0x80045840;
DVDChangeDiskAsyncForBS        = 0x800458fc;
DVDChangeDiskAsync             = 0x800459b8;
DVDChangeDisk                  = 0x80045ac8;
cbForChangeDiskSync            = 0x80045bcc;
/* DVDStopMotorAsync is not present? */
/* DVDStopMotor is not present? */
/* cbForStopMotorSync is not present? */
DVDInquiryAsync                = 0x80045bf0;
DVDInquiry                     = 0x80045cc0;
cbForInquirySync               = 0x80045d88;
DVDReset                       = 0x80045dac;
DVDResetRequired               = 0x80045df0;
DVDGetCommandBlockStatus       = 0x80045df8;
DVDGetDriveStatus              = 0x80045e44;
DVDSetAutoInvalidation         = 0x80045ef0;
DVDPause                       = 0x80045f00;
DVDResume                      = 0x80045f3c;
DVDCancelAsync                 = 0x80045f8c;
DVDCancel                      = 0x80046208;
cbForCancelSync                = 0x800462b4;
DVDCancelAllAsync              = 0x800462d8;
DVDCancelAll                   = 0x800463c4;
cbForCancelAllSync             = 0x800464fc;
DVDGetCurrentDiskID            = 0x80046528;
DVDCheckDisk                   = 0x80046530;
__DVDPrepareResetAsync         = 0x80046628;
/* __DVDTestAlarm not present             */

/* dvdqueue.o */
__DVDClearWaitingQueue     = 0x80046744;
__DVDPushWaitingQueue      = 0x8004677c;
PopWaitingQueuePrio        = 0x800467e4;
__DVDPopWaitingQueue       = 0x80046848;
__DVDCheckWaitingQueue     = 0x800468e8;
__DVDDequeueWaitingQueue   = 0x80046940;
__DVDIsBlockInWaitingQueue = 0x800469a0;
DVDDumpWaitingQueue        = 0x80046a6c;

/* dvderror.o */
ErrorCode2Num       = 0x80046b78;
Convert             = 0x80046c94;
__DVDStoreErrorCode = 0x80046df4;

/* dvdidutils.o */
strnlen           = 0x80046e70;
DVDCompareDiskID  = 0x80046ea8;
DVDGenerateDiskID = 0x80046fa0;

/* dvdFatal.o */
ShowMessage              = 0x80047020;
DVDSetAutoFatalMessaging = 0x800470ac;
__DVDPrintFatalMessage   = 0x8004711c;

/* fstload.o */
cb        = 0x8004714c;
__fstload = 0x80047224;

/* dvdlow.o */
__DVDInitWA                 = 0x8004738c;
ProcessNextCommand          = 0x800473cc;
__DVDInterruptHandler       = 0x80047460;
AlarmHandler                = 0x80047740;
AlarmHandlerForTimeout      = 0x800477c4;
SetTimeoutAlarm             = 0x80047834;
Read                        = 0x80047898;
AudioBufferOn               = 0x800479a8;
HitCache                    = 0x800479e0;
DoJustRead                  = 0x80047a80;
SeekTwiceBeforeRead         = 0x80047ab4;
WaitBeforeRead              = 0x80047b34;
DVDLowRead                  = 0x80047bc0;
DVDLowSeek                  = 0x80047e58;
DVDLowWaitCoverClose        = 0x80047eec;
DVDLowReadDiskID            = 0x80047f18;
DVDLowStopMotor             = 0x80047fbc;
DVDLowRequestError          = 0x80048048;
DVDLowInquiry               = 0x800480d4;
DVDLowAudioStream           = 0x80048170;
DVDLowRequestAudioStatus    = 0x80048208;
DVDLowAudioBufferConfig     = 0x80048294;
DVDLowReset                 = 0x80048330;
DVDLowSetResetCoverCallback = 0x800483ec;
DoBreak                     = 0x80048430;
AlarmHandlerForBreak        = 0x8004844c;
SetBreakAlarm               = 0x800484c0;
DVDLowBreak                 = 0x80048524;
DVDLowClearCallback         = 0x80048538;
DVDLowGetCoverStatus        = 0x80048550;
__DVDLowSetWAType           = 0x800485e4;
/* __DVDLowTestAlarm is not present    */

/* end dvd.o */

/* pad.a */

/* Padclamp.o */
ClampStick     = 0x80048628;
ClampCircle    = 0x80048758;
ClampTrigger   = 0x80048900;
PADClamp       = 0x8004893c;
PADClampCircle = 0x80048a50;

/* Pad.o */
PADEnable                 = 0x80048b5c;
PADDisable                = 0x80048bbc;
DoReset                   = 0x80048c54;
UpdateOrigin              = 0x80048cc4;
PADOriginCallback         = 0x80048e68;
PADOriginUpdateCallback   = 0x80048f2c;
PADProbeCallback          = 0x80048fec;
PADTypeAndStatusCallback  = 0x800490c4;
PADReceiveCheckCallback   = 0x800493f0;
PADReset                  = 0x80049524;
PADRecalibrate            = 0x80049624;
PADInit                   = 0x80049728;
PADRead                   = 0x80049940;
PADSetSamplingRate        = 0x80049cec;
PADControlAllMotors       = 0x80049d0c;
PADControlMotor           = 0x80049dc4;
PADSetSpec                = 0x80049e68;
PADGetSpec                = 0x80049ec8;
SPEC0_MakeStatus          = 0x80049ed0;
SPEC1_MakeStatus          = 0x8004a044;
ClampS8                   = 0x8004a1b8;
ClampU8                   = 0x8004a20c;
SPEC2_MakeStatus          = 0x8004a228;
PADGetType                = 0x8004a620;
PADSync                   = 0x8004a694;
PADSetAnalogMode          = 0x8004a6f8;
OnReset                   = 0x8004a76c;
_PADDisableXPatch         = 0x8004a900;
SamplingHandler           = 0x8004a90c;
PADSetSamplingCallback    = 0x8004a96c;
__PADDisableRecalibration = 0x8004a9c0;
/* __PADDisableRumble is not present */
/* PADIsBarrel is not present        */

/* end pad.a */

/* begin ai.a */
/* ai.o */
AIRegisterDMACallback    = 0x8004aa3c;
AIInitDMA                = 0x8004aa80;
AIGetDMAEnableFlag       = 0x8004ab08;
AIStartDMA               = 0x8004ab18;
AIStopDMA                = 0x8004ab30;
AIGetDMABytesLeft        = 0x8004ab48;
AIGetDMAStartAddr        = 0x8004ab58;
AIGetDMALength           = 0x8004ab74;
AICheckInit              = 0x8004ab84;
AIRegisterStreamCallback = 0x8004ab8c;
AIGetStreamSampleCount   = 0x8004abd0;
AIResetStreamSampleCount = 0x8004abe0;
AISetStreamTrigger       = 0x8004abf8;
AIGetStreamTrigger       = 0x8004ac04;
AISetStreamPlayState     = 0x8004ac14;
AIGetStreamPlayState     = 0x8004acec;
AISetDSPSampleRate       = 0x8004acfc;
AIGetDSPSampleRate       = 0x8004addc;
AISetStreamSampleRate    = 0x8004adf0;
__AI_DEBUG_set_stream_sample_rate = 0x8004ae18;
__AI_set_stream_sample_rate       = 0x8004ae38;
AIGetStreamSampleRate    = 0x8004af0c;
AISetStreamVolLeft       = 0x8004af1c;
AIGetStreamVolLeft       = 0x8004af38;
AISetStreamVolRight      = 0x8004af48;
AIGetStreamVolRight      = 0x8004af64;
AIInit                   = 0x8004af74;
AIReset                  = 0x8004b0e0;
__AISHandler             = 0x8004b0ec;
__AIDHandler             = 0x8004b168;
__AICallbackStackSwitch  = 0x8004b214;
__AI_SRC_INIT            = 0x8004b26c;
__ai_src_get_time        = 0x8004b450;

/* end ai.a */

/* card.a */

/* CARDBios.o */
__CARDDefaultApiCallback = 0x8004b458;
__CARDSyncCallback       = 0x8004b45c;
__CARDExtHandler         = 0x8004b490;
__CARDExiHandler         = 0x8004b490;
__CARDTxHandler          = 0x8004b680;
__CARDUnlockHandler      = 0x8004b728;
__CARDEnableInterrupt    = 0x8004b7ac;
__CARDReadStatus         = 0x8004b86c;
/* __CARDReadVendorID is not present */
__CARDClearStatus        = 0x8004b95c;
__CARDSleep              = 0x8004ba08;
__CARDWakeup             = 0x8004bab4;
TimeoutHandler           = 0x8004bb60;
SetupTimeoutAlarm        = 0x8004bc04;
Retry                    = 0x8004bcfc;
UnlockedCallback         = 0x8004bf28;
__CARDStart              = 0x8004c038;
__CARDReadSegment        = 0x8004c1ec;
__CARDWritePage          = 0x8004c320;
__CARDErase              = 0x8004c43c;
__CARDEraseSector        = 0x8004c520;
CARDInit                 = 0x8004c600;
__CARDGetFontEncode      = 0x8004c6ac;
__CARDSetFontEncode      = 0x8004c6b4;
__CARDSetDiskID          = 0x8004c6dc;
CARDGetDiskID            = 0x8004c714;
CARDSetDiskID            = 0x8004c72c;
__CARDGetControlBlock    = 0x8004c7a4;
__CARDPutControlBlock    = 0x8004c85c;
CARDGetResultCode        = 0x8004c8c0;
CARDFreeBlocks           = 0x8004c8f0;
CARDGetEncoding          = 0x8004ca40;
CARDGetMemSize           = 0x8004cac8;
CARDGetSectorSize        = 0x8004cb4c;
__CARDSync               = 0x8004cbd0;
OnReset                  = 0x8004cc68;
/* CARDSetFastMode is not present   */
/* CARDGetFastMode is not present   */
/* CARDGetCurrentMode is not present*/

/* CARDUnlock.o is missing */

/* CARDRdwr.o */
BlockReadCallback   = 0x8004ccb8;
__CARDRead          = 0x8004cd94;
BlockWriteCallback  = 0x8004cdf8;
__CARDWrite         = 0x8004ced4;
CARDGetXferredBytes = 0x8004cf38;

/* CARDBlock.o */
__CARDGetFatBlock    = 0x8004cf50;
WriteCallback        = 0x8004cf58;
EraseCallback        = 0x8004d02c;
__CARDAllocBlock     = 0x8004d0f4;
__CARDFreeBlock      = 0x8004d20c;
__CARDUpdateFatBlock = 0x8004d2a8;

/* CARDDir.o */
__CARDGetDirBlock = 0x8004d354;
WriteCallback     = 0x8004d35c;
EraseCallback     = 0x8004d42c;
__CARDUpdateDir   = 0x8004d4f4;

/* CARDCheck.o */
__CARDCheckSum   = 0x8004d5b8;
VerifyID         = 0x8004d768;
VerifyDir        = 0x8004d9ec;
VerifyFAT        = 0x8004dc2c;
__CARDVerify     = 0x8004deb0;
CARDCheckExAsync = 0x8004df3c;
CARDCheckAsync   = 0x8004e4cc;
CARDCheckEx      = 0x8004e4f4;
CARDCheck        = 0x8004e550;

/* CARDMount.o */
IsCard              = 0x8004e5a4;
__CARDDisable       = 0x8004e670;
CARDProbe           = 0x8004e6c4;
CARDProbeEx         = 0x8004e6fc;
DoMount             = 0x8004e878;
__CARDMountCallback = 0x8004ec88;
CARDMountAsync      = 0x8004edc0;
CARDMount           = 0x8004ef60;
DoUnmount           = 0x8004efa8;
CARDUnmount         = 0x8004f044;

/* CARDFormat.o */
FormatCallback          = 0x8004f0f0;
__CARDFormatRegionAsync = 0x8004f234;
__CARDFormatRegion      = 0x8004f88c;
CARDFormatAsync         = 0x8004f8d4;
CARDFormat              = 0x8004f91c;

/* CARDOpen.o */
__CARDCompareFileName = 0x8004f970;
__CARDAccess          = 0x8004f9d8;
__CARDIsPublic        = 0x8004fa70;
__CARDGetFileNo       = 0x8004faa0;
CARDFastOpen          = 0x8004fbf0;
CARDOpen              = 0x8004fd50;
CARDClose             = 0x8004fec8;
__CARDIsOpened        = 0x8004ff1c;

/* CARDCreate.o */
CreateCallbackFat = 0x8004ff24;
CARDCreateAsync   = 0x80050054;
CARDCreate        = 0x80050274;

/* CARDRead.o */
__CARDSeek    = 0x800502bc;
ReadCallBack  = 0x80050474;
CARDReadAsync = 0x800505a4;
CARDRead      = 0x800506ec;
CARDCancel    = 0x80050734;

/* CARDWrite.o */
WriteCallback  = 0x800507c0;
EraseCallback  = 0x80050930;
CARDWriteAsync = 0x800509e0;
CARDWrite      = 0x80050af4;

/* CARDDelete.o */
DeleteCallback      = 0x80050b3c;
CARDFastDeleteAsync = 0x80050be0;
CARDFastDelete      = 0x80050d0c;
CARDDeleteAsync     = 0x80050d54;
CARDDelete          = 0x80050e64;

/* CARDStat.o */
UpdateIconOffsets  = 0x80050eac;
CARDGetStatus      = 0x800510a4;
CARDSetStatusAsync = 0x800511d0;
CARDSetStatus      = 0x80051344;

/* CARDRename.o is missing */
/* CARDStatEx.o is missing */
/* CARDRaw.o is missing */

/* CARDNet.o */
CARDSetVendorID        = 0x8005138c;
CARDGetVendorID        = 0x8005139c;
CARDGetSerialNo        = 0x800513a4;
CARDGetUniqueCode      = 0x80051468;
CARDGetAttributes      = 0x800514fc;
CARDSetAttributesAsync = 0x8005153c;
CARDSetAttributes      = 0x800515b4;

/* end card.a */

/* todo: 80051638 */

/* vi.a */
/* vi.o */
getEncoderType            = 0x800551e4;
cntlzd                    = 0x800551ec;
VISetRegs                 = 0x80055248;
__VIRetraceHandler        = 0x80055368;
VISetPreRetraceCallback   = 0x80055598;
VISetPostRetraceCallback  = 0x800555dc;
/* __VISetExtraTiming is missing */
getTiming                 = 0x80055620;
__VIInit                  = 0x800556c0;
AdjustPosition            = 0x800558c0;
ImportAdjustingValues     = 0x80055a20;
VIInit                    = 0x80055a5c;
VIWaitForRetrace          = 0x80055f0c;
setInterruptRegs          = 0x80055f60;
setPicConfig              = 0x80055fdc;
setBBIntervalRegs         = 0x80056074;
setScalingRegs            = 0x80056130;
calcFbbs                  = 0x800561cc;
setFbbRegs                = 0x8005624c;
setHorizontalRegs         = 0x80056520;
setVerticalRegs           = 0x800565ec;
PrintDebugPalCaution      = 0x8005678c;
VIConfigure               = 0x80056820;
VIConfigurePan            = 0x80057048;
VIFlush                   = 0x800573dc;
VISetNextFrameBuffer      = 0x8005750c;
VIGetNextFrameBuffer      = 0x80057578;
VIGetCurrentFrameBuffer   = 0x80057580;
VISetNextRightFrameBuffer = 0x80057588;
VISetBlack                = 0x800575f4;
VISet3D                   = 0x80057670;
VIGetRetraceCount         = 0x80057770;
/* GetCurrentDisplayPosition is missing */
getCurrentHalfLine        = 0x80057778;
getCurrentFieldEvenOdd    = 0x800577c8;
VIGetNextField            = 0x80057830;
VIGetCurrentLine          = 0x800578d8;
VIGetTvFormat             = 0x80057970;
/* VIGetScanMode is missing */
VIGetDTVStatus            = 0x800579d8;
__VISetAdjustingValues    = 0x80057a14;
__VIGetAdjustingValues    = 0x80057cdc;

/* end vi.a */

/* base.a */
/* PPCArch.o */
PPCMfmsr   = 0x80057d28;
PPCMtmsr   = 0x80057d30;
PPCOrMsr   = 0x80057d38;
PPCAndMsr  = 0x80057d44;
PPCAndCMsr = 0x80057d50;
PPCMfhid0  = 0x80057d5c;
PPCMthid0  = 0x80057d64;
PPCMfhid1  = 0x80057d6c;
PPCMfl2cr  = 0x80057d74;
PPCMtl2cr  = 0x80057d7c;
PPCMtdec   = 0x80057d84;
PPCMfdec   = 0x80057d8c;
PPCSync    = 0x80057d94;
PPCEieio   = 0x80057d9c;
PPCHalt    = 0x80057dd0;
PPCMfmmcr0 = 0x80057de4;
PPCMtmmcr0 = 0x80057dec;
PPCMfmmcr1 = 0x80057df4;
PPCMtmmcr1 = 0x80057dfc;
PPCMfpmc1  = 0x80057e04;
PPCMtpmc1  = 0x80057e0c;
PPCMfpmc2  = 0x80057e14;
PPCMtpmc2  = 0x80057e1c;
PPCMfpmc3  = 0x80057e24;
PPCMtpmc3  = 0x80057e2c;
PPCMfpmc4  = 0x80057e34;
PPCMtpmc4  = 0x80057e3c;
PPCMfsia   = 0x80057e44;
PPCMtsia   = 0x80057e4c;
PPCMffpscr = 0x80057e54;
PPCMtfpscr = 0x80057e74;
PPCMfhid2  = 0x80057e9c;
PPCMthid2  = 0x80057ea4;
PPCMfwpar  = 0x80057eac;
PPCMtwpar  = 0x80057eb8;
PPCMfdmaU  = 0x80057ec0;
PPCMfdmaL  = 0x80057ec8;
PPCMtdmaU  = 0x80057ed0;
PPCMtdmaL  = 0x80057ed8;
PPCMfpvr   = 0x80057ee0;
PPCEnableSpeculation  = 0x80057ee8;
PPCDisableSpeculation = 0x80057f10;
PPCSetFpIEEEMode      = 0x80057f38;
PPCSetFpNonIEEEMode   = 0x80057f40;

/* PPCPm.o is missing */

/* end base.a */

/* gba.a */
/* GBA.o */
ShortCommandProc    = 0x80057f48;
GBAInit             = 0x80057f9c;
GBAGetStatusAsync   = 0x80058040;
GBAGetStatus        = 0x800580a4;
GBAResetAsync       = 0x80058134;
GBAReset            = 0x80058198;
OnReset             = 0x80058228;

/* GBAGetProcessStatus.o */
GBAGetProcessStatus = 0x80058238;

/* GBAJoyBoot.o */
F72             = 0x800583ac;
F95             = 0x80058414;
F104            = 0x80058460;
GBAJoyBootAsync = 0x800584b4;
F23             = 0x800585f0;
F25             = 0x80058698;
F27             = 0x80058768;
F29             = 0x80058838;
__GBAX01        = 0x800588b8;
F31             = 0x80058a94;
F33             = 0x80058f2c;
F35             = 0x800590a0;
F37             = 0x800591fc;
F39             = 0x800592f8;
GBAJoyBoot      = 0x80059378;

/* GBARead.o */
ReadProc     = 0x800593c0;
GBAReadAsync = 0x80059420;
GBARead      = 0x8005948c;

/* GBAWrite.o */
WriteProc     = 0x80059520;
GBAWriteAsync = 0x80059550;
GBAWrite      = 0x800595e4;

/* GBAXfer.o */
__GBAHandler          = 0x800596a8;
__GBASyncCallback     = 0x80059784;
__GBASync             = 0x800597b8;
TypeAndStatusCallback = 0x80059824;
__GBATransfer         = 0x80059948;
__GBASetDelay         = 0x800599bc;

/* GBAKey.o */
F152     = 0x800599e0;
F23      = 0x80059a7c;
F25      = 0x80059b58;
__GBAX02 = 0x80059c00;

/* end gba.o */

/* si.a */
/* SIBios.o */
SIBusy                     = 0x80059ce4;
SIIsChanBusy               = 0x80059d04;
SIClearTCInterrupt         = 0x80059d40;
CompleteTransfer           = 0x80059d5c;
SITransferNext             = 0x8005a058;
SIInterruptHandler         = 0x8005a148;
SIEnablePollingInterrupt   = 0x8005a48c;
SIRegisterPollingHandler   = 0x8005a524;
SIUnregisterPollingHandler = 0x8005a5f0;
SIInit                     = 0x8005a6e4;
__SITransfer               = 0x8005a798;
SISync                     = 0x8005a9a4;
SIGetStatus                = 0x8005aaac;
SISetCommand               = 0x8005ab28;
SIGetCommand               = 0x8005ab3c;
SITransferCommands         = 0x8005ab50;
SISetXY                    = 0x8005ab60;
SIEnablePolling            = 0x8005abcc;
SIDisablePolling           = 0x8005ac68;
SIGetResponseRaw = 0x8005acd4;
SIGetResponse = 0x8005ada8;
AlarmHandler = 0x8005ae6c;
SITransfer = 0x8005aef8;
CallTypeAndStatusCallback = 0x8005b064;
GetTypeCallback = 0x8005b0dc;
SIGetType = 0x8005b374;
SIGetTypeAsync = 0x8005b538;
SIDecodeType = 0x8005b674;
SIProbe = 0x8005b7c0;
SIGetTypeString = 0x8005b7e4;

/* SISamplingRate.o */
SISetSamplingRate = 0x8005b93c;
SIRefreshSamplingRate = 0x8005ba20;

/* SISteering.o is missing */
/* SISteeringXfer.o is missing */
/* SISteeringAuto.o is missing */
/* end si.a */

/* exi.a */
/* EXIBios.o */
SetExiInterruptMask = 0x8005ba44;
CompleteTransfer    = 0x8005bb38;
EXIImm              = 0x8005bc8c;
EXIImmEx            = 0x8005bee8;
EXIDma              = 0x8005bf88;
EXISync             = 0x8005c074;
EXIClearInterrupts  = 0x8005c2c0;
EXISetExiCallback   = 0x8005c308;
EXIProbeReset       = 0x8005c384;
__EXIProbe          = 0x8005c3d0;
EXIProbe            = 0x8005c544;
EXIProbeEX          = 0x8005c5c4;
__EXIAttach         = 0x8005c678;
EXIAttach           = 0x8005c74c;
EXIDetach           = 0x8005c858;
/* EXISelectSD is not present */
EXISelect           = 0x8005c914;
EXIDeselect         = 0x8005ca40;
EXIIntrruptHandler  = 0x8005cb50; /* nintendo typo? */
TCInterruptHandler  = 0x8005cc18;
EXTIntrruptHandler  = 0x8005ce30;
EXIInit             = 0x8005cf00;
EXILock             = 0x8005d064;
EXIUnlock           = 0x8005d158;
EXIGetState         = 0x8005d234;
UnlockedHandler     = 0x8005d24c;
EXIGetID            = 0x8005d274;
EXIGetType          = 0x8005d624;
EXIGetTypeString    = 0x8005d73c;

/* EXIUart.o */
ProbeBarnacle      = 0x8005d970;
__OSEnableBarnacle = 0x8005dafc;
InitializeUART     = 0x8005dcb8;
ReadUARTN          = 0x8005dd28;
QueueLength        = 0x8005dd30;
WriteUARTN         = 0x8005ddc8;

/* EXIAd16.o is missing */

/* end exi.h */

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


/* all of the helpers pulled in from libgcc.a GCC 2.95.3 */
__ashldi3 = 0x80070d08; /* 64 bit left shift */
__divdi3    = 0x80070d48; /* signed 64 bit division, truncating toward zero */
/* __fixdfdi   = ???; double to signed 64 bit integer */
__fixunsdfdi = 0x800712fc; /* double to unsigned 64 bit integer, truncating toward zero */
__floatdidf = 0x800714d4; /* signed 64 bit integer to double */
__floatdisf = 0x8007153c; /* signed 64 bit integer to float */
__lshrdi3   = 0x800715f8; /* logical 64 bit right shift */
__moddi3    = 0x80071638; /* signed 64 bit remainder; sign follows numerator */
__udivdi3   = 0x80071bb0; /* unsigned 64 bit division */
__umoddi3   = 0x800720d4; /* unsigned 64 bit remainder */
__cmpdi2    = 0x800725b8; /* signed 64 bit comparison: 0=less, 1=equal, 2=greater */

strcpy      = 0x80072600;
strncpy     = 0x80072630;
strlen      = 0x80072678;
strncmp     = 0x800726a0;
srand       = 0x800726e8;
memmove     = 0x80072710;
memcmp      = 0x80072798;
memset      = 0x800727d0;
memcpy      = 0x800727f8;
tanf    = 0x80072828;
acosf   = 0x80072940;
asinf   = 0x80072a28;



__OSActiveThreadQueue = 0x800000dc;

Controllers = 0x8026bb60;
all_inputs = 0x8026bc40;

camera = 0x8026c7c0;

TBRenderStatePool     = 0x8027d1e0;
DEFAULT_STRING_BUFFER = 0x802cbcc0;
TextBlockPool         = 0x802cc050;

__OSCurrHeap = 0x802d7b10;
PendingCancels = 0x802d7d14;

FrameCounter = 0x802d7e00;

PreshotController = 0x80412f6c;