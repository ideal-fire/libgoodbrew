#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <goodbrew/config.h>
#include <goodbrew/text.h>
#include <goodbrew/useful.h>
#include <goodbrew/base.h>

// Text Stuff
#if GBTXT == GBTXT_FONTCACHE
	#include <SDL_FontCache.h>
#elif GBTXT == GBTXT_VITA2D
	#include <vita2d.h>
#endif

#if GBTXT == GBTXT_FONTCACHE
	extern SDL_Window* mainWindow;
	extern SDL_Renderer* mainWindowRenderer;
	#include "SDL_FontCache.h"
#endif
#include <goodbrew/graphics.h>
#include <goodbrew/images.h>

struct goodbrewfont{
	void* data;
	char type;
	double size;
};

struct bitmapFontLetter{
	short x;
	short y;
	short imageWidth;
	short imageHeight;
	short imageDisplayWidth;
};

struct loadedBitmapFont{
	crossTexture internalImage;
	struct bitmapFontLetter* letterInfos;
	short numLetters;
	short firstLetter;
	short exportedSize;
	short height;
};

double _bitmapFontScaleGet(struct loadedBitmapFont* _passed, double _passedSize){
	return (_passedSize/_passed->exportedSize);
}
void drawBitmapLetterColorAlpha(struct loadedBitmapFont* _passedFont, double _passedSize, int letterId, int _x, int _y, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	letterId-=_passedFont->firstLetter;
	drawTexturePartSizedTintAlpha(_passedFont->internalImage,_x,_y+_passedFont->letterInfos[letterId].y*_bitmapFontScaleGet(_passedFont,_passedSize),_passedFont->letterInfos[letterId].imageWidth*_bitmapFontScaleGet(_passedFont,_passedSize),_passedFont->letterInfos[letterId].imageHeight*_bitmapFontScaleGet(_passedFont,_passedSize),_passedFont->letterInfos[letterId].x,_passedFont->letterInfos[letterId].y,_passedFont->letterInfos[letterId].imageWidth,_passedFont->letterInfos[letterId].imageHeight,r,g,b,a);
}

double getResonableFontSize(char _passedType){
	if (_passedType==GBTXT_BITMAP){
		return 28;
	}
	#if GBTXT == GBTXT_FONTCACHE
		return 20;
	#elif GBTXT == GBTXT_VITA2D
		return 32;
	#elif GBTXT == TEXT_UNDEFINED
		return 32;
	#elif GBTXT == TEXT_UNDEFINED
		return 1;
	#endif
}

void freeFont(crossFont _passedFont){
	if (_passedFont->type==GBTXT_BITMAP){
		struct loadedBitmapFont* _castPassed = _passedFont->data;
		freeTexture(_castPassed->internalImage);
		free(_castPassed->letterInfos);
		free(_passedFont->data);
	}else{
		#if GBTXT == GBTXT_FONTCACHE
			FC_FreeFont(_passedFont->data);
		#elif GBTXT == GBTXT_VITA2D
			vita2d_free_font(_passedFont->data);
		#endif
	}
	free(_passedFont);
}

