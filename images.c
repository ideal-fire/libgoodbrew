#include <goodbrew/config.h>
#include <goodbrew/graphics.h>
#include <goodbrew/images.h>
#include <stdio.h>
#if GBREND == GBREND_SDL
	extern SDL_Window* mainWindow;
	extern SDL_Renderer* mainWindowRenderer;
	#include <SDL2/SDL_image.h>	
#elif GBREND == GBREND_SF2D
	#include <3ds.h>
	#include <sfil.h>
	#include <3ds/svc.h>

	#warning no good support for sf2d yet
#elif GBREND == GBREND_VITA2D
	#include <psp2/display.h>
#endif
/*
=================================================
== IMAGES
=================================================
*/
#if GBREND == GBREND_SDL
	crossTexture surfaceToTexture(SDL_Surface* _tempSurface){
		SDL_Texture* _returnTexture = SDL_CreateTextureFromSurface( mainWindowRenderer, _tempSurface );
		SDL_FreeSurface(_tempSurface);
		SDL_SetTextureBlendMode(_returnTexture, SDL_BLENDMODE_BLEND); // Allow the texture to be drawn translucent
		return _returnTexture;
	}
	void SDLDrawShared(const crossTexture passedTexture, int destX, int destY, int destW, int destH, int partX, int partY, int partW, int partH, unsigned char r, unsigned char g, unsigned b, unsigned char a, char _doAlpha, char _doTint){
		unsigned char oldr;
		unsigned char oldg;
		unsigned char oldb;
		unsigned char olda;
		if (_doTint){
			SDL_GetTextureColorMod(passedTexture,&oldr,&oldg,&oldb);
			SDL_SetTextureColorMod(passedTexture, r,g,b);
		}
		if (_doAlpha || _doTint){
			SDL_GetTextureAlphaMod(passedTexture, &olda);
			SDL_SetTextureAlphaMod(passedTexture, a);
		}
		////
		SDL_Rect _srcRect;
		SDL_Rect _destRect;
		if (partX==-1){
			SDL_QueryTexture(passedTexture, NULL, NULL, &(_srcRect.w), &(_srcRect.h));
			_srcRect.x=0;
			_srcRect.y=0;
		}else{
			_srcRect.w=partW;
			_srcRect.h=partH;
		
			_srcRect.x=partX;
			_srcRect.y=partY;
		}
		if (destW==-1){
			_destRect.w = _srcRect.w;
			_destRect.h = _srcRect.h;
		}else{
			_destRect.w=destW;
			_destRect.h=destH;
		}
	
		_destRect.x=destX;
		_destRect.y=destY;
	
		SDL_RenderCopy(mainWindowRenderer, passedTexture, &_srcRect, &_destRect );
		////
		if (_doTint){
			SDL_SetTextureColorMod(passedTexture, oldr, oldg, oldb);
		}
		if (_doAlpha || _doTint){
			SDL_SetTextureAlphaMod(passedTexture, olda);
		}
	}
