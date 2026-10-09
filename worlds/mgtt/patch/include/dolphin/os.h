#include <types.h>

#include <OSAlloc.h>
#include <OSContext.h>
#include <OSInterrupt.h>
#include <OSThread.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define OSHalt(msg) OSPanic(__FILE__, __LINE__, msg)


/*8001d20c*/ void OSReport(const char* msg, ...);

/*80033138*/ void DCFlushRange(void* address, uint len);
/*80033248*/ void ICInvalidateRange(void* address, uint len);
/*80034548*/ void OSReport(const char* msg, ...); // the game never calls this one
/*800345e8*/ [[noreturn]] void OSPanic(const char* file, int line, const char* msg);


//void OSFatal  ( GXColor fg, GXColor bg, const char* msg );



/*80055f0c*/ void VIWaitForRetrace();

#ifdef __cplusplus
}
#endif