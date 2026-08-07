#include <ball.h>
#include <math.h>
#include <mgtt.h>
#include <mtx.h>


extern bool ResolveCameraCollision(
    float sweepRadius,
    float groundClearance,
    Vec3f* eye,
    Vec3f* target,
    BOOL verticalOnly
);


#define HOLD_FRAMES                   15u
#define CATCHUP_FRAMES                40u

#define FOV_KICK                      -1.5f

#define CAMERA_SWEEP_RADIUS           0.48f
#define CAMERA_GROUND_CLEARANCE       3.0f

#define DEFAULT_ORBIT_DISTANCE        9.60f
#define MIN_DIRECTION_SPEED           0.05f

#define DIRECTION_RESPONSE            0.10f
#define IMPACT_DIRECTION_RESPONSE     0.025f

#define LOOK_AHEAD_SCALE              0.55f
#define MAX_LOOK_AHEAD                3.20f

#define DESCENT_SPEED_FOR_MAX_TILT    1.50f
#define MAX_DESCENT_LOOK_DOWN         5.0f

#define DESCENT_TILT_RESPONSE         0.28f
#define IMPACT_TILT_RESPONSE          0.06f

#define CATCHUP_MAX_HORIZONTAL_STEP   8.00f
#define CATCHUP_MAX_VERTICAL_STEP     2.00f

#define FOLLOW_EYE_XZ_RESPONSE        0.55f
#define FOLLOW_EYE_Y_RESPONSE         0.28f
#define FOLLOW_TARGET_XZ_RESPONSE     0.65f
#define FOLLOW_TARGET_Y_RESPONSE      0.35f

#define FOLLOW_MAX_HORIZONTAL_STEP    6.40f
#define FOLLOW_MAX_VERTICAL_STEP      1.60f

#define IMPACT_EYE_XZ_RESPONSE        0.25f
#define IMPACT_EYE_Y_RESPONSE         0.08f
#define IMPACT_TARGET_XZ_RESPONSE     0.30f
#define IMPACT_TARGET_Y_RESPONSE      0.10f

#define IMPACT_MAX_HORIZONTAL_STEP    3.20f
#define IMPACT_MAX_VERTICAL_STEP      0.45f

#define IMPACT_DAMP_FRAMES            10u

#define MIN_LOOK_DISTANCE             1.00f

/*
 * Prevents the camera's forward direction from approaching the world-up
 * vector too closely.
 *
 * tan(70 degrees) is approximately 2.75.
 */
#define MAX_FORWARD_VERTICAL_RATIO    2.75f


typedef struct {
    bool initialized;
    GolfBall* ball;

    uint lastShotUpdateCount;
    int lastGroundCollisionCount;
    int impactDampFrames;

    Vec3f heldEye;
    Vec3f heldTarget;
    Vec3f heldForward;

    Vec3f currentEye;
    Vec3f currentTarget;

    /*
     * Horizontal unit vector pointing from the ball toward the camera.
     * Its inverse therefore points in the ball's forward direction.
     */
    Vec3f backDirection;

    float heldLookDistance;
    float orbitDistance;
    float orbitHeight;
    float baseFov;
    float descentLookDown;
} BallCameraState;


static BallCameraState sCameraState;


float clampf(float, float, float);


static void VECLerp(
    const Vec3f* from,
    const Vec3f* to,
    float amount,
    Vec3f* result
)
{
    result->x =
        from->x +
        (to->x - from->x) * amount;

    result->y =
        from->y +
        (to->y - from->y) * amount;

    result->z =
        from->z +
        (to->z - from->z) * amount;
}


/*
 * Interpolates two normalized directions.
 *
 * Negating the destination when the dot product is negative prevents the
 * interpolation from passing through a zero-length direction.
 */
static void DirectionNlerp(
    Vec3f* from,
    Vec3f* to,
    float amount,
    Vec3f* result
)
{
    Vec3f adjustedTo;
    Vec3f blended;
    float dot;

    adjustedTo = *to;
    dot = VECDotProduct(from, to);

    if (dot < 0.0f) {
        adjustedTo.x = -adjustedTo.x;
        adjustedTo.y = -adjustedTo.y;
        adjustedTo.z = -adjustedTo.z;
    }

    blended.x =
        from->x +
        (adjustedTo.x - from->x) * amount;

    blended.y =
        from->y +
        (adjustedTo.y - from->y) * amount;

    blended.z =
        from->z +
        (adjustedTo.z - from->z) * amount;

    VECNormalize(&blended, result);
}


