#include <stdarg.h>
#include "config.h"
#include "text.h"
#include "useful.h"

// TODO - Should multiple fonts at once be supported? If so, I need to store the bitmap font info in a struct that's in the crossFont variable

#if GBTXT == GBTXT_FONTCACHE
	extern SDL_Window* mainWindow;
	extern SDL_Renderer* mainWindowRenderer;
	#include "SDL_FontCache.h"
#endif
#include "graphics.h"

#if GBTXT == GBTXT_BITMAP
	typedef struct{
		int x;
		int y;
		int imageWidth;
		int imageHeight;
		int imageDisplayWidth;
	}BitmapFontLetter;
	
	BitmapFontLetter bitmapFontLetterInfo[95];
	unsigned short maxBitmapCharacterHeight = 12;

	void drawLetter(crossFont _passedFont, int letterId, int _x, int _y, double size){
		letterId-=32;
		drawTexturePartScale(_passedFont,_x,_y+bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].x,bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].imageWidth,bitmapFontLetterInfo[letterId].imageHeight,size,size);
	}
	void drawLetterColorAlpha(int letterId, int _x, int _y, double size, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
		letterId-=32;
		drawTexturePartScaleTintAlpha(_passedFont,_x,_y+bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].x,bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].imageWidth,bitmapFontLetterInfo[letterId].imageHeight,size,size,r,g,b,a);
	}
	void drawLetterColor(int letterId, int _x, int _y, double size, unsigned char r, unsigned char g, unsigned char b){
		drawLetterColorAlpha(letterId,_x,_y,size,r,g,b,255);
	}

#endif

double getDefaultFontSize(){
	#if GBTXT == GBTXT_BITMAP
		return 1;
	#endif
	#if GBTXT == GBTXT_FONTCACHE
		return 20;
	#endif
	#if GBTXT == GBTXT_VITA2D
		return 32;
	#endif
	#if GBTXT == TEXT_UNDEFINED
		return 32;
	#endif
}

void freeFont(crossFont _passedFont){
	#if GBTXT == GBTXT_BITMAP
		freeTexture(_passedFont);
	#elif GBTXT == GBTXT_FONTCACHE
		FC_FreeFont(_passedFont);
	#elif GBTXT == GBTXT_VITA2D
		vita2d_free_font(_passedFont);
	#elif GBTXT == TEXT_UNDEFINED
	#endif
}

crossFont loadFont(char* filename, double _passedSize){
	#if GBTXT == GBTXT_BITMAP
		// Load bitmap font image
		char _specificFontImageBuffer[strlen(filename)+5+1]; // filepath + extention + null
		sprintf(_specificFontImageBuffer, "%s%s", filename, ".png");
		crossFont _retFont = loadPNG(_specificFontImageBuffer);
		
		// Load font info
		sprintf(_specificFontImageBuffer, "%s%s", filename, ".info");
		FILE* fp = fopen(_specificFontImageBuffer,"r");
		int i;
		for (i=0;i<95;i++){
			fscanf(fp,"%d %d %d %d %d\n",&(bitmapFontLetterInfo[i].x),&(bitmapFontLetterInfo[i].y),&(bitmapFontLetterInfo[i].imageWidth),&(bitmapFontLetterInfo[i].imageHeight),&(bitmapFontLetterInfo[i].imageDisplayWidth));
			if (bitmapFontLetterInfo[i].imageHeight>maxBitmapCharacterHeight){
				maxBitmapCharacterHeight=bitmapFontLetterInfo[i].imageHeight;
			}
		}
		fclose(fp);

		return _retFont;
	#elif GBTXT == GBTXT_FONTCACHE
		crossFont _retFont = FC_CreateFont();
		FC_LoadFont(_retFont, mainWindowRenderer, filename, _passedSize, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
		return _retFont;
	#elif GBTXT == GBTXT_VITA2D
		return vita2d_load_font_file(filename);
	#elif GBTXT == TEXT_UNDEFINED
		return NULL;
	#endif
}

int textHeight(crossFont _passedFont, double _passedSize){
	#if GBTXT == GBTXT_BITMAP
		return (maxBitmapCharacterHeight*_passedSize);
	#elif GBTXT == GBTXT_VITA2D
		return vita2d_font_text_height(_passedFont,_passedSize,"a");
	#elif GBTXT == GBTXT_FONTCACHE
		return floor(FC_GetRealHeight(_passedFont));
	#elif GBTXT == TEXT_UNDEFINED
		return _passedSize;
	#endif
}

// Please always use the same font size
int textWidth(crossFont _passedFont, const char* message, double _passedSize){
	#if GBTXT == GBTXT_BITMAP
		int _currentWidth=0;
		int i;
		for (i=0;i<strlen(message);i++){
			if (message[i]-32<95 && message[i]-32>=0){
				_currentWidth+=(bitmapFontLetterInfo[message[i]-32].imageDisplayWidth)*_passedSize;
			}
		}
		return _currentWidth;
	#elif GBTXT == GBTXT_VITA2D
		return vita2d_font_text_width(_passedFont,_passedSize,message);
	#elif GBTXT == GBTXT_FONTCACHE
		return FC_GetWidth(_passedFont,"%s",message);
	#elif GBTXT == TEXT_UNDEFINED
		return _passedSize*strlen(message);
	#endif
}
void gbDrawTextAlpha(crossFont _passedFont, int x, int y, const char* text, double _passedSize, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	EASYFIXCOORDS(&x,&y);
	#if GBTXT == GBTXT_VITA2D
		vita2d_font_draw_text(_passedFont,x,y+textHeight(_passedSize), RGBA8(r,g,b,a),floor(_passedSize),text);
	#elif GBTXT == GBTXT_BITMAP
		int i=0;
		int _currentDrawTextX=x;
		for (i = 0; i < strlen(text); i++){
			if (text[i]-32<95){
				drawLetterColorAlpha(_passedFont,text[i],_currentDrawTextX,y,_passedSize,r,g,b,a);
				_currentDrawTextX+=bitmapFontLetterInfo[text[i]-32].imageDisplayWidth*_passedSize;
			}
		}
	#elif GBTXT == GBTXT_FONTCACHE
		SDL_Color _tempcolor;
		_tempcolor.r = r;
		_tempcolor.g = g;
		_tempcolor.b = b;
		_tempcolor.a = a;
		FC_DrawColor(_passedFont, mainWindowRenderer, x, y, _tempcolor ,"%s", text);
	#endif
}
void gbDrawText(crossFont _passedFont, int x, int y, const char* text, float size, unsigned char r, unsigned char g, unsigned char b){
	gbDrawTextAlpha(_passedFont,x,y,text,size,r,g,b,255);
}
void gbDrawTextf(crossFont _passedFont, int x, int y, float size, unsigned char r, unsigned char g, unsigned char b, unsigned char a, const char* _formatString, ...){
	va_list _tempArgs;
	va_start(_tempArgs, _formatString);
	char* _completeString = formatf(_tempArgs,_formatString);
	gbDrawTextAlpha(_passedFont,x,y,_completeString,size,r,g,b,a);
	free(_completeString);
}
