#include <types.h>

#ifndef MGTT_MATH_H
#define MGTT_MATH_H

#define PI 3.1415927f


float hypot(float, float);
float hypot3(float, float, float); // just use PSVECMag

float sqrtf(float x);
float fabsf(float);
float powf(float, float);

float cosf(float);
float sinf(float);
float tanf(float);
void  sincosf(float x, float *s, float *c);
float atan2f(float x, float y);


#endif