#include <types.h>
#include <OSContext.h>
#include <OSException.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


typedef short  __OSInterrupt;
typedef uint OSInterruptMask;
typedef void (*__OSInterruptHandler)(
    __OSInterrupt   interrupt,
    OSContext*      context
);

/*80034c14*/ BOOL                 OSDisableInterrupts();
/*80034c28*/ BOOL                 OSEnableInterrupts();
/*80034c3c*/ BOOL                 OSRestoreInterrupts(BOOL level);
/*80034c60*/ __OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt, __OSInterruptHandler handler);
/*80034c7c*/ __OSInterruptHandler __OSGetInterruptHandler(__OSInterrupt interrupt);
/*80034d04*/ OSInterruptMask      OSSetInterruptMask(OSInterruptMask mask);
/*80034fdc*/ OSInterruptMask      OSGetInterruptMask();
/*80035078*/ OSInterruptMask      __OSMaskInterrupts(OSInterruptMask mask);
/*80035100*/ OSInterruptMask      __OSUnmaskInterrupts(OSInterruptMask mask);
/*80035188*/ void                 __OSDispatchInterrupt(__OSException exception, OSContext* context);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

struct InterruptsDisabled {

    BOOL level;

    InterruptsDisabled() : level(OSDisableInterrupts()) {}
    ~InterruptsDisabled() {OSRestoreInterrupts(level);}

};

#endif