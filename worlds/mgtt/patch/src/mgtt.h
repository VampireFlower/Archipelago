// powerpc-eabi-gcc -Os -mcpu=750 -mbig-endian -ffreestanding -nostdlib -fno-pic -mno-sdata -G 0 -c main.c -o file.o

#ifndef MGTT_H
#define MGTT_H

typedef float mtx[3][4];
typedef float mtx44[4][4];

typedef struct { int stuff; } TextBlock;

void* malloc(int);
void  free(void*);


void C_MTXOrtho(mtx44, float top, float bottom, float left, float right, float near, float far);

void PSMTXIdentity(mtx);
void GXSetTexCoordGen2(int,int,int,int);
void GXSetNumTexGens(int);
void GXSetCullMode(int);
void GXSetNumChans(int);
void GXSetChanCtrl(int, int, int, int, int, int, int);
void GXSetTevOrder(int, int, int, int);
void GXSetNumTevStages(int);
void GXSetBlendMode(int, int, int, int);
void GXSetZMode(int, int, int);
void GXSetProjection(mtx44, int type);
void GXLoadPosMtxImm(mtx, int);



TextBlock* TextBlockCreate(int, int, int, int, int, int);
void TextBlockConfigure(TextBlock*, int strid, int x, int y, int);
void TextBlockDraw(void* subsystem, mtx, int color, int);
void TextBlockDelete(TextBlock*);

void add_to_render_queue(float depth, void* drawer, int arg, int priority);


#endif