crossFont loadFont(const char* filename, double _passedSize){
	struct goodbrewfont* _retFont = malloc(sizeof(struct goodbrewfont));
	int _cachedstrlen = strlen(filename);
	if (_cachedstrlen>=4 && strcmp(&(filename[_cachedstrlen-4]),".sfl")==0){
		_retFont->type = GBTXT_BITMAP;
		//https://github.com/andryblack/fontbuilder/blob/24459633c531f2316c7c3eed1fee5ef87ac8bda0/src/exporters/simpleexporter.cpp
		FILE* fp = fopen(filename,"r");
		if (fp!=NULL){
			struct loadedBitmapFont* _retBitmapFont = malloc(sizeof(struct loadedBitmapFont));

			seekNextLine(fp); // Seek past font name
			fscanf(fp,"%hd %hd\n",&_retBitmapFont->exportedSize,&_retBitmapFont->height);

			char* _tempReadName = fancyReadLine(fp);
			char* _completeImagePath = swapFilename(filename,_tempReadName);
			_retBitmapFont->internalImage = loadPNG(_completeImagePath);
			if (_retBitmapFont->internalImage==NULL){
				printf("failed to load bitmap font image at %s\n",_completeImagePath);
			}
			free(_tempReadName);
			free(_completeImagePath);

			fscanf(fp,"%hd\n",&_retBitmapFont->numLetters);
			
			_retBitmapFont->letterInfos = malloc(sizeof(struct bitmapFontLetter)*_retBitmapFont->numLetters);

			int _highestReadIndex;
			int i;
			for (i=0;i<_retBitmapFont->numLetters;++i){
				fscanf(fp,"%d %hd %hd %hd %hd %*d %*d %hd\n",&_highestReadIndex,&(_retBitmapFont->letterInfos[i].x),&(_retBitmapFont->letterInfos[i].y),&(_retBitmapFont->letterInfos[i].imageWidth),&(_retBitmapFont->letterInfos[i].imageHeight),&(_retBitmapFont->letterInfos[i].imageDisplayWidth));
			}
			_retBitmapFont->firstLetter = _highestReadIndex-(_retBitmapFont->numLetters-1);
			fclose(fp);

			_retFont->data = _retBitmapFont;
			_retFont->size = (_passedSize==-1) ? _retBitmapFont->exportedSize : _passedSize;
		}
	}else{
		_retFont->type = GBTXT;
		if (_passedSize==-1){
			_passedSize = getResonableFontSize(_retFont->type);
		}
		#if GBTXT == GBTXT_FONTCACHE
			_retFont->data = FC_CreateFont();
			FC_LoadFont(_retFont->data, mainWindowRenderer, filename, _passedSize, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
		#elif GBTXT == GBTXT_VITA2D
			_retFont->data = vita2d_load_font_file(filename);
		#endif
		_retFont->size = _passedSize;
	}
	if (_retFont->data==NULL){
		printf("failed to load font\n");
		free(_retFont);
		return NULL;
	}
	return _retFont;
}

int textHeight(crossFont _passedFont){
	if (_passedFont->type==GBTXT_BITMAP){
		struct loadedBitmapFont* _castPassed = _passedFont->data;
		return (_castPassed->height*_bitmapFontScaleGet(_castPassed,_passedFont->size));
	}	
	#if GBTXT == GBTXT_VITA2D
		return vita2d_font_text_height(_passedFont->data,_passedFont->size,"a");
	#elif GBTXT == GBTXT_FONTCACHE
		return floor(FC_GetRealHeight(_passedFont->data));
	#elif GBTXT == TEXT_UNDEFINED
		return 32;
	#endif
}

// Please always use the same font size
int textWidth(crossFont _passedFont, const char* message){
	if (_passedFont->type==GBTXT_BITMAP){
		struct loadedBitmapFont* _castPassed = _passedFont->data;
		int _currentWidth=0;
		int _cachedstrlen = strlen(message);
		int i;
		for (i=0;i<_cachedstrlen;i++){
			if (message[i]>=_castPassed->firstLetter && message[i]<_castPassed->firstLetter+_castPassed->numLetters){
				_currentWidth+=_castPassed->letterInfos[message[i]-_castPassed->firstLetter].imageDisplayWidth*_bitmapFontScaleGet(_castPassed,_passedFont->size);
			}
		}
		return _currentWidth;
	}
	#if GBTXT == GBTXT_VITA2D
		return vita2d_font_text_width(_passedFont->data,_passedFont->size,message);
	#elif GBTXT == GBTXT_FONTCACHE
		return FC_GetWidth(_passedFont->data,"%s",message);
	#elif GBTXT == TEXT_UNDEFINED
		return _passedFont->size*strlen(message);
	#endif
}
void gbDrawTextAlpha(crossFont _passedFont, int x, int y, const char* text, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	EASYFIXCOORDS(&x,&y);
	if (_passedFont->type==GBTXT_BITMAP){
		struct loadedBitmapFont* _castPassed = _passedFont->data;
		int _currentDrawTextX=x;
		int _cachedstrlen = strlen(text);
		int i;
		for (i = 0; i < _cachedstrlen; i++){
			if (text[i]>=_castPassed->firstLetter && text[i]<_castPassed->firstLetter+_castPassed->numLetters){
				drawBitmapLetterColorAlpha(_castPassed,_passedFont->size,text[i],_currentDrawTextX,y,r,g,b,a);
				_currentDrawTextX+=_castPassed->letterInfos[text[i]-_castPassed->firstLetter].imageDisplayWidth*_bitmapFontScaleGet(_castPassed,_passedFont->size);
			}
		}
	}else{
		#if GBTXT == GBTXT_VITA2D
			vita2d_font_draw_text(_passedFont->data,x,y+textHeight(_passedFont,_passedFont->size), RGBA8(r,g,b,a),_passedFont->size,text);
		#elif GBTXT == GBTXT_FONTCACHE
			SDL_Color _tempcolor;
			_tempcolor.r = r;
			_tempcolor.g = g;
			_tempcolor.b = b;
			_tempcolor.a = a;
			FC_DrawColor(_passedFont->data, mainWindowRenderer, x, y, _tempcolor ,"%s", text);
		#endif
	}
}
void gbDrawText(crossFont _passedFont, int x, int y, const char* text, unsigned char r, unsigned char g, unsigned char b){
	gbDrawTextAlpha(_passedFont,x,y,text,r,g,b,255);
}
void gbDrawTextf(crossFont _passedFont, int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a, const char* _formatString, ...){
	va_list _tempArgs;
	va_start(_tempArgs, _formatString);
	char* _completeString = formatf(_tempArgs,_formatString);
	gbDrawTextAlpha(_passedFont,x,y,_completeString,r,g,b,a);
	free(_completeString);
}
