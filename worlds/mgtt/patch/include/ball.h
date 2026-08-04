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
    Vec3f position;
    Quaternion rotation;
    Vec3f contactNormal;
    bool isHoled;
    byte lieFlags; // Course geometry reports lie ID along with 2 boolean flags. OB and hazard related
    bool isUnderwater;
    LieID lie;
    uint lieQuality;
} BallRestingState;

typedef enum : uint {
    BALL_STATE_0,
    BALL_INACTIVE,
    BALL_FLYING,
    BALL_ROLLING,
    BALL_IN_CUP,
    BALL_STATE_5,
    BALL_MARKER,
    BALL_PIPE_LERP,
    BALL_PIPE_FLYING
} BallMotion;


typedef struct BallFlyingState {
    Vec3f pipeExitPosition;
    int pipeLerpFramesRemaining;
    int field_0x10;
    float field_0x14;
    float field_0x18;
    float field_0x1c;
    float field_0x20;
    byte spin_type;
    byte field_0x25;
    Vec3f field_0x28;
    Vec3f field_0x34;
    int field_0x40;
    float field_0x44;
    int flightContext;
    BallMotion state;
    Vec3f velocity;
    float spinrate;
    Quaternion rotation;
    sbyte owner;
    bool isHoled;
    bool pinShot;
    bool flagShot;
    byte assistRelated;
    bool underwater;
    byte field_0x76;
    byte field_0x77;
    byte field_0x78;
    Vec3f field_0x7c;
    byte field_0x88;
    byte booSFXcooldown;
    int groundCollisionCount;
    uint groundFramesConsecutive;
    int physicsStepIndex;
    uint shotUpdateCount;
    Vec3f position;
    Vec3f contactNormal;
    LieID firstContact;
    LieID lie; // set by contact surface when rolling. during flight, raycast to the ground once every 8 frames if the ball is moving downwards
    int lieQuality;
    uint groundHistoryBitmap;
    int field_0xc4;
    byte lieFlags;
    struct ShotParameters* shotParameters;
    int specialHazard;
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


// these belong to the sim line, not the ball
extern BallRestingState BallRestSimulated; // 80502768
extern int TotalBallFlightDuration; // 80502800



void CopyFlyingToRest(BallFlyingState*, BallRestingState*);
void CopyRestToFlying(BallRestingState*, BallFlyingState*);

#endif