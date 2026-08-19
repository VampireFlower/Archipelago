#include <types.h>
#include <mtx.h>

#ifndef MGTT_TEXT_H
#define MGTT_TEXT_H


/*         GAME         */



void FormatRepositoryClear();
void FormatRepositoryPutInt(int tag, int x);


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

struct TextBlock {
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
   short Ox260;
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

    static TextBlock* Create(int x, int y);
};

extern TextBlock TextBlocks[60]; // 802cc050

extern "C"{
TextBlock* TextBlockCreate(int x, int y, int bubble_width, int bubble_height, int unk, int layer);
void TextBlockConfigure(TextBlock*, int strid, int x, int y, int);
void TextBlockDelete(TextBlock*);
void TextBlockDrawGlyphs(struct TextBlockRenderState *, Mtx, int color, int);
void TextBlockStateMachine(void);
void set_pen_color(TextBlock*, int idx);
}



/*         OURS         */
extern "C" void TextManager(void);



#endif