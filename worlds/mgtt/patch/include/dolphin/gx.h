#include <types.h>
#include <mtx.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GXTexObj {
    int Ox0;
    int Ox4;
    int Ox8;
    int Oxc;
    int Ox10;
    int Ox14;
    int Ox18;
    int Ox1c;
} GXTexObj;

typedef enum GXProjectionType : uint {
    GX_PERSPECTIVE,
    GX_ORTHOGRAPHIC,
} GXProjectionType;


void GXSetTexCoordGen2(int,int,int,int);
void GXSetNumTexGens(int);
void GXSetCullMode(int);
void GXSetNumChans(int);
void GXSetChanCtrl(int, int, int, int, int, int, int);
void GXSetTevOrder(int, int, int, int);
void GXSetNumTevStages(int);
void GXSetBlendMode(int, int, int, int);
void GXSetZMode(int, int, int);
void GXSetProjection(Mtx44, GXProjectionType);
void GXLoadPosMtxImm(Mtx, int);




#ifdef __cplusplus
}
#endif