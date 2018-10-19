#ifndef GOODBREWTEXTINCLUDED_H
#define GOODBREWTEXTINCLUDED_H
 

// Text Stuff
#if GBTXT == GBTXT_FONTCACHE
	#include <SDL_FontCache.h>
	typedef FC_Font* crossFont;
#elif GBTXT == GBTXT_BITMAP
	#include "graphics.h"
	typedef CrossTexture* crossFont;
#elif GBTXT == GBTXT_VITA2D
	#include <vita2d.h>
	typedef vita2d_font* crossFont;
#elif GBTXT == TEXT_UNDEFINED
	typedef int* crossFont;
#endif

void freeFont(crossFont _passedFont);
void gbDrawTextAlpha(crossFont _passedFont, int x, int y, const char* text, double _passedSize, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void gbDrawTextf(crossFont _passedFont, int x, int y, float size, unsigned char r, unsigned char g, unsigned char b, unsigned char a, const char* _formatString, ...);
void gbDrawText(crossFont _passedFont, int x, int y, const char* text, float size, unsigned char r, unsigned char g, unsigned char b);
double getDefaultFontSize();
crossFont loadFont(char* filename, double _passedSize);
int textHeight(crossFont _passedFont, double _passedSize);
int textWidth(crossFont _passedFont, const char* message, double _passedSize);

#endif // GOODBREWTEXTINCLUDED_H