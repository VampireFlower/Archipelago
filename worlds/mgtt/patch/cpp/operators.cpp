
extern "C" {
    #include <types.h>
    #include <mgtt.h>
}

void * operator new(uint size, uint arenaID) {
    return malloc_from(arenaID, size);
}

void * operator new(uint size) {
    return malloc_from(0, size);
}

void * operator new[](uint size, uint arenaID) {
    return operator new (size, arenaID);
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

