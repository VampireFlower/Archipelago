#include <types.h>
#include <mtx.h>

#ifndef MGTT_TEXT_H
#define MGTT_TEXT_H



/*         GAME         */
typedef int TextBlock;

extern TextBlock TextBlocks[]; // 802cc050

TextBlock* TextBlockActivate(int, int, int, int, int, int);
void TextBlockConfigure(TextBlock*, int strid, int x, int y, int);
void TextBlockDelete(TextBlock*);
void TextBlockDrawGlyphs(int subsystem, Mtx, int color, int);
void TextBlockStateMachine(void);



/*         OURS         */
void TextManager(void);

#endif