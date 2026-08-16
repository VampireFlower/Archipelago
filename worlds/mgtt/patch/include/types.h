// primitive types

#ifndef MGTT_TYPES_H
#define MGTT_TYPES_H

typedef int* ptr;

// integer sized boolean
typedef enum BOOL : unsigned int {
    FALSE,
    TRUE
} BOOL;

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned int uint;
typedef unsigned short ushort;

typedef unsigned int size_t;


typedef struct {
    short x;
    short y;
    short z;
} Vec3s;

typedef struct {
    float x;
    float y;
    float z;
} Vec3f;

typedef struct {
    double x;
    double y;
    double z;
} Vec3d;

typedef struct {
    union {
       short x;
       short u;
    };
    union {
       short y;
       short v;
    };
} Vec2s;

typedef struct {
    union {
       float x;
       float u;
    };
    union {
       float y;
       float v;
    };
} Vec2f;

typedef struct {
    union {
       double x;
       double u;
    };
    union {
       double y;
       double v;
    };
} Vec2d;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Quaternion;

#endif