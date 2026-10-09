#include <types.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum : short {
    LEFT  = 1,
    RIGHT = 2,
    DOWN  = 4,
    UP    = 8,
    Z     = 16,
    R     = 32,
    L     = 64,
    A     = 256,
    B     = 512,
    X     = 1024,
    Y     = 2048,
    START = 4096
} Input;

// this is shit
typedef struct Controller {
    /*0x00*/ Input input;               // dpad and left stick both contribute
    /*0x02*/ Input input2;              // seems to just be a copy of the first
    /*0x04*/ Input newlyPressedButtons; // does not include stick
    /*0x06*/ Input newlyPressed;        // includes stick
    /*0x08*/ Input __unknown;
    /*0x0a*/ Input buttons;             // input without left stick
    /*0x0c*/ Input repeat;              // repeats once every 5 frames 10 frames after the first press
    /*0x0e*/ short repeatTimer;
    /*0x10*/ Input repeatButtons;       // same as above but without stick
    /*0x12*/ short repeatButtonsTimer;
    /*0x14*/ byte newCStickDir;
    /*0x15*/ byte cStickDir;            // 1 2 4 8 l r d u
    /*0x16*/ byte previousCStickDir;
    /*0x17*/ Vec2b lStickRaw;           // -56 to 56
    /*0x19*/ Vec2b cStickRaw;           // -44 to 44
    /*0x1b*/ byte lShoulderRaw;         //   0 to 150
    /*0x1c*/ byte rShoulderRaw;         //   0 to 150
    /*0x1d*/ sbyte error;
    /*0x20*/ Vec2f lStick;              //  -1 to 1
    /*0x28*/ Vec2f cStick;              //  -1 to 1
    /*0x30*/ float lShoulder;           //   0 to 1
    /*0x34*/ float rShoulder;           //   0 to 1

} Controller;



/*8001aaa4*/ void UpdateControllerFromPad(PADStatus*, Controller*);
/*8001af60*/ void UpdateControllerRepeat(Controller*);

/*8026bb60*/ extern Controller Controllers[4];
/*8026bc40*/ extern Controller all_inputs;


#ifdef __cplusplus
}
#endif