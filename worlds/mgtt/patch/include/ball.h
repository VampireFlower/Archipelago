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
    int Ox10;
    float Ox14;
    float Ox18;
    float Ox1c;
    float Ox20;
    byte spin_type;
    byte Ox25;
    Vec3f Ox28;
    Vec3f Ox34;
    int Ox40;
    float Ox44;
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
    byte Ox76;
    byte Ox77;
    byte Ox78;
    Vec3f Ox7c;
    byte Ox88;
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
    int Oxc4;
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
    int Ox4;
    HoleScore scores[18];
    
    byte Ox98[28]; // referenced by 8040e2a0
    BallRestingState restingState;
    BallFlyingState flyingState;

    byte nStrokes;
    byte nPutts;
    byte completionStatus; // 0: none, 1: normal, 2: give up
    byte Ox1bf;
    byte Ox1c0;
    byte Ox1c1;
    byte Ox1c2;
    byte Ox1c3;
    byte Ox1c4;

} GolfBall;


// these belong to the sim line, not the ball
extern BallRestingState BallRestSimulated; // 80502768
extern int TotalBallFlightDuration; // 80502800



void CopyFlyingToRest(BallFlyingState*, BallRestingState*);
void CopyRestToFlying(BallRestingState*, BallFlyingState*);

#endif