// powerpc-eabi-gcc -Os -mcpu=750 -mbig-endian -ffreestanding -nostdlib -fno-pic -mno-sdata -G 0 -c main.c -o file.o

#include <types.h>
#include <mtx.h>

#ifndef MGTT_H
#define MGTT_H







void* malloc(int);
void* malloc_from(int heap, int);
void* memcpy(void* dst, void* src, uint size);
void  free(void*);
int   GetHeapID(); // which heap the current thread uses

int GetRandomInt(); // 31 bits, guaranteed >= 0
float GetRandomFloat(); // 0 to 1


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
    Mtx view;
    Mtx forwardView;
    Mtx field_0x9c;
    Mtx44 projection;
    BOOL ortho;
    float misc[13];
    Mtx field_0x144;
    float field_0x174;
    float field_0x178;
} Camera;

extern Camera camera; // 8026c7c0
extern Camera SecondaryCamera; // 80505408
extern Camera* pCamera; // 802d7e64


extern uint ShotReplayCount;       // 805052a4

extern int  FlyingCameraAngle;     // 8050f128
extern BOOL TRY_BEHIND_CUP_CAMERA; // 8050f14c
extern BOOL TRY_ACTION_CUP_CAMERA; // 8050f150


#endif