#include <types.h>


float sqrtf(float x)
{
    float y;

    if (x <= 0.0f)
        return 0.0f;

    __asm__("frsqrte %0,%1" : "=f"(y) : "f"(x));

    y = y * (1.5f - 0.5f * x * y * y);

    return x * y;
}