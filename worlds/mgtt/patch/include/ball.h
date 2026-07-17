#include <types.h>
#include <lies.h>

#ifndef MGTT_BALL_H
#define MGTT_BALL_H


typedef enum {
    SPIN_NONE,
    SPIN_AA,
    SPIN_AB,
    SPIN_BB,
    SPIN_BA
} SpinType;


typedef enum {
    SHOT_LONG = 0,
    SHOT_MIDDLE = 1,
    SHOT_SHORT = 2,

    SHOT_POWER = 0,
    SHOT_NORMAL = 1,
    SHOT_APPROACH = 2,
} ShotType;






typedef struct BallRestingState {
    Vec3 position;
    Quaternion rotation;
    Vec3 contactNormal;
    bool isHoled;
    byte lieFlags; // Course geometry reports lie ID along with 2 boolean flags. They are related to ob and hazard
    bool isUnderwater;
    LieID lie;
    uint lieQuality;
} BallRestingState;


typedef struct BallFlyingState {
    struct ShotParameters* shotParameters;
} BallFlyingState;


typedef struct HoleScore {
    byte exitStatus; // overloaded field
    sbyte nPutts;
    byte nStrokes;
    int exitValue; // speed golf timer, coins collected, etc
} HoleScore;


typedef struct GolfBall {

    int modeRunningValue;
    int field_0x4;
    HoleScore scores[18];
    
    byte field_0x98[28]; // 8040e2a0
    BallRestingState restingState;
    BallFlyingState flyingState;

    byte nStrokes;
    byte nPutts;
    byte completionStatus; // 0: none, 1: normal, 2: give up
    byte field_0x1bf;
    byte field_0x1c0;
    byte field_0x1c1;
    byte field_0x1c2;
    byte field_0x1c3;
    byte field_0x1c4;

} GolfBall;


#endif