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



typedef struct Camera {
    Vec3f eye;
    Vec3f target;
    float width;
    float height;
    float fov;
    float nearClip;
    float farClip;
    float aspect;
    mtx view;
    mtx forwardView;
    mtx field_0x9c;
    mtx44 projection;
    BOOL ortho;
    float misc[13];
    mtx field_0x144;
    float field_0x174;
    float field_0x178;
} Camera;

extern Camera camera; // 8026c7c0
extern Camera* pCamera; // 802d7e64


extern uint ShotReplayCount; // 805052a4


#endif