#include <types.h>

#ifndef MGTT_MATH_H
#define MGTT_MATH_H

#define PI 3.1415927f


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