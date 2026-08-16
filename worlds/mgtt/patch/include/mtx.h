#include <types.h>

#ifndef MGTT_MTX_H
#define MGTT_MTX_H

#ifdef __cplusplus
extern "C" {
#endif



typedef float Mtx[3][4];
typedef float Mtx44[4][4];



void C_MTXOrtho(
    Mtx44 m, 
    float top, 
    float bottom, 
    float left, 
    float right, 
    float near, 
    float far
);


void PSMTXIdentity  ( Mtx m );
void PSMTXCopy      ( Mtx src, Mtx dst );
void PSMTXConcat    ( Mtx a, Mtx b, Mtx ab );
void PSMTXTranspose ( Mtx src, Mtx xPose );
uint PSMTXInverse   ( Mtx src, Mtx inv );
uint PSMTXInvXpose  ( Mtx src, Mtx invX );

void PSMTXMultVec   ( Mtx m, Vec3f* src, Vec3f* dst );
void PSMTXMultVecSR ( Mtx m, Vec3f* src, Vec3f* dst );


void  PSVECAdd            ( Vec3f* a, Vec3f* b, Vec3f* ab );
void  PSVECSubtract       ( Vec3f* a, Vec3f* b, Vec3f* a_b );
void  PSVECScale          ( Vec3f* src, Vec3f* dst, float scale );
void  PSVECNormalize      ( Vec3f* src, Vec3f* unit );
float PSVECSquareMag      ( Vec3f* v );
float PSVECMag            ( Vec3f* v );
float PSVECDotProduct     ( Vec3f* a, Vec3f* b );
void  PSVECCrossProduct   ( Vec3f* a, Vec3f* b, Vec3f* axb );
float PSVECSquareDistance ( Vec3f* a, Vec3f* b );
float PSVECDistance       ( Vec3f* a, Vec3f* b );



#define VECAdd                  PSVECAdd
#define VECSubtract             PSVECSubtract
#define VECScale                PSVECScale
#define VECNormalize            PSVECNormalize
#define VECSquareMag            PSVECSquareMag
#define VECMag                  PSVECMag
#define VECDotProduct           PSVECDotProduct
#define VECCrossProduct         PSVECCrossProduct
#define VECSquareDistance       PSVECSquareDistance
#define VECDistance             PSVECDistance


#ifdef __cplusplus
}
#endif

#endif // MGTT_MTX_H