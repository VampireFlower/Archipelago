
#include <types.h>
#include <mgtt.h>

void * operator new(uint size) {
    int tmp = SetCurrentArena(0);
    void* allocation = malloc(size);
    SetCurrentArena(tmp);
    return allocation;
}

void * operator new[](uint size) {
    return operator new(size);
}


void operator delete(void* ptr) {
    free(ptr);
}

void operator delete(void* ptr, uint size) {
    operator delete(ptr);
}

