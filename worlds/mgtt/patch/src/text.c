#include "mgtt.h"

TextBlock* mytext;

void TextDraw(TextBlock* block) {
    
    mtx44 projection;
    mtx identity;

    C_MTXOrtho(
        projection,0,480,0,640,0,-1000
    );

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
    
    TextBlockDraw(*(block + 0x95), identity, 0xffffffff, 1);

    // if it isnt deleted, the game automatically picks up on the tb and draws it?!
    // means we can't use effects like typewriter. investigate later
    // TextBlockDelete(block);
    // mytext = 0;

}


void TextManager(void)
{
    char input = *(char*)0x8026bb60;

    if (input == 1) { 
        if (!mytext){
            mytext = TextBlockCreate(368,192,320,192,1,3);
            TextBlockConfigure(mytext, 0, 8, 28, 0);
        }
        add_to_render_queue(11.0, TextDraw, (int)mytext, 20);
    } else if (mytext) {
        TextBlockDelete(mytext);
        mytext = 0;
    }
}