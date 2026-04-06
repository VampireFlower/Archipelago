// powerpc-eabi-gcc -Os -mcpu=750 -mbig-endian -ffreestanding -nostdlib -fno-pic -mno-sdata -G 0 -c text.c -o file.o

#ifndef MGTT_H
#define MGTT_H

typedef float mtx[3][4];
typedef float mtx44[4][4];

typedef void * pTextBlock;

#define C_MTXOrtho ((void (*) \
(                             \
    mtx44,                    \
    float top,                \
    float bottom,             \
    float left,               \
    float right,              \
    float near,               \
    float far                 \
))0x8003e684)

#define PSMTXIdentity       ((void (*)(mtx))                              0x8003c354)
#define GXSetTexCoordGen2   ((void (*)(int,int,int,int))                  0x80068814)
#define GXSetNumTexGens     ((void (*)(int))                              0x80068a38)
#define GXSetCullMode       ((void (*)(int))                              0x8006982c)
#define GXSetNumChans       ((void (*)(int))                              0x8006afc4)
#define GXSetChanCtrl       ((void (*)(int, int, int, int, int, int, int))0x8006afe8)
#define GXSetTevOrder       ((void (*)(int, int, int, int))               0x8006dba0)
#define GXSetNumTevStages   ((void (*)(int))                              0x8006dd18)
#define GXSetBlendMode      ((void (*)(int, int, int, int))               0x8006e154)
#define GXSetZMode          ((void (*)(int, int, int))                    0x8006e23c)
#define GXSetProjection     ((void (*)(mtx44,int type))                   0x8006fcc0)
#define GXLoadPosMtxImm     ((void (*)(mtx, int))                         0x8006fe10)








#define TextBlockCreate     ((pTextBlock (*)(int, int, int, int, int, int))      0x80024e04)
#define TextBlockConfigure  ((void (*)(pTextBlock, int strid, int x, int y, int))0x80024cfc)
#define TextBlockDraw       ((void (*)(int subsystem, mtx, int color, int))      0x80027ecc)
#define TextBlockDelete     ((void (*)(pTextBlock))                              0x80024c98)

#define add_to_render_queue ((void (*)(float depth, void* drawer, int arg, int priority))0x8001e924)



float Float(int bits)
{
    float out;
    int tmp;

    __asm__ volatile(
        "stw %1, 0(%2)\n"
        "lfs %0, 0(%2)\n"
        : "=f"(out)
        : "r"(bits), "r"(&tmp)
    );

    return out;
}



#endif