/*
 * Keeps the forward direction from becoming nearly parallel to world-up.
 *
 * A conventional look-at matrix computes its right vector from:
 *
 *     right = forward x worldUp
 *
 * That cross product becomes unstable when forward is almost vertical.
 */
static void LimitForwardPitch(Vec3f* forward)
{
    float horizontalLength;
    float maximumVertical;
    float length;

    horizontalLength = hypotf(
        forward->x,
        forward->z
    );

    /*
     * Supply a horizontal component if the direction has become completely
     * vertical.
     */
    if (horizontalLength < 0.001f) {
        forward->z = -0.001f;

        horizontalLength = hypotf(
            forward->x,
            forward->z
        );
    }

    maximumVertical =
        horizontalLength *
        MAX_FORWARD_VERTICAL_RATIO;

    forward->y = clampf(
        forward->y,
        -maximumVertical,
        maximumVertical
    );

    length = VECMag(forward);

    if (length > 0.001f) {
        forward->x /= length;
        forward->y /= length;
        forward->z /= length;
    }
}


/*
 * Follows a point with independent horizontal and vertical behavior.
 *
 * Keeping Y separate from XZ prevents an abrupt landing-height correction
 * from producing an equally abrupt horizontal camera correction.
 */
static void FollowPoint(
    Vec3f* current,
    const Vec3f* wanted,
    float horizontalResponse,
    float verticalResponse,
    float maxHorizontalStep,
    float maxVerticalStep
)
{
    float dx;
    float dz;
    float horizontalStep;
    float scale;
    float dy;

    dx = (wanted->x - current->x) * horizontalResponse;

    dz = (wanted->z - current->z) * horizontalResponse;

    horizontalStep = hypotf(dx, dz);

    if (horizontalStep > maxHorizontalStep) {
        scale =
            maxHorizontalStep /
            horizontalStep;

        dx *= scale;
        dz *= scale;
    }

    current->x += dx;
    current->z += dz;

    dy =
        (wanted->y - current->y) *
        verticalResponse;

    dy = clampf(
        dy,
        -maxVerticalStep,
        maxVerticalStep
    );

    current->y += dy;
}


/*
 * An ease-out-warped smoothstep.
 *
 * This begins with zero velocity, accelerates quickly, and completes most of
 * the catch-up during the first half of the transition.
 */
static float CatchupEase(float time)
{
    float remaining;
    float warpedTime;

    time = clampf(time, 0.0f, 1.0f);

    remaining = 1.0f - time;
    warpedTime = 1.0f - remaining * remaining;

    return
        warpedTime *
        warpedTime *
        (3.0f - 2.0f * warpedTime);
}


/*
 * Smooth FOV pulse which peaks early in the catch-up.
 */
static float CatchupFovPulse(float time)
{
    float remaining;
    float warpedTime;
    float warpedRemaining;

    time = clampf(time, 0.0f, 1.0f);

    remaining = 1.0f - time;
    warpedTime = 1.0f - remaining * remaining;
    warpedRemaining = 1.0f - warpedTime;

    return
        16.0f *
        warpedTime *
        warpedTime *
        warpedRemaining *
        warpedRemaining;
}


static void InitializeBallCamera(
    BallCameraState* state,
    GolfBall* ball,
    uint shotUpdateCount
)
{
    Vec3f horizontalOffset;
    Vec3f heldView;

    float horizontalDistance;

    state->initialized = true;
    state->ball = ball;

    state->lastShotUpdateCount = shotUpdateCount;

    state->lastGroundCollisionCount = ball->flyingState.groundCollisionCount;

    state->impactDampFrames = 0;
    state->descentLookDown = 0.0f;

    state->heldEye = camera.eye;
    state->heldTarget = camera.target;

    state->currentEye = camera.eye;
    state->currentTarget = camera.target;

    state->baseFov = camera.fov;

    VECSubtract(&state->heldTarget, &state->heldEye, &heldView);

    state->heldLookDistance = VECMag(&heldView);

    if (state->heldLookDistance < MIN_LOOK_DISTANCE) {
        state->heldLookDistance = MIN_LOOK_DISTANCE;

        state->heldForward.x = 0.0f;
        state->heldForward.y = 0.0f;
        state->heldForward.z = -1.0f;
    }
    else {
        state->heldForward.x = heldView.x / state->heldLookDistance;
        state->heldForward.y = heldView.y / state->heldLookDistance;
        state->heldForward.z = heldView.z / state->heldLookDistance;
    }

    LimitForwardPitch(&state->heldForward);

    horizontalOffset.x = camera.eye.x - ball->restingState.position.x;

    horizontalOffset.y = 0.0f;
    
    horizontalOffset.z = camera.eye.z - ball->restingState.position.z;

    horizontalDistance = VECMag(&horizontalOffset);

    if (horizontalDistance > 0.001f) {
        state->orbitDistance = horizontalDistance;

        state->backDirection.x = horizontalOffset.x / horizontalDistance;

        state->backDirection.y = 0.0f;

        state->backDirection.z = horizontalOffset.z / horizontalDistance;
    }
    else {
        state->orbitDistance = DEFAULT_ORBIT_DISTANCE;

        state->backDirection.x = 0.0f;
        state->backDirection.y = 0.0f;
        state->backDirection.z = 1.0f;
    }

    state->orbitHeight = camera.eye.y - ball->restingState.position.y;
}


