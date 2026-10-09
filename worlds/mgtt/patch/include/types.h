// primitive types

#pragma once

#define clz(x) __builtin_clz(x)
#define typeof(x) __typeof__(x)
#define purefunc [[gnu::pure]]
#define constfunc [[gnu::const]]

#define MAX(a, b)          \
    ({ typeof(a) _a = (a); \
       typeof(b) _b = (b); \
       _a > _b ? _a : _b; })

#define MIN(a, b)          \
    ({ typeof(a) _a = (a); \
       typeof(b) _b = (b); \
       _a > _b ? _b : _a; })

// integer sized boolean
typedef enum BOOL : unsigned int {
    FALSE,
    TRUE
} BOOL;

typedef unsigned char  byte;
typedef signed   char  sbyte;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned int   size_t;


typedef sbyte int8;
typedef short int16;
typedef int   int32;
typedef long long int64;

typedef byte   uint8;
typedef ushort uint16;
typedef uint   uint32;
typedef unsigned long long uint64;

typedef struct {
    byte r;
    byte g;
    byte b;
} RGB;

typedef struct {
    byte r;
    byte g;
    byte b;
    byte a;
} RGBA;

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
       byte x;
       byte u;
    };
    union {
       byte y;
       byte v;
    };
} Vec2b;

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

typedef __builtin_va_list va_list;

#define va_start(v, l)    __builtin_va_start(v, l)
#define va_end(v)         __builtin_va_end(v)
#define va_arg(v, t)      __builtin_va_arg(v, t)
#define va_copy(d, s)     __builtin_va_copy(d, s)

#define UNKNOWN_FUNCTION(name) void name(void);