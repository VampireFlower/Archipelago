#include <types.h>
#include <mtx.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif





typedef struct Camera {
    Vec3f eye;
    Vec3f target;
    Vec3f up;
    float width;
    float height;
    float fov;
    float nearClip;
    float farClip;
    float aspect;
    Mtx view;
    Mtx forwardView;
    Mtx field_0x9c;
    Mtx44 projection;
    BOOL ortho;
    float projectionVector[7];
    Vec2f viewportCenter;
    float viewportHalfWidth;
    float viewportNegHalfHeight;
    float depthScale;
    float field_0x140;
    Mtx field_0x144;
    float field_0x174;
    float field_0x178;
} Camera;






typedef struct GameState {
    int x;
} GameState;





void PlaySoundEffect(short id, byte volume); // 80011b88

void ManageCamera(Camera *); // 800179f0


/*80018004*/ [[noreturn]] void SetupMainLoop();
/*80018ed0*/ void RunFrame();

/*80018338*/ void ResetGame();

/*80018b28*/ void ClearFlag(byte flag);
/*80018b50*/ void SetFlag(byte flag);
/*80018b78*/ bool GetFlag(byte flag);


/*8001a628*/ void* malloc_from(int arena, int);
/*8001a670*/ void* malloc(uint);
/*8001a85c*/ void  free(void*);


int   SetCurrentArena(int);
int   GetCurrentArena(int);

int   GetRandomInt(); // 31 bits, guaranteed >= 0
float GetRandomFloat(); // 0 to 1


int sprintf(char* dst, const char* fmt, ...);           // 8001c480
int vsprintf(char* dst, const char* fmt, va_list args); // 8001d784


void add_to_render_queue(void* drawer, int arg, int priority, float depth);

bool ProjectOnScreen(Vec3f* world, Vec3f* Screen);



[[noreturn]] void exit(int); // 8003c14c
[[noreturn]] void PPCHalt(); // 80057dd0


void OverlayEntry0();
void OverlayEntry1();
void OverlayEntry2();




extern GameState GameStates;

extern Camera camera;   // 8026c7c0
extern Camera* pCamera; // 802d7e64

extern volatile uint FrameCounter; // 802d7e00



/* GOLF */

extern uint ShotReplayCount;       // 805052a4

extern Camera SecondaryCamera;     // 80505408

extern int  FlyingCameraAngle;     // 8050f128
extern BOOL TRY_BEHIND_CUP_CAMERA; // 8050f14c
extern BOOL TRY_ACTION_CUP_CAMERA; // 8050f150


#ifdef __cplusplus
}
#endif