/*
 * Gradually rotates the trailing direction toward the inverse of the ball's
 * horizontal velocity.
 *
 * A direction reversal of more than 90 degrees is ignored. This prevents a
 * bounce or wall impact from whipping the camera around the ball.
 */
static float UpdateBackDirection(
    BallCameraState* state,
    const Vec3f* velocity,
    bool impactDamping
)
{
    Vec3f desiredBack;
    Vec3f mixedDirection;

    float horizontalSpeed;
    float dot;
    float response;
    float mixedLength;

    horizontalSpeed = hypotf(
        velocity->x,
        velocity->z
    );

    if (horizontalSpeed <= MIN_DIRECTION_SPEED) {
        return horizontalSpeed;
    }

    desiredBack.x =
        -velocity->x /
        horizontalSpeed;

    desiredBack.y = 0.0f;

    desiredBack.z =
        -velocity->z /
        horizontalSpeed;

    dot =
        state->backDirection.x * desiredBack.x +
        state->backDirection.z * desiredBack.z;

    if (dot <= 0.0f) {
        return horizontalSpeed;
    }

    response = impactDamping
        ? IMPACT_DIRECTION_RESPONSE
        : DIRECTION_RESPONSE;

    mixedDirection.x =
        state->backDirection.x +
        (
            desiredBack.x -
            state->backDirection.x
        ) * response;

    mixedDirection.y = 0.0f;

    mixedDirection.z =
        state->backDirection.z +
        (
            desiredBack.z -
            state->backDirection.z
        ) * response;

    mixedLength = hypotf(
        mixedDirection.x,
        mixedDirection.z
    );

    if (mixedLength > 0.001f) {
        state->backDirection.x =
            mixedDirection.x /
            mixedLength;

        state->backDirection.z =
            mixedDirection.z /
            mixedLength;
    }

    return horizontalSpeed;
}


static void BuildFollowPose(
    BallCameraState* state,
    const BallFlyingState* flyingState,
    Vec3f* ballPosition,
    bool impactDamping,
    Vec3f* wantedEye,
    Vec3f* wantedTarget
)
{
    float horizontalSpeed;
    float lookAheadDistance;

    float descentAmount;
    float wantedLookDown;
    float tiltResponse;

    horizontalSpeed = UpdateBackDirection(
        state,
        &flyingState->velocity,
        impactDamping
    );

    wantedEye->x =
        ballPosition->x +
        state->backDirection.x *
        state->orbitDistance;

    wantedEye->y =
        ballPosition->y +
        state->orbitHeight;

    wantedEye->z =
        ballPosition->z +
        state->backDirection.z *
        state->orbitDistance;

    lookAheadDistance = clampf(
        horizontalSpeed * LOOK_AHEAD_SCALE,
        0.0f,
        MAX_LOOK_AHEAD
    );

    /*
     * backDirection points backward, so subtracting it looks forward.
     */
    wantedTarget->x =
        ballPosition->x -
        state->backDirection.x *
        lookAheadDistance;

    wantedTarget->y =
        ballPosition->y;

    wantedTarget->z =
        ballPosition->z -
        state->backDirection.z *
        lookAheadDistance;

    descentAmount = clampf(
        -flyingState->velocity.y /
        DESCENT_SPEED_FOR_MAX_TILT,
        0.0f,
        1.0f
    );

    wantedLookDown =
        MAX_DESCENT_LOOK_DOWN *
        descentAmount *
        descentAmount;

    tiltResponse = impactDamping
        ? IMPACT_TILT_RESPONSE
        : DESCENT_TILT_RESPONSE;

    state->descentLookDown +=
        (
            wantedLookDown -
            state->descentLookDown
        ) * tiltResponse;

    wantedTarget->y -= state->descentLookDown;
}