#endif
void initImages(){
	#if GBREND == GBREND_SDL
		IMG_Init( IMG_INIT_PNG );
		IMG_Init( IMG_INIT_JPG );
	#endif
}
crossTexture loadPNGBuffer(void* _passedBuffer, int _passedBufferSize){
	#if GBREND == GBREND_VITA2D
		return vita2d_load_PNG_buffer(_passedBuffer);
	#elif GBREND == GBREND_SDL
		SDL_RWops* _passedBufferRW = SDL_RWFromMem(_passedBuffer, _passedBufferSize);
		SDL_Surface* _tempSurface = IMG_LoadPNG_RW(_passedBufferRW);
		SDL_RWclose(_passedBufferRW);
		return surfaceToTexture(_tempSurface);
	#elif GBREND == GBREND_SF2D
		return sfil_load_PNG_buffer(_passedBuffer,SF2D_PLACE_RAM);
	#endif
}
crossTexture loadJPGBuffer(void* _passedBuffer, int _passedBufferSize){
	#if GBREND == GBREND_VITA2D
		return vita2d_load_JPEG_buffer(_passedBuffer,_passedBufferSize);
	#elif GBREND == GBREND_SDL
		SDL_RWops* _passedBufferRW = SDL_RWFromMem(_passedBuffer, _passedBufferSize);
		SDL_Surface* _tempSurface = IMG_LoadJPG_RW(_passedBufferRW);
		SDL_RWclose(_passedBufferRW);
		return surfaceToTexture(_tempSurface);
	#elif GBREND == GBREND_SF2D
		return sfil_load_JPEG_buffer(_passedBuffer,_passedBufferSize,SF2D_PLACE_RAM);
	#endif
}
crossTexture loadPNG(const char* path){
	#if GBREND == GBREND_VITA2D
		return vita2d_load_PNG_file(path);
	#elif GBREND == GBREND_SDL
		SDL_Surface* _tempSurface = IMG_Load(path);
		return surfaceToTexture(_tempSurface);
	#elif GBREND == GBREND_SF2D
		return sfil_load_PNG_file(path,SF2D_PLACE_RAM);
	#endif
}
crossTexture loadJPG(const char* path){
	#if GBREND == GBREND_VITA2D
		return vita2d_load_JPEG_file(path);
	#elif GBREND == GBREND_SDL
		return loadPNG(path);
	#elif GBREND == GBREND_SF2D
		return sfil_load_PNG_file(path,SF2D_PLACE_RAM);
	#endif
}
void freeTexture(crossTexture passedTexture){
	if (passedTexture==NULL){
		printf("Don't free NULL textures.");
		return;
	}
	#if GBREND == GBREND_VITA2D
		vita2d_wait_rendering_done();
		sceDisplayWaitVblankStart();
		vita2d_free_texture(passedTexture);
	#elif GBREND == GBREND_SDL
		SDL_DestroyTexture(passedTexture);
	#elif GBREND == GBREND_SF2D
		sf2d_free_texture(passedTexture);
	#endif
}
int getTextureWidth(const crossTexture passedTexture){
	#if GBREND == GBREND_VITA2D
		return vita2d_texture_get_width(passedTexture);
	#elif GBREND == GBREND_SDL
		int w, h;
		SDL_QueryTexture(passedTexture, NULL, NULL, &w, &h);
		return w;
	#elif GBREND == GBREND_SF2D
		return passedTexture->width;
	#endif
}
int getTextureHeight(const crossTexture passedTexture){
	#if GBREND == GBREND_VITA2D
		return vita2d_texture_get_height(passedTexture);
	#elif GBREND == GBREND_SDL
		int w, h;
		SDL_QueryTexture(passedTexture, NULL, NULL, &w, &h);
		return h;
	#elif GBREND == GBREND_SF2D
		return passedTexture->height;
	#endif
}
// Simple
void drawTexture(const crossTexture passedTexture, int destX, int destY){
	EASYFIXCOORDS(&destX,&destY);
	#if GBREND == GBREND_VITA2D
		vita2d_draw_texture(passedTexture,destX,destY);
	#elif GBREND == GBREND_SDL
		SDLDrawShared(passedTexture,destX,destY,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0);
	#elif GBREND == GBREND_SF2D
		//sf2d_draw_texture(passedTexture,_destX,_destY);
	#endif
}
void drawTextureSized(const crossTexture passedTexture, int destX, int destY, int destW, int destH){
	EASYFIXCOORDS(&destX,&destY);
	#if GBREND == GBREND_VITA2D
		vita2d_draw_texture_sized(passedTexture,destX,destY,destW,destH);
	#elif GBREND == GBREND_SDL
		SDLDrawShared(passedTexture,destX,destY,destW,destH,-1,-1,-1,-1,0,0,0,0,0,0);
	#elif GBREND == GBREND_SF2D
		//sf2d_draw_texture_scale(passedTexture,destX,destY,partXScale,partYScale);
	#endif
}
void drawTextureAlpha(const crossTexture passedTexture, int destX, int destY, unsigned char alpha){
	EASYFIXCOORDS(&destX,&destY);
	#if GBREND == GBREND_VITA2D
		vita2d_draw_texture_tint(passedTexture,destX,destY,RGBA8(255,255,255,alpha));
	#elif GBREND == GBREND_SDL
		SDLDrawShared(passedTexture,destX,destY,-1,-1,-1,-1,-1,-1,0,0,0,alpha,1,0);
	#elif GBREND == GBREND_SF2D
		sf2d_draw_texture_blend(passedTexture,destX,destY,RGBA8(255,255,255,alpha));
	#endif
}
void drawTextureScaled(const crossTexture passedTexture, int destX, int destY, double _scaleFactor){
	drawTextureSized(passedTexture,destX,destY,getTextureWidth(passedTexture)*_scaleFactor,getTextureHeight(passedTexture)*_scaleFactor);
}
// Less simple
void drawTexturePartSized(const crossTexture passedTexture, int destX, int destY, int destW, int destH, int partX, int partY, int partW, int partH){
	EASYFIXCOORDS(&destX,&destY);
	#if GBREND == GBREND_VITA2D
		vita2d_draw_texture_part_sized(passedTexture,destX,destY,partX,partY,partW, partH, destW, destH);
	#elif GBREND == GBREND_SDL
		SDLDrawShared(passedTexture,destX,destY,destW,destH,partX,partY,partW,partH,0,0,0,0,0,0);
	#elif GBREND == GBREND_SF2D
		//sf2d_draw_texture_part_scale(passedTexture,destX,destY,partX,partY,partW, partH, partXScale, partYScale);
	#endif
}
void drawTextureSizedAlpha(const crossTexture passedTexture, int destX, int destY, int partW, int partH, unsigned char alpha){
	EASYFIXCOORDS(&destX,&destY);
	#if GBREND == GBREND_VITA2D
		vita2d_draw_texture_tint_sized(passedTexture,destX,destY,partW,partH,RGBA8(255,255,255,alpha));
	#elif GBREND == GBREND_SDL
		SDLDrawShared(passedTexture,destX,destY,partW,partH,-1,-1,-1,-1,0,0,0,alpha,1,0);
	#elif GBREND == GBREND_SF2D
		//sf2d_draw_texture_scale_blend(passedTexture,destX,destY,partXScale,partYScale,RGBA8(255,255,255,alpha));
	#endif
}
void drawTextureSizedTint(const crossTexture passedTexture, int destX, int destY, int destW, int destH, unsigned char r, unsigned char g, unsigned char b){
	EASYFIXCOORDS(&destX,&destY);
	#if GBREND == GBREND_VITA2D
		vita2d_draw_texture_tint_sized(passedTexture,destX,destY,destW,destH,RGBA8(r,g,b,255));
	#elif GBREND == GBREND_SDL
		SDLDrawShared(passedTexture,destX,destY,destW,destH,-1,-1,-1,-1,r,g,b,255,1,1);
	#elif GBREND == GBREND_SF2D
		//sf2d_draw_texture_scale_blend(passedTexture,destX,destY,partXScale,partYScale,RGBA8(r,g,b,255));
	#endif
}
// Even less simple
void drawTexturePartSizedTintAlpha(const crossTexture passedTexture, int destX, int destY, int destW, int destH, int partX, int partY, int partW, int partH, unsigned char r, unsigned char g, unsigned b, unsigned char a){
	EASYFIXCOORDS(&destX,&destY);
	#if GBREND == GBREND_VITA2D
		vita2d_draw_texture_tint_part_sized(passedTexture,destX,destY,partX,partY,partW, partH, destW, destH,RGBA8(r,g,b,a));
	#elif GBREND == GBREND_SDL
		SDLDrawShared(passedTexture,destX,destY,destW,destH,partX,partY,partW,partH,r,g,b,a,1,1);
	#elif GBREND == GBREND_SF2D
		//sf2d_draw_texture_part_scale_blend(passedTexture,destX,destY,partX,partY,partW, partH, partXScale, partYScale, RGBA8(r,g,b,a));
	#endif
}