#include <types.h>
#include <mtx.h>

#ifndef MGTT_TEXT_H
#define MGTT_TEXT_H


/*         GAME         */


void FormatRepositoryClear();
void FormatRepositoryPutInt(int tag, int x);

typedef struct TBAnimationState {
    Vec2s target;
    Vec2f position_velocity;
    short position_frames;
    float target_scale;
    float scale_velocity;
    short scale_frames;
    float target_roll;
    float roll_velocity;
    short roll_frames;
    short padding;
    short opacity_start;
    short opacity_end;
    short opacity_duration;
    short opacity_elapsed;
    bool alive;
} TBAnimationState;

typedef struct TextBlockRenderState {
    struct Glyph* list; // points to LAST character first
    byte Ox5;
    bool alive;
    byte layer;
    byte opacity_start;
   Vec3f position;
   Vec3f Ox18;
   float pitch;
   float yaw;
   float roll;
  TBAnimationState * animState;
   Vec2s bubbleDimensions;
    byte Ox38;
    byte Ox39;
    byte Ox3a;
    byte Ox3b[6];
    byte bitfield;
    bool manual_draw;
    byte taunt;
    byte Ox44;
} TextBlockRenderState;

typedef enum : byte {
    TB_FREE,
    TB_SPAWN,
    TB_unk1,
    TB_WRTIE,
    TB_IDLE,
    TB_WAIT,
    TB_unk3,
    TB_unk4,
    TB_DESPAWN,
    TB_unk5,
    TB_WRITE_ALL
} TBState;

typedef struct TextBlock {
    char buffer[512];
   char* cursor;
    struct {
       short total_width;
       short total_height;
        byte lines_height[16];
    } layout[4];
  struct TextBlockRenderState* tbrs;
   float Ox258;
   Vec2s position;
   short opacity_end;
   short Ox262;
   short Ox264;
   Vec2s pen_pos;
 TBState state;
    byte delay_timer_reset_to;
    byte delay_timer;
    byte active_layout;
    byte Ox26e;
    byte unknown[131];
    byte Ox2f2;
    byte lf_count;
    byte pen_r;
    byte pen_g;
    byte pen_b;
    byte cheesy_effects;
   float scale;
} TextBlock;

extern TextBlock TextBlocks[60]; // 802cc050




TextBlock* TextBlockCreate(int, int, int, int, int, int);
void TextBlockConfigure(TextBlock*, int strid, int x, int y, int);
void TextBlockDelete(TextBlock*);
void TextBlockDrawGlyphs(struct TextBlockRenderState *, Mtx, int color, int);
void TextBlockStateMachine(void);



/*         OURS         */
void TextManager(void);



#endif