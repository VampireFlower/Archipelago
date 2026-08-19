extern "C" {
    #include <gx.h>
    #include <mgtt.h>
    #include <mtx.h>
}

#include <text.hpp>

int bar = 8;

TextBlock* TextBlock::Create(int x, int y) {
    return TextBlockCreate(x,y,x+10,y+10,1,20);
}

TextBlock* mytext;

// based on DrawShotEndText
void TextDraw(TextBlock* block) {

    if (block->state == TB_FREE)
        return;

    Mtx44 projection;
    Mtx identity;

    C_MTXOrtho(projection, 0, 480, 0, 640, 0, -1000);

    GXSetProjection(projection, 1);

    PSMTXIdentity(identity);
    GXSetBlendMode(1,4,5,15);

    // All vertex positions will now be multiplied by this matrix before projection.
    GXLoadPosMtxImm(identity, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0,0,0,4);
    GXSetChanCtrl(4,0,0,1,0,2,2);
    GXSetCullMode(0);
    GXSetZMode(0,7,0);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetTexCoordGen2(0,1,4,60);
    
    TextBlockDrawGlyphs(block->tbrs, identity, 0xffffffff, 1);

}

void TextManager(void)
{
    
    char input = *(char*)0x8026bb60;

    if (input) { 
        if (!mytext){
            mytext = TextBlock::Create(368,192);
            TextBlockConfigure(mytext, 0, 8, 28, 1);
        }
        if (mytext->state != TB_FREE) // deleted when exiting gameplay
            add_to_render_queue(11.0, (void*)TextDraw, (int)mytext, 20);
        else
            mytext = nullptr;
    } else if (mytext) {
        TextBlockDelete(mytext);
        mytext = nullptr;
    }
}