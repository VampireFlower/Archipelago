// powerpc-eabi-gcc -Os -mcpu=750 -mbig-endian -ffreestanding -nostdlib -fno-pic -mno-sdata -G 0 -c main.c -o file.o

#include <types.h>

#ifndef MGTT_H
#define MGTT_H







void* malloc(int);
void* malloc_from(int heap, int);
void  free(void*);
int   GetHeapID(); // which heap the current thread uses

int GetRandomInt(); // 31 bits, guaranteed >= 0
float GetRandomFloat(); // 0 to 1

typedef int TextBlock;

extern TextBlock TextBlocks[]; // 802cc050

TextBlock* TextBlockActivate(int, int, int, int, int, int);
void TextBlockConfigure(TextBlock*, int strid, int x, int y, int);
void TextBlockDelete(TextBlock*);
void TextBlockDrawGlyphs(int subsystem, mtx, int color, int);
void TextBlockStateMachine(void);

void add_to_render_queue(float depth, void* drawer, int arg, int priority);






extern uint ShotReplayCount; // 805052a4


#endif