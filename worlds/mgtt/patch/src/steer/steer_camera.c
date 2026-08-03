#include <mgtt.h>
#include <mtx.h>
#include <types.h>

bool active;

bool SeekingCamera() {

    active = false; // debug

    // run vanilla code for replays
    if (ShotReplayCount) return false;

    active = true;

    // if you start within 32 units and end inside 0.96 units of the
    // cup, the game will use a camera focusing on the cup 1/4 times
    TRY_BEHIND_CUP_CAMERA = false;
    
    // selectballflightcamera returns before this value is set to false
    // on first shot, so it could be true from a previous replay
    TRY_ACTION_CUP_CAMERA = false;

    // Disable special hazard cameras
    *(int*)0x80502810 = 0;

    return true;


}
