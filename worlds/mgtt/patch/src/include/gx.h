#include <types.h>

#ifndef MGTT_GX_H
#define MGTT_GX_H



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









#endif