#include <types.h>
#include <dolphin/os/OSThread.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*EXICallback)(int, OSContext*);

typedef struct EXIControl {
    EXICallback exiCallback;
    EXICallback tcCallback;
    EXICallback extCallback;
    volatile unsigned long state;
    int immLen;
    unsigned char * immBuf;
    unsigned long dev;
    int items;
    struct {
        unsigned long dev;
        EXICallback callback;
    } queue[3];
} EXIControl;


/*8005ba44*/ void SetExiInterruptMask(int chan, EXIControl* exi);
/*8005bc8c*/ int EXIImm(int chan, void * data, int len, int mode, void * completionCb);
/*8005bee8*/ UNKNOWN_FUNCTION(EXIImmEx)
/*8005bf88*/ UNKNOWN_FUNCTION(EXIDma)
/*8005c074*/ int EXISync(int chan);
/*8005c2c0*/ void EXIClearInterrupts();
/*8005c308*/ void EXISetExiCallback(int chan, EXICallback cb);
BOOL __EXIProbe(int chan);
int EXIProbe(int chan);
int EXIProbeEX(int chan);
UNKNOWN_FUNCTION(EXIAttach)
UNKNOWN_FUNCTION(EXIDetach)
int EXISelect(int chan, int dev, int freq);
int EXIDeselect(int chan);
UNKNOWN_FUNCTION(EXIInterruptHandler)
UNKNOWN_FUNCTION(TCInterruptHandler)
UNKNOWN_FUNCTION(EXTInterruptHandler)
UNKNOWN_FUNCTION(EXIInit)
int EXILock(int chan, int dev, void * unlockCb);
int EXIUnlock(int chan);
UNKNOWN_FUNCTION(UnlockedHandler)
int EXIGetID(int chan, int dev, uint * outId);
UNKNOWN_FUNCTION(ProbeBarnacle)
UNKNOWN_FUNCTION(__OSEnableBarnacle)
UNKNOWN_FUNCTION(EXIWriteReg)


#ifdef __cplusplus
}
#endif