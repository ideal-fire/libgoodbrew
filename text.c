#include <stdarg.h>
#include "config.h"
#include "text.h"
#include "useful.h"

CrossFont* fontImage=NULL;

#if GBTXT == GBTXT_BITMAP
	float fontSize = 1;
#endif
#if GBTXT == GBTXT_FONTCACHE
	//int fontSize = 20;
	int fontSize=50;
#endif
#if GBTXT == GBTXT_VITA2D
	int fontSize=32;
#endif
#if GBTXT == TEXT_UNDEFINED
	int fontSize = 32;
#endif
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

	void drawLetter(int letterId, int _x, int _y, float size){
		letterId-=32;
		drawTexturePartScale(fontImage,_x,_y+bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].x,bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].imageWidth,bitmapFontLetterInfo[letterId].imageHeight,size,size);
	}
	void drawLetterColorAlpha(int letterId, int _x, int _y, float size, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
		letterId-=32;
		drawTexturePartScaleTintAlpha(fontImage,_x,_y+bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].x,bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].imageWidth,bitmapFontLetterInfo[letterId].imageHeight,size,size,r,g,b,a);
	}
	void drawLetterColor(int letterId, int _x, int _y, float size, unsigned char r, unsigned char g, unsigned char b){
		drawLetterColorAlpha(letterId,_x,_y,size,r,g,b,255);
	}

#endif

void loadFont(char* filename){
	#if GBTXT == GBTXT_BITMAP
		// Load bitmap font image
		char _specificFontImageBuffer[strlen(filename)+5+1]; // filepath + extention + null
		sprintf(_specificFontImageBuffer, "%s%s", filename, ".png");
		fontImage=loadPNG(_specificFontImageBuffer);
		
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
	#elif GBTXT == GBTXT_FONTCACHE
		//fontSize = (SCREENHEIGHT-TEXTBOXY)/3.5;
		FC_FreeFont(fontImage);
		fontImage = NULL;
		fontImage = FC_CreateFont();
		FC_LoadFont(fontImage, mainWindowRenderer, filename, fontSize, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
	#elif GBTXT == GBTXT_VITA2D
		if (fontImage!=NULL){
			vita2d_free_font(fontImage);
			fontImage=NULL;
		}
		fontImage = vita2d_load_font_file(filename);
	#elif GBTXT == TEXT_UNDEFINED
	#endif
}

int textHeight(float scale){
	#if GBTXT == GBTXT_BITMAP
		return (maxBitmapCharacterHeight*scale);
	#elif GBTXT == GBTXT_VITA2D
		return vita2d_font_text_height(fontImage,scale,"a");
	#elif GBTXT == GBTXT_FONTCACHE
		return floor(FC_GetRealHeight(fontImage));
	#elif GBTXT == TEXT_UNDEFINED
		return fontSize;
	#endif
}

// Please always use the same font size
int textWidth(float scale, const char* message){
	#if GBTXT == GBTXT_BITMAP
		int _currentWidth=0;
		int i;
		for (i=0;i<strlen(message);i++){
			if (message[i]-32<95 && message[i]-32>=0){
				_currentWidth+=(bitmapFontLetterInfo[message[i]-32].imageDisplayWidth)*scale;
			}
		}
		return _currentWidth;
	#elif GBTXT == GBTXT_VITA2D
		return vita2d_font_text_width(fontImage,scale,message);
	#elif GBTXT == GBTXT_FONTCACHE
		return FC_GetWidth(fontImage,"%s",message);
	#elif GBTXT == TEXT_UNDEFINED
		return fontSize*strlen(message);
	#endif
}
void gbDrawTextAlpha(int x, int y, const char* text, float size, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	EASYFIXCOORDS(&x,&y);
	#if GBTXT == GBTXT_VITA2D
		vita2d_font_draw_text(fontImage,x,y+textHeight(size), RGBA8(r,g,b,a),floor(size),text);
	#elif GBTXT == GBTXT_BITMAP
		int i=0;
		int _currentDrawTextX=x;
		for (i = 0; i < strlen(text); i++){
			if (text[i]-32<95){
				drawLetterColorAlpha(text[i],_currentDrawTextX,y,size,r,g,b,a);
				_currentDrawTextX+=bitmapFontLetterInfo[text[i]-32].imageDisplayWidth;
			}
		}
	#elif GBTXT == GBTXT_FONTCACHE
		SDL_Color _tempcolor;
		_tempcolor.r = r;
		_tempcolor.g = g;
		_tempcolor.b = b;
		_tempcolor.a = a;
		FC_DrawColor(fontImage, mainWindowRenderer, x, y, _tempcolor ,"%s", text);
	#endif
}
void gbDrawText(int x, int y, const char* text, float size, unsigned char r, unsigned char g, unsigned char b){
	gbDrawTextAlpha(x,y,text,size,r,g,b,255);
}
void gbDrawTextf(int x, int y, float size, unsigned char r, unsigned char g, unsigned char b, unsigned char a, const char* _formatString, ...){
	va_list _tempArgs;
	va_start(_tempArgs, _stringFormat);
	char* _completeString = formatf(_tempArgs,_formatString);
	gbDrawTextAlpha(x,y,_completeString,size,r,g,b,a);
	free(_completeString);
}
