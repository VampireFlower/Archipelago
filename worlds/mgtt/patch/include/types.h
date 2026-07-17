// primtiive types

#ifndef MGTT_TYPES_H
#define MGTT_TYPES_H

// big bool
typedef enum BOOL {
    false,
    true
} BOOL;

// little bool
typedef unsigned char bool;

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned int uint;
typedef unsigned short ushort;



typedef float mtx[3][4];
typedef float mtx44[4][4];


typedef struct {
    float x;
    float y;
    float z;
} Vec3;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Quaternion;





#endif