#include <types.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*800032d4*/ void*  memclear(void* dst, uint size); // saves 4 bytes over memset 0
/*80072600*/ char*  strcpy(char* dst, const char* src);
/*80072678*/ size_t strlen(const char* str);
/*800727d0*/ void*  memset(void* dst, int val, uint count);
/*800727f8*/ void*  memcpy(void* dst, const void* src, uint size);

#ifdef __cplusplus
}
#endif