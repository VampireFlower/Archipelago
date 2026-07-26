#include <ball.h>
#include <math.h>
#include <mgtt.h>


/*
 * Based on an observed launch horizontal speed near 4.0.
 */
#define BALL_STEER_FAST_SPEED 4.0f
#define BALL_STEER_SLOW_SPEED 0.5f

/*
 * Desired sideways velocity change per second.
 *
 * These are initial tuning values, not physically derived constants.
 * At high speed the control is deliberately weak.
 */

#define BALL_STEER_FAST_ACCELERATION 0.04f

#define BALL_STEER_SLOW_ACCELERATION 0.75f

// Prevent the 1/speed conversion from producing an extreme turn.
#define BALL_STEER_MAX_DEGREES_PER_SECOND 120.0f

#define BALL_STEER_MIN_HORIZONTAL_SPEED 0.02f

#define BALL_STEER_STICK_DEADZONE 0.08f

#define BALL_STEER_DIRECTION_SIGN -1.0f

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


float steer_process_stick(float stick)
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

    return stick < 0.0f ? -magnitude : magnitude;
}

// flightcontext is 1
void ball_steer(BallFlyingState* ball, float stick_x)
{
    const float dt = 1.0f / 60.0f;

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


    Vec3f* velocity = &ball->velocity;

    extern float recording[];

    //float* floats = malloc_from(0, 0x500);


    if (ShotReplayCount > 0) { // play recording
        stick_x = recording[ball->shotUpdateCount];

    } else { // steer
        recording[ball->shotUpdateCount] = stick_x = steer_process_stick(stick_x);
    }

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
        stick_x *
        turn_rate *
        dt;

    sine = sinf(angle);
    cosine = cosf(angle);

    new_x =
        velocity->x * cosine -
        velocity->z * sine;

    new_z =
        velocity->z * cosine +
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