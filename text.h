#ifndef GOODBREWTEXTINCLUDED_H
#define GOODBREWTEXTINCLUDED_H
 

// Text Stuff
#if GBTXT == GBTXT_FONTCACHE
	#include <SDL_FontCache.h>
	typedef FC_Font* CrossFont;
#elif GBTXT == GBTXT_BITMAP
	#include "graphics.h"
	typedef CrossTexture* CrossFont;
#elif GBTXT == GBTXT_VITA2D
	#include <vita2d.h>
	typedef vita2d_font* CrossFont;
#elif GBTXT == TEXT_UNDEFINED
	typedef int* CrossFont;
#endif

#endif // GOODBREWTEXTINCLUDED_H