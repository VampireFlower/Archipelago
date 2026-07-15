#include <mgtt.h>

float fabsf(float);
float powf(float, float);

void sincosf(float x, float *s, float *c)
{
    *s = sinf(x);
    *c = cosf(x);
}

typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;


#ifndef BALL_STEER_UPDATE_HZ
#define BALL_STEER_UPDATE_HZ 60.0f
#endif

/*
 * Based on an observed launch horizontal speed near 4.0.
 */
#ifndef BALL_STEER_FAST_SPEED
#define BALL_STEER_FAST_SPEED 4.0f
#endif

#ifndef BALL_STEER_SLOW_SPEED
#define BALL_STEER_SLOW_SPEED 0.5f
#endif

/*
 * Desired sideways velocity change per second.
 *
 * These are initial tuning values, not physically derived constants.
 * At high speed the control is deliberately weak.
 */
#ifndef BALL_STEER_FAST_ACCELERATION
#define BALL_STEER_FAST_ACCELERATION 0.04f
#endif

#ifndef BALL_STEER_SLOW_ACCELERATION
#define BALL_STEER_SLOW_ACCELERATION 0.75f
#endif

/*
 * Prevent the 1/speed conversion from producing an extreme turn.
 */
#ifndef BALL_STEER_MAX_DEGREES_PER_SECOND
#define BALL_STEER_MAX_DEGREES_PER_SECOND 120.0f
#endif

#ifndef BALL_STEER_MIN_HORIZONTAL_SPEED
#define BALL_STEER_MIN_HORIZONTAL_SPEED 0.02f
#endif

#ifndef BALL_STEER_STICK_DEADZONE
#define BALL_STEER_STICK_DEADZONE 0.08f
#endif

#ifndef BALL_STEER_DIRECTION_SIGN
#define BALL_STEER_DIRECTION_SIGN 1.0f
#endif

#define BALL_STEER_DEG_TO_RAD 0.01745329251994329577f


static float
clampf(float x, float minimum, float maximum)
{
    if (x < minimum)
        return minimum;

    if (x > maximum)
        return maximum;

    return x;
}


static inline float
sqrtf(float x)
{
    float y;

    if (x <= 0.0f)
        return 0.0f;

    __asm__("frsqrte %0,%1" : "=f"(y) : "f"(x));

    y = y * (1.5f - 0.5f * x * y * y);

    return x * y;
}


static float
steer_process_stick(float stick)
{
    float magnitude;

    //stick = clampf(stick, -1.0f, 1.0f);
    magnitude = fabsf(stick);

    if (magnitude <= BALL_STEER_STICK_DEADZONE)
        return 0.0f;

    magnitude =
        (magnitude - BALL_STEER_STICK_DEADZONE) /
        (1.0f - BALL_STEER_STICK_DEADZONE);

    /*
     * Quadratic response gives finer control near the stick center.
     */
    magnitude *= magnitude;

    return stick > 0.0f ? -magnitude : magnitude;
}


void ball_steer(Vec3* velocity, float stick_x)
{
    const float dt = 1.0f / BALL_STEER_UPDATE_HZ;

    float horizontal_speed_squared;
    float horizontal_speed;
    float speed_factor;
    float acceleration;
    float turn_rate;
    float maximum_turn_rate;
    float angle;
    float sine;
    float cosine;
    float new_x;
    float new_z;
    float new_length_squared;
    float correction;

    stick_x = steer_process_stick(stick_x);

    if (stick_x == 0.0f)
        return;

    horizontal_speed_squared =
        velocity->x * velocity->x +
        velocity->z * velocity->z;

    if (
        horizontal_speed_squared <=
        BALL_STEER_MIN_HORIZONTAL_SPEED *
        BALL_STEER_MIN_HORIZONTAL_SPEED
    ) {
        return;
    }

    horizontal_speed = sqrtf(horizontal_speed_squared);

    /*
     * 0 at fast speed, 1 at slow speed.
     */
    speed_factor =
        (BALL_STEER_FAST_SPEED - horizontal_speed) /
        (BALL_STEER_FAST_SPEED - BALL_STEER_SLOW_SPEED);

    speed_factor = clampf(speed_factor, 0.0f, 1.0f);

    /*
     * Quadratic curve:
     *
     * Steering stays weak through much of the high-speed flight,
     * then increases more strongly as the ball becomes slow.
     */
    speed_factor *= speed_factor;

    acceleration =
        BALL_STEER_FAST_ACCELERATION +
        (
            BALL_STEER_SLOW_ACCELERATION -
            BALL_STEER_FAST_ACCELERATION
        ) * speed_factor;

    /*
     * Lateral acceleration divided by speed gives angular velocity.
     */
    turn_rate = acceleration / horizontal_speed;

    maximum_turn_rate =
        BALL_STEER_MAX_DEGREES_PER_SECOND *
        BALL_STEER_DEG_TO_RAD;

    if (turn_rate > maximum_turn_rate)
        turn_rate = maximum_turn_rate;

    angle =
        BALL_STEER_DIRECTION_SIGN *
        stick_x *
        turn_rate *
        dt;

    sine = sinf(angle);
    cosine = cosf(angle);

    new_x =
        velocity->x * cosine +
        velocity->z * sine;

    new_z =
        velocity->z * cosine -
        velocity->x * sine;

    /*
     * Rotation should preserve magnitude exactly. Correct only any
     * floating-point increase, satisfying the no-speed-increase rule.
     */
    new_length_squared =
        new_x * new_x +
        new_z * new_z;

    if (new_length_squared > horizontal_speed_squared) {
        correction =
            sqrtf(horizontal_speed_squared / new_length_squared);

        new_x *= correction;
        new_z *= correction;
    }

    velocity->x = new_x;
    velocity->z = new_z;

}