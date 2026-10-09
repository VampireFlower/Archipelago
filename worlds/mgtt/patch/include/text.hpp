#include <types.h>
#include <dolphin/gx.h>
#include <mtx.h>

#ifndef MGTT_TEXT_HPP
#define MGTT_TEXT_HPP


/*         GAME DATATYPES         */

struct TBAnimationState {
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

    static TBAnimationState * Allocate(); // 80022790
};

struct Glyph {
    Glyph* next;
    byte kind;
    bool visible;
    byte field3_0x6;
    byte field4_0x7;
    Vec3f position; // ignore z
    Vec3f scale; // ignore z
    float pitch;
    float yaw;
    float roll;
    void *field8_0x2c; /* points to a datatype 0x34 large (800227dc) */
    GXTexObj field9_0x30;
    void *image_data;
    byte field11_0x54;
    byte a;
    byte r;
    byte g;
    byte b;
    byte field16_0x59;
    byte cheesy_effects; /* copy of the parent textbox effects */
    byte ascii_value;
};

struct TBRenderState {
    Glyph* list; // points to LAST character first
    byte Ox4;
    bool alive;
    byte layer;
    byte bubble_opacity;
    byte text_opacity;
   Vec3f position; // the real position
   Vec3f scale;
   float pitch;
   float yaw;
   float roll;
  TBAnimationState * animState;
   Vec2s bubbleDimensions;
    byte Ox38;
    byte Ox39;
    byte Ox3a;
    byte Ox3b[5];
    byte Ox40;
    byte bitfield;
    bool manual_draw;
    sbyte taunt;
    byte Ox44;

    /* Game methods */
    static TBRenderState* Allocate(); // 80022564
    
    // invokes freeglyphs and zeroes this and tbas
    void Clean(); // 80022694

    // call this before freeglyphs
    void FreeImageData(); // 80024c2c
    void FreeGlyphs(); // 80022708
    
    // GX state is already expected to be set up for 2D when calling this function
    void DrawGlyphs(Mtx, int color, bool setup); // 80027ecc

};

enum TBState : byte {
    TB_FREE = 0,
    TB_SPAWN = 1,
    TB_WRITE = 3,
    TB_IDLE = 4,
    TB_WAIT = 5,
    TB_WAIT_2 = 7,
    TB_DESPAWN = 8,
    TB_WRITE_ALL = 10
};

struct TextBlock {
    char buffer[512];
   char* cursor;
    struct {
       short total_width;
       short total_height;
        byte lines_height[16];
    } layout[4];
   TBRenderState* tbrs;
   float spawn_scale; // when spawning, animate to this scale. scale_frames must be nonzero. i hate this
   Vec2s position; // only used for spawning and despawning? you probably want this to stay in sync with tbrs->position
   short opacity_end;
   short cr_position;
   short top_padding;
   Vec2s pen_pos; // relative to the top left of the block
 TBState state;
    byte delay_timer_reset_to;
    byte delay_timer;
    byte active_layout;
    bool instant_spawn;
    byte unknown[131];
    byte current_line;
    byte lf_count;
    RGB pen_color;
    byte cheesy_effects;
   float pen_scale; // can be set by controlcode \x07


    /* Game methods */
    void set_pen_color(char); // 80024598

    void Configure(int strid, int pen_x, int pen_y, int bitfield); // 80024cfc
    static TextBlock* SpawnMessage(int strid, short x, short y, byte taunt, int layer); // 80024f1c
    static TextBlock* Create(short x, short y, int bubble_width, int bubble_height, int _, int layer); // 80024e04
    
    byte AssessLayout(void* layout); // 80023320



    /* User defined methods */

    static TextBlock* New();

    TextBlock* SetText(const char * fmt, ...);
    TextBlock* Position(float x, float y);
    
    // not retroactive
    TextBlock* Color(RGB color);
    TextBlock* Bold();
    TextBlock* Italic();
    TextBlock* Lineless();
    TextBlock* Ghosted();
    TextBlock* Shake();
    TextBlock* Spin();

    void Render();

};


/* Game data */
extern TBRenderState TBRenderStatePool[50]; // 8027d1e0
extern char DEFAULT_STRING_BUFFER[512];     // 802cbcc0
extern TextBlock TextBlockPool[60];         // 802cc050



/* Game Functions*/
extern "C" {
    void TextBlockDeleteAll(); // 800243c0
    void TextBlockStateMachine(void);

    void TextBlockDelete(TextBlock*);


    
    void FormatRepositoryClear();
    void FormatRepositoryPutInt(int tag, int i); // tag must be 5, control code is \x05
    void FormatRepositoryPutFloat(int tag, float f, int precision); // tag must be 4, control code is \x04
    void FormatRepositoryPutString(int tag, char* s); // tag must be 6, control code is \x03
}

/*         OURS         */
extern "C" void TextManager(void);
void print(char* message, Vec2s position, bool (*predicate)());



#endif