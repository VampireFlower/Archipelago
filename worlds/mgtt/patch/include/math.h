#include <types.h>

#ifndef MGTT_MATH_H
#define MGTT_MATH_H


#define PI 3.1415927f
#define DegToRad(a)   ( (a) * 0.01745329252f )
#define RadToDeg(a)   ( (a) * 57.29577951f )


float hypotf(float, float);
float hypot3f(float, float, float); // just use PSVECMag

float sqrtf(float x);
float fabsf(float);
float powf(float, float);

float cosf(float);
float sinf(float);
float tanf(float);
float atan2f(float x, float y);


#endif