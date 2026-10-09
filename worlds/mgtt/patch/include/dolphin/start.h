#include <types.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// an interesting data structure exists at 80003478

/*80003140*/ void __start();
/*80003278*/ void __copy_rom_section(void* dst, void* src, size_t length);
/*800032d4*/ void __init_bss_section(void* address, size_t length);
/*80003304*/ void __init_registers();
/*80003320*/ void __init_data();
/*80003590*/ void __flush_cache(void* address, size_t length);

#ifdef __cplusplus
}
#endif
