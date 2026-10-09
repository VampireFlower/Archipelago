#include <types.h>

#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#define PI 3.1415927f
#define DegToRad(a)   ( (a) * 0.01745329252f )
#define RadToDeg(a)   ( (a) * 57.29577951f )


constfunc float hypotf(float, float);
constfunc float hypot3f(float, float, float);

constfunc float sqrtf(float x);
constfunc float fabsf(float);
constfunc float powf(float, float);

constfunc float cosf(float);
constfunc float sinf(float);
constfunc float tanf(float);
constfunc float atan2f(float x, float y);

constfunc float clampf(float x, float minimum, float maximum);

#ifdef __cplusplus
}
#endif