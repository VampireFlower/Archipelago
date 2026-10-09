#include <types.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


typedef struct PADStatus
{
    uint16 button;                // Or-ed PAD_BUTTON_* bits
    uint8  stickX;                // -128 <= stickX       <= 127
    int8   stickY;                // -128 <= stickY       <= 127
    int8   ubstickX;              // -128 <= substickX    <= 127
    int8   substickY;             // -128 <= substickY    <= 127
    uint8  triggerLeft;           //    0 <= triggerLeft  <= 255
    uint8  triggerRight;          //    0 <= triggerRight <= 255
    uint8  analogA;               //    0 <= analogA      <= 255
    uint8  analogB;               //    0 <= analogB      <= 255
    int8   err;                   // one of PAD_ERR_* number
} PADStatus;

typedef void (*PADSamplingCallback)(void);

#define PAD_BUTTON_LEFT         0x0001
#define PAD_BUTTON_RIGHT        0x0002
#define PAD_BUTTON_DOWN         0x0004
#define PAD_BUTTON_UP           0x0008
#define PAD_TRIGGER_Z           0x0010
#define PAD_TRIGGER_R           0x0020
#define PAD_TRIGGER_L           0x0040
#define PAD_BUTTON_A            0x0100
#define PAD_BUTTON_B            0x0200
#define PAD_BUTTON_X            0x0400
#define PAD_BUTTON_Y            0x0800
#define PAD_BUTTON_MENU         0x1000
#define PAD_BUTTON_START        0x1000

#define PAD_ERR_NONE            0
#define PAD_ERR_NO_CONTROLLER   -1
#define PAD_ERR_NOT_READY       -2
#define PAD_ERR_TRANSFER        -3


/*80049524*/ BOOL PADReset            ( uint mask );
/*80049624*/ BOOL PADRecalibrate      ( uint mask );
/*80049728*/ BOOL PADInit             ( );
/*8004893c*/ void PADClamp            ( PADStatus* status );
/*80049940*/ uint PADRead             ( PADStatus* status );
/*80049cec*/ void PADSetSamplingRate  ( uint msec );
/*80049d0c*/ void PADControlAllMotors ( uint* command );
/*80049dc4*/ void PADControlMotor     ( int chan, uint command );
/*8004a694*/ BOOL PADSync             ( );
/*8004a6f8*/ void PADSetAnalogMode    ( uint mode );
/*8004a96c*/ PADSamplingCallback PADSetSamplingCallback( PADSamplingCallback callback );



#ifdef __cplusplus
}
#endif