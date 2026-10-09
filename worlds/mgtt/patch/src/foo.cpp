#include <types.h>
#include <mgtt.h>
#include <hook.hpp>


#include <os.h>
#include <pad.h>

#include <controller.h>



static OSThreadQueue sample_ready;

void WakePreshot() {
    OSWakeupThread(&sample_ready);
}

static Input Merge(Input a, Input b)
{
    return static_cast<Input>(
        static_cast<unsigned short>(a) |
        static_cast<unsigned short>(b));
}

static Input NewlyPressed(Input now, Input before)
{
    return static_cast<Input>(
        static_cast<unsigned short>(now) &
        ~static_cast<unsigned short>(before));
}

static void RefreshController(Controller* controller)
{
    // Assumes controller points into Controllers[4].
    const unsigned channel = controller - Controllers;

    PADStatus pads[4];
    PADRead(pads);
    PADClamp(pads);

    if (pads[channel].err != PAD_ERR_NONE)
        return;

    const Input previousHeld = controller->input;
    const Input savedPressed = controller->newlyPressed;
    const Input savedButtonsPressed = controller->newlyPressedButtons;
    const byte savedCStickPressed = controller->newCStickDir;
    const Input savedButtonsRepeat = controller->repeatButtons;

    UpdateControllerFromPad(&pads[channel], controller);

    controller->newlyPressed = Merge(savedPressed, NewlyPressed(controller->input, previousHeld));
    controller->newlyPressedButtons = Merge(savedButtonsPressed, controller->newlyPressedButtons);
    controller->newCStickDir |= savedCStickPressed;
    controller->repeatButtons = savedButtonsRepeat;
}

typedef struct {
    byte _[0x4f13];
    Controller* controller;
} Player;

typedef struct {
    byte _[0x25b];
    Player* player;
} Preshot;

extern "C" int PreshotController(Preshot*);

extern "C" int WaitForSample(Preshot* p)
{

    PADSetSamplingRate(1); // 1ms
    PADSetSamplingCallback(WakePreshot);


    const BOOL enabled = OSDisableInterrupts();

    OSSleepThread(&sample_ready);

    RefreshController(p->player->controller);

    OSRestoreInterrupts(enabled);



    return PreshotController(p);
}