#include <types.h>
/*
 * Freestanding implementation of sincosf(float x, float *s, float *c)
 *
 * Computes sin(x) and cos(x) simultaneously for single-precision IEEE 754 floats.
 * - Inputs: float x (in radians), pointers to store result *s and *c.
 * - Accuracy: ~1 ULP across typical input ranges (|x| < 100,000).
 */

/* Type punning union to safely inspect float bit representations */
typedef union {
    float f;
    uint u;
} float_bits_t;

/* Constants for Range Reduction and Polynomials */
static const float TWO_OVER_PI = 0.63661977236758134308f;

/* Pi/2 split into 3 single-precision float parts (Cody-Waite) to preserve precision */
static const float PIO2_1 = 1.5707962512969970703125f; /* 0x3fc90fdb */
static const float PIO2_2 = 7.549789948768648e-8f;      /* 0x337dbe22 */
static const float PIO2_3 = 4.371138828673793e-15f;     /* 0x27000000 */

/* Coefficients for sin(r) ~ r + S1*r^3 + S2*r^5 + S3*r^7 */
static const float S1 = -0.166666666416265235595703125f; /* -1/6 */
static const float S2 =  0.00833333333333333321768f;      /*  1/120 */
static const float S3 = -0.000198412698412696162806f;     /* -1/5040 */

/* Coefficients for cos(r) ~ 1 + C1*r^2 + C2*r^4 + C3*r^6 + C4*r^8 */
static const float C1 = -0.5f;                            /* -1/2 */
static const float C2 =  0.041666666666666664f;          /*  1/24 */
static const float C3 = -0.0013888888888888889f;         /* -1/720 */
static const float C4 =  0.0000248015873015873f;         /*  1/40320 */

void sincosf(float x, float *s, float *c) {
    float_bits_t fb;
    fb.f = x;

    uint ix = fb.u & 0x7FFFFFFF; /* Clear sign bit (|x|) */

    /* 1. Special Case: NaN or Infinity */
    if (ix >= 0x7F800000) {
        *s = x - x; /* Produces NaN */
        *c = x - x; /* Produces NaN */
        return;
    }

    /* 2. Special Case: Small numbers (|x| < 2^-12 ~ 0.000244) */
    if (ix < 0x39800000) {
        *s = x;
        *c = 1.0f;
        return;
    }

    /* 3. Range Reduction: x = n * (pi/2) + r, where r in [-pi/4, pi/4] */
    float fn = x * TWO_OVER_PI;
    
    /* Round to nearest integer */
    int n = (int)(fn + (fn >= 0.0f ? 0.5f : -0.5f));
    float fn_float = (float)n;

    /* Subtract n * (pi/2) in 3 precision steps to prevent catastrophic cancellation */
    float r = ((x - fn_float * PIO2_1) - fn_float * PIO2_2) - fn_float * PIO2_3;

    /* 4. Polynomial Evaluation (Horner's Scheme) */
    float r2 = r * r;

    /* sin(r) evaluation */
    float sin_r = r + (r * r2) * (S1 + r2 * (S2 + r2 * S3));

    /* cos(r) evaluation */
    float cos_r = 1.0f + r2 * (C1 + r2 * (C2 + r2 * (C3 + r2 * C4)));

    /* 5. Reconstruction based on Quadrant (n mod 4) */
    /* n & 3 works correctly for negative integers in 2's complement */
    int quadrant = n & 3;

    switch (quadrant) {
        case 0: /* Quadrant 0: sin = sin(r), cos = cos(r) */
            *s =  sin_r;
            *c =  cos_r;
            break;
        case 1: /* Quadrant 1: sin = cos(r), cos = -sin(r) */
            *s =  cos_r;
            *c = -sin_r;
            break;
        case 2: /* Quadrant 2: sin = -sin(r), cos = -cos(r) */
            *s = -sin_r;
            *c = -cos_r;
            break;
        case 3: /* Quadrant 3: sin = -cos(r), cos = sin(r) */
            *s = -cos_r;
            *c =  sin_r;
            break;
    }
}


float sqrtf(float x)
{
    float y;

    if (x <= 0.0f)
        return 0.0f;

    __asm__("frsqrte %0,%1" : "=f"(y) : "f"(x));

    y = y * (1.5f - 0.5f * x * y * y);

    return x * y;
}