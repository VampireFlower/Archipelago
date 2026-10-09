#include <types.h>

#pragma once


void _Patch(void* address, uint value);
void _Hook(void* source_address, void* destination_address, bool link);

// write `value` to `address`
#define PATCH(address, value) _Patch((void*)address, value)

#define HOOK(source, destination, link)                       \
    do {                                                      \
        void _##source()      asm(#source);                   \
        void _##destination() asm(#destination);              \
        asm volatile (                                        \
            ".global "#destination"_hook\n"                   \
            "\t.set "#destination"_hook, "#source"\n"         \
        );                                                    \
        _Hook((void*)_##source, (void*)_##destination, link); \
    } while (0)

// write a `bl` targeting `destination` at `source`. defines `destination_hook` with the value of `source`
#define HOOK_CALL(source, destination) HOOK(source, destination, true)
// write a `b` targeting `destination` at `source`. defines `destination_hook` with the value of `source`
#define HOOK_JUMP(source, destination) HOOK(source, destination, false)

/*
    `HOOK_FUNCTION(foo, displaced, myfoo)`

    All calls to foo are redirected to myfoo.
    Neither foo nor myfoo need to be declared when hooking.
    `displaced` is the first instruction of foo. e.g. `mtcr 12`
    to call the original foo, use `ORIGINAL(foo)(args);`
    foo must be declared before invoking ORIGINAL.
*/
#define HOOK_FUNCTION(source, displaced, destination) \
    do {                                              \
        asm volatile (                                \
            "b 0f\n"                                  \
            "\t.global "#source"_orig\n"              \
            "\t"#source"_orig:\n"                     \
            "\t"#displaced"\n"                        \
            "\tb "#source" + 4\n"                     \
            "0:\n"                                    \
        ); HOOK_JUMP(source, destination);            \
    } while (0)

#define ORIGINAL(name) ({typeof(name) name##_orig; name##_orig;})