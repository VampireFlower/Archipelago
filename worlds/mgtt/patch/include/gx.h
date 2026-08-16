#include <types.h>
#include <mtx.h>

#ifndef MGTT_GX_H
#define MGTT_GX_H

#ifdef __cplusplus
extern "C" {
#endif


void GXSetTexCoordGen2(int,int,int,int);
void GXSetNumTexGens(int);
void GXSetCullMode(int);
void GXSetNumChans(int);
void GXSetChanCtrl(int, int, int, int, int, int, int);
void GXSetTevOrder(int, int, int, int);
void GXSetNumTevStages(int);
void GXSetBlendMode(int, int, int, int);
void GXSetZMode(int, int, int);
void GXSetProjection(Mtx44, int type);
void GXLoadPosMtxImm(Mtx, int);

#ifdef __cplusplus
}
#endif

#endif