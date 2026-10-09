#include <types.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSContext OSContext; // so that this file and OSContext don't endlessly include loop

typedef byte   __OSException;
typedef void (*__OSExceptionHandler)(__OSException exception, OSContext* context);

/*800315d0*/ __OSExceptionHandler __OSSetExceptionHandler(__OSException exception, __OSExceptionHandler handler);
/*800315ec*/ __OSExceptionHandler __OSGetExceptionHandler(__OSException exception);


#ifdef __cplusplus
}
#endif