/*
 * Constructs the catch-up pose by interpolating:
 *
 *   1. eye position;
 *   2. normalized forward direction;
 *   3. eye-to-target distance.
 *
 * Interpolating eye and target independently can cause their difference to
 * become nearly zero or nearly vertical, which makes a conventional look-at
 * matrix's up vector unstable.
 */
static void BuildCatchupPose(
    BallCameraState* state,
    Vec3f* wantedEye,
    Vec3f* wantedTarget,
    float amount,
    Vec3f* transitionEye,
    Vec3f* transitionTarget
)
{
    Vec3f wantedView;
    Vec3f wantedForward;
    Vec3f transitionForward;

    float wantedLookDistance;
    float transitionLookDistance;

    VECLerp(
        &state->heldEye,
        wantedEye,
        amount,
        transitionEye
    );

    VECSubtract(wantedTarget, wantedEye, &wantedView);

    wantedLookDistance = VECMag(&wantedView);

    VECNormalize(&wantedView, &wantedForward);

    LimitForwardPitch(&wantedForward);

    DirectionNlerp(
        &state->heldForward,
        &wantedForward,
        amount,
        &transitionForward
    );

    LimitForwardPitch(&transitionForward);

    transitionLookDistance =
        state->heldLookDistance +
        (
            wantedLookDistance -
            state->heldLookDistance
        ) * amount;

    if (transitionLookDistance < MIN_LOOK_DISTANCE) {
        transitionLookDistance =
            MIN_LOOK_DISTANCE;
    }

    transitionTarget->x =
        transitionEye->x +
        transitionForward.x *
        transitionLookDistance;

    transitionTarget->y =
        transitionEye->y +
        transitionForward.y *
        transitionLookDistance;

    transitionTarget->z =
        transitionEye->z +
        transitionForward.z *
        transitionLookDistance;
}


/*
 * Ensures the resolved eye and target still describe a valid view direction.
 *
 * Collision resolution can move the eye onto or extremely close to the
 * target. That would make the following look-at matrix degenerate.
 */
static void ValidateResolvedView(Vec3f* eye, Vec3f* target)
{
    Vec3f view;
    Vec3f forward;

    float distance;

    PSVECSubtract(target, eye, &view);

    distance = PSVECMag(&view);

    if (distance >= MIN_LOOK_DISTANCE) return;

    VECNormalize(&view, &forward);

    LimitForwardPitch(&forward);

    target->x =
        eye->x +
        forward.x *
        MIN_LOOK_DISTANCE;

    target->y =
        eye->y +
        forward.y *
        MIN_LOOK_DISTANCE;

    target->z =
        eye->z +
        forward.z *
        MIN_LOOK_DISTANCE;
}


bool SeekingCamera(GolfBall* ball)
{
    BallCameraState* state;
    BallFlyingState* flyingState;

    Vec3f* ballPosition;

    Vec3f wantedEye;
    Vec3f wantedTarget;

    Vec3f transitionEye;
    Vec3f transitionTarget;

    uint frame;

    bool newCameraFrame;
    bool impactDamping;
    bool inCatchup;

    float catchupTime;
    float catchupAmount;
    float fovPulse;

    state = &sCameraState;
    flyingState = &ball->flyingState;
    frame = flyingState->shotUpdateCount;


    if (ShotReplayCount != 0) {
        state->initialized = false;
        return false;
    }

    // if you start within 32 units and end inside 0.96 units of the
    // cup, the game will use a camera focusing on the cup 1/4 times
    TRY_BEHIND_CUP_CAMERA = false;

    // selectballflightcamera returns before this value is set to false
    // on first shot, so it could be true from a previous replay
    TRY_ACTION_CUP_CAMERA = false;

    // Disable special hazard cameras
    *(int*)0x80514c90 = 0;
    
    // action cup or something idk
    *(int*)0x80523fb8 = 0;

    /*
     * A reused GolfBall object is detected by shotUpdateCount returning to
     * zero. Tracking the pointer also handles switching to another ball.
     */
    if (
        !state->initialized ||
        state->ball != ball ||
        frame < state->lastShotUpdateCount
    ) {
        InitializeBallCamera(state, ball, frame);
    }

    newCameraFrame = frame != state->lastShotUpdateCount;

    if (flyingState->groundCollisionCount != state->lastGroundCollisionCount) {
        
        state->lastGroundCollisionCount = flyingState->groundCollisionCount;
        state->impactDampFrames = IMPACT_DAMP_FRAMES;
    
    }

    impactDamping = state->impactDampFrames > 0;

    /*
     * shotUpdateCount zero may precede a valid flying position.
     */
    ballPosition = frame == 0
        ? &ball->restingState.position
        : &flyingState->position;

    BuildFollowPose(
        state,
        flyingState,
        ballPosition,
        impactDamping,
        &wantedEye,
        &wantedTarget
    );

    /*
     * Hold the launch composition for ten complete frames.
     */
    if (frame < HOLD_FRAMES) {
        state->currentEye = state->heldEye;

        state->currentTarget = state->heldTarget;

        camera.eye = state->currentEye;

        camera.target = state->currentTarget;

        camera.fov = state->baseFov;

        if (newCameraFrame && state->impactDampFrames > 0) {
            --state->impactDampFrames;
        }

        state->lastShotUpdateCount = frame;

        return true;
    }

    inCatchup = frame < HOLD_FRAMES + CATCHUP_FRAMES;

    if (inCatchup) {
        /*
         * Frames 10 through 54 produce values from 1/45 through 45/45.
         */
        catchupTime =
            (float)(
                frame -
                HOLD_FRAMES +
                1u
            ) /
            (float)CATCHUP_FRAMES;

        catchupAmount = CatchupEase(catchupTime);

        BuildCatchupPose(
            state,
            &wantedEye,
            &wantedTarget,
            catchupAmount,
            &transitionEye,
            &transitionTarget
        );

        FollowPoint(
            &state->currentEye,
            &transitionEye,
            impactDamping
                ? IMPACT_EYE_XZ_RESPONSE
                : FOLLOW_EYE_XZ_RESPONSE,
            impactDamping
                ? IMPACT_EYE_Y_RESPONSE
                : FOLLOW_EYE_Y_RESPONSE,
            impactDamping
                ? IMPACT_MAX_HORIZONTAL_STEP
                : CATCHUP_MAX_HORIZONTAL_STEP,
            impactDamping
                ? IMPACT_MAX_VERTICAL_STEP
                : CATCHUP_MAX_VERTICAL_STEP
        );

        FollowPoint(
            &state->currentTarget,
            &transitionTarget,
            impactDamping
                ? IMPACT_TARGET_XZ_RESPONSE
                : FOLLOW_TARGET_XZ_RESPONSE,
            impactDamping
                ? IMPACT_TARGET_Y_RESPONSE
                : FOLLOW_TARGET_Y_RESPONSE,
            impactDamping
                ? IMPACT_MAX_HORIZONTAL_STEP
                : CATCHUP_MAX_HORIZONTAL_STEP,
            impactDamping
                ? IMPACT_MAX_VERTICAL_STEP
                : CATCHUP_MAX_VERTICAL_STEP
        );

        fovPulse = CatchupFovPulse(catchupTime);

        camera.fov = state->baseFov + FOV_KICK * fovPulse;
    }
    else {
        if (impactDamping) {
            FollowPoint(
                &state->currentEye,
                &wantedEye,
                IMPACT_EYE_XZ_RESPONSE,
                IMPACT_EYE_Y_RESPONSE,
                IMPACT_MAX_HORIZONTAL_STEP,
                IMPACT_MAX_VERTICAL_STEP
            );

            FollowPoint(
                &state->currentTarget,
                &wantedTarget,
                IMPACT_TARGET_XZ_RESPONSE,
                IMPACT_TARGET_Y_RESPONSE,
                IMPACT_MAX_HORIZONTAL_STEP,
                IMPACT_MAX_VERTICAL_STEP
            );
        }
        else {
            FollowPoint(
                &state->currentEye,
                &wantedEye,
                FOLLOW_EYE_XZ_RESPONSE,
                FOLLOW_EYE_Y_RESPONSE,
                FOLLOW_MAX_HORIZONTAL_STEP,
                FOLLOW_MAX_VERTICAL_STEP
            );

            FollowPoint(
                &state->currentTarget,
                &wantedTarget,
                FOLLOW_TARGET_XZ_RESPONSE,
                FOLLOW_TARGET_Y_RESPONSE,
                FOLLOW_MAX_HORIZONTAL_STEP,
                FOLLOW_MAX_VERTICAL_STEP
            );
        }

        camera.fov += (state->baseFov - camera.fov) * 0.25f;
    }



    /*
     * The original collision routine allocates approximately 7 KiB of
     * temporary query storage on every invocation, so call it only once.
     */
    ResolveCameraCollision(
        CAMERA_SWEEP_RADIUS,
        CAMERA_GROUND_CLEARANCE,
        &state->currentEye,
        &state->currentTarget,
        true
    );

    camera.eye = state->currentEye;

    camera.target = state->currentTarget;

    if (
        newCameraFrame &&
        state->impactDampFrames > 0
    ) {
        --state->impactDampFrames;
    }

    state->lastShotUpdateCount = frame;

    return true;
}