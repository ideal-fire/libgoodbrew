#ifndef GENERALGOODIMAGESHEADER
#define GENERALGOODIMAGESHEADER
	#include "config.h"
	#include "graphics.h"
	#include "images.h"
	#include <stdio.h>
	#if GBREND == GBREND_SDL
		extern SDL_Window* mainWindow;
		extern SDL_Renderer* mainWindowRenderer;
		#include <SDL2/SDL_image.h>	
	#endif
	#if GBREND == GBREND_SF2D
		#include <3ds.h>
		#include <sfil.h>
		#include <3ds/svc.h>
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
			return _returnTexture;
		}
	#endif
	crossTexture loadPNGBuffer(const void* _passedBuffer, int _passedBufferSize){
		#if GPREND == GBREND_VITA2D
			return vita2d_load_PNG_buffer(_passedBuffer);
		#elif GPREND == GBREND_SDL
			SDL_RWops* _passedBufferRW = SDL_RWFromMem(_passedBuffer, _passedBufferSize);
			SDL_Surface* _tempSurface = IMG_LoadPNG_RW(_passedBufferRW);
			SDL_RWclose(_passedBufferRW);
			return surfaceToTexture(_tempSurface);
		#elif GPREND == GBREND_SF2D
			return sfil_load_PNG_buffer(_passedBuffer,SF2D_PLACE_RAM);
		#endif
	}
	crossTexture loadJPGBuffer(const void* _passedBuffer, int _passedBufferSize){
		#if GPREND == GBREND_VITA2D
			return vita2d_load_JPEG_buffer(_passedBuffer,_passedBufferSize);
		#elif GPREND == GBREND_SDL
			SDL_RWops* _passedBufferRW = SDL_RWFromMem(_passedBuffer, _passedBufferSize);
			SDL_Surface* _tempSurface = IMG_LoadJPG_RW(_passedBufferRW);
			SDL_RWclose(_passedBufferRW);
			return surfaceToTexture(_tempSurface);
		#elif GPREND == GBREND_SF2D
			return sfil_load_JPEG_buffer(_passedBuffer,_passedBufferSize,SF2D_PLACE_RAM);
		#endif
	}
	crossTexture loadPNG(const char* path){
		#if GPREND == GBREND_VITA2D
			return vita2d_load_PNG_file(path);
		#elif GPREND == GBREND_SDL
			SDL_Surface* _tempSurface = IMG_Load(path);
			return surfaceToTexture(_tempSurface);
		#elif GPREND == GBREND_SF2D
			return sfil_load_PNG_file(path,SF2D_PLACE_RAM);
		#endif
	}
	crossTexture loadJPG(const char* path){
		#if GPREND == GBREND_VITA2D
			return vita2d_load_JPEG_file(path);
		#elif GPREND == GBREND_SDL
			return loadPNG(path);
		#elif GPREND == GBREND_SF2D
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
	
	void drawTexture(const crossTexture passedTexture, int _destX, int _destY){
		EASYFIXCOORDS(&_destX,&_destY);
		#if GBREND == GBREND_VITA2D
			vita2d_draw_texture(passedTexture,_destX,_destY);
		#elif GBREND == GBREND_SDL
			SDL_Rect _srcRect;
			SDL_Rect _destRect;
	
			SDL_QueryTexture(passedTexture, NULL, NULL, &(_srcRect.w), &(_srcRect.h));
	
			_destRect.w=_srcRect.w;
			_destRect.h=_srcRect.h;
	
			_destRect.x=_destX;
			_destRect.y=_destY;
			
			_srcRect.x=0;
			_srcRect.y=0;
		
			SDL_RenderCopy(mainWindowRenderer, passedTexture, &_srcRect, &_destRect );
		#elif GBREND == GBREND_SF2D
			sf2d_draw_texture(passedTexture,_destX,_destY);
		#endif
	}

	void drawTexturePartScale(const crossTexture passedTexture, int destX, int destY, int texX, int texY, int texW, int texH, double texXScale, double texYScale){
		EASYFIXCOORDS(&destX,&destY);
		#if GBREND == GBREND_VITA2D
			vita2d_draw_texture_part_scale(passedTexture,destX,destY,texX,texY,texW, texH, texXScale, texYScale);
		#elif GBREND == GBREND_SDL
			SDL_Rect _srcRect;
			SDL_Rect _destRect;
			_srcRect.w=texW;
			_srcRect.h=texH;
			
			_srcRect.x=texX;
			_srcRect.y=texY;
		
			_destRect.w=_srcRect.w*texXScale;
			_destRect.h=_srcRect.h*texYScale;
			//printf("Dest dimensionds is %dx%d;%.6f;%.6f\n",_destRect.w,_destRect.h,texXScale,texYScale);
			_destRect.x=destX;
			_destRect.y=destY;
	
			SDL_RenderCopy(mainWindowRenderer, passedTexture, &_srcRect, &_destRect );
		#elif GPREND == GBREND_SF2D
			sf2d_draw_texture_part_scale(passedTexture,destX,destY,texX,texY,texW, texH, texXScale, texYScale);
		#endif
	}

	void drawTextureScaleTint(const crossTexture passedTexture, int destX, int destY, double texXScale, double texYScale, unsigned char r, unsigned char g, unsigned char b){
		EASYFIXCOORDS(&destX,&destY);
		#if GBREND == GBREND_VITA2D
			vita2d_draw_texture_tint_scale(passedTexture,destX,destY,texXScale,texYScale,RGBA8(r,g,b,255));
		#elif GBREND == GBREND_SDL
			unsigned char oldr;
			unsigned char oldg;
			unsigned char oldb;
			SDL_GetTextureColorMod(passedTexture,&oldr,&oldg,&oldb);
			SDL_SetTextureColorMod(passedTexture, r,g,b);
			SDL_Rect _srcRect;
			SDL_Rect _destRect;
			SDL_QueryTexture(passedTexture, NULL, NULL, &(_srcRect.w), &(_srcRect.h));
			
			_srcRect.x=0;
			_srcRect.y=0;
		
			_destRect.w=(_srcRect.w*texXScale);
			_destRect.h=(_srcRect.h*texYScale);
	
			_destRect.x=destX;
			_destRect.y=destY;
	
			SDL_RenderCopy(mainWindowRenderer, passedTexture, &_srcRect, &_destRect );
			SDL_SetTextureColorMod(passedTexture, oldr, oldg, oldb);
		#elif GBREND == GBREND_SF2D
			sf2d_draw_texture_scale_blend(passedTexture,destX,destY,texXScale,texYScale,RGBA8(r,g,b,255));
			
		#endif
	}

	void drawTexturePartScaleTintAlpha(const crossTexture passedTexture, int destX, int destY, int texX, int texY, int texW, int texH, double texXScale, double texYScale, unsigned char r, unsigned char g, unsigned b, unsigned char a){
		EASYFIXCOORDS(&destX,&destY);
		#if GBREND == GBREND_VITA2D
			vita2d_draw_texture_tint_part_scale(passedTexture,destX,destY,texX,texY,texW, texH, texXScale, texYScale,RGBA8(r,g,b,a));
		#elif GBREND == GBREND_SDL
			unsigned char oldr;
			unsigned char oldg;
			unsigned char oldb;
			unsigned char olda;
			SDL_GetTextureAlphaMod(passedTexture, &olda);
			SDL_SetTextureAlphaMod(passedTexture, a);
			SDL_GetTextureColorMod(passedTexture,&oldr,&oldg,&oldb);
			SDL_SetTextureColorMod(passedTexture, r,g,b);
			SDL_Rect _srcRect;
			SDL_Rect _destRect;
			_srcRect.w=texW;
			_srcRect.h=texH;
			
			_srcRect.x=texX;
			_srcRect.y=texY;
		
			_destRect.w=_srcRect.w*texXScale;
			_destRect.h=_srcRect.h*texYScale;
	
			_destRect.x=destX;
			_destRect.y=destY;
	
			SDL_RenderCopy(mainWindowRenderer, passedTexture, &_srcRect, &_destRect );
			SDL_SetTextureColorMod(passedTexture, oldr, oldg, oldb);
			SDL_SetTextureAlphaMod(passedTexture, olda);
		#elif GPREND == GBREND_SF2D
			sf2d_draw_texture_part_scale_blend(passedTexture,destX,destY,texX,texY,texW, texH, texXScale, texYScale, RGBA8(r,g,b,a));
		#endif
	}

	void drawTexturePartScaleTint(const crossTexture passedTexture, int destX, int destY, int texX, int texY, int texW, int texH, double texXScale, double texYScale, unsigned char r, unsigned char g, unsigned char b){
		drawTexturePartScaleTintAlpha(passedTexture, destX, destY, texX, texY, texW, texH, texXScale, texYScale, r, g, b,255);
	}
	
	void drawTextureScale(const crossTexture passedTexture, int destX, int destY, double texXScale, double texYScale){
		EASYFIXCOORDS(&destX,&destY);
		#if GBREND == GBREND_VITA2D
			vita2d_draw_texture_scale(passedTexture,destX,destY,texXScale,texYScale);
		#elif GBREND == GBREND_SDL
			SDL_Rect _srcRect;
			SDL_Rect _destRect;
			SDL_QueryTexture(passedTexture, NULL, NULL, &(_srcRect.w), &(_srcRect.h));
			
			_srcRect.x=0;
			_srcRect.y=0;
		
			_destRect.w=(_srcRect.w*texXScale);
			_destRect.h=(_srcRect.h*texYScale);
	
			_destRect.x=destX;
			_destRect.y=destY;
	
			SDL_RenderCopy(mainWindowRenderer, passedTexture, &_srcRect, &_destRect );
		#elif GBREND == GBREND_SF2D
			sf2d_draw_texture_scale(passedTexture,destX,destY,texXScale,texYScale);
		#endif
	}

	void drawTextureScaleSize(const crossTexture passedTexture, int destX, int destY, double texXScale, double texYScale){
		EASYFIXCOORDS(&destX,&destY);
		#if GBREND == GBREND_VITA2D
			vita2d_draw_texture_scale(passedTexture,destX,destY,texXScale/(double)getTextureWidth(passedTexture),texYScale/(double)getTextureHeight(passedTexture));
		#elif GBREND == GBREND_SDL
			SDL_Rect _srcRect;
			SDL_Rect _destRect;
			SDL_QueryTexture(passedTexture, NULL, NULL, &(_srcRect.w), &(_srcRect.h));
			
			_srcRect.x=0;
			_srcRect.y=0;
		
			_destRect.w=(texXScale);
			_destRect.h=(texYScale);
	
			_destRect.x=destX;
			_destRect.y=destY;
	
			SDL_RenderCopy(mainWindowRenderer, passedTexture, &_srcRect, &_destRect );
		#elif GBREND == GBREND_SF2D
			sf2d_draw_texture_scale(passedTexture,destX,destY,texXScale,texYScale);
		#endif
	}
	
	// TODO MAKE ROTATE ON WINDOWS
	void drawTexturePartScaleRotate(const crossTexture texture, int x, int y, int tex_x, int tex_y, int tex_w, int tex_h, double x_scale, double y_scale, double rad){
		EASYFIXCOORDS(&x,&y);
		#if GBREND == GBREND_VITA2D
			vita2d_draw_texture_part_scale_rotate(texture,x,y,tex_x,tex_y,tex_w,tex_h,x_scale,y_scale,rad);
		#elif GBREND == GBREND_SDL
			drawTexturePartScale(texture,x,y,tex_x,tex_y,tex_w,tex_h,x_scale,y_scale);
		#elif GBREND == GBREND_SF2D
			sf2d_draw_texture_part_rotate_scale(texture,x,y,rad,tex_x,tex_y,tex_w,tex_h,x_scale,y_scale);
		#endif
	}
	void drawTextureAlpha(const crossTexture passedTexture, int _destX, int _destY, unsigned char alpha){
		EASYFIXCOORDS(&_destX,&_destY);
		#if GBREND == GBREND_VITA2D
			vita2d_draw_texture_tint(passedTexture,_destX,_destY,RGBA8(255,255,255,alpha));
		#elif GBREND == GBREND_SDL
			unsigned char oldAlpha;
			SDL_GetTextureAlphaMod(passedTexture, &oldAlpha);
			SDL_SetTextureAlphaMod(passedTexture, alpha);
			SDL_Rect _srcRect;
			SDL_Rect _destRect;
	
			SDL_QueryTexture(passedTexture, NULL, NULL, &(_srcRect.w), &(_srcRect.h));
	
			_destRect.w=_srcRect.w;
			_destRect.h=_srcRect.h;
	
			_destRect.x=_destX;
			_destRect.y=_destY;
			
			_srcRect.x=0;
			_srcRect.y=0;
		
			SDL_RenderCopy(mainWindowRenderer, passedTexture, &_srcRect, &_destRect );
			SDL_SetTextureAlphaMod(passedTexture, oldAlpha);
		#elif GBREND == GBREND_SF2D
			sf2d_draw_texture_blend(passedTexture,_destX,_destY,RGBA8(255,255,255,alpha));
		#endif
	}
	void drawTextureScaleAlpha(const crossTexture passedTexture, int destX, int destY, double texXScale, double texYScale, unsigned char alpha){
		EASYFIXCOORDS(&destX,&destY);
		#if GBREND == GBREND_VITA2D
			vita2d_draw_texture_tint_scale(passedTexture,destX,destY,texXScale,texYScale,RGBA8(255,255,255,alpha));
		#elif GBREND == GBREND_SDL
			unsigned char oldAlpha;
			SDL_GetTextureAlphaMod(passedTexture, &oldAlpha);
			SDL_SetTextureAlphaMod(passedTexture, alpha);
			SDL_Rect _srcRect;
			SDL_Rect _destRect;
			SDL_QueryTexture(passedTexture, NULL, NULL, &(_srcRect.w), &(_srcRect.h));
			
			_srcRect.x=0;
			_srcRect.y=0;
		
			_destRect.w=(_srcRect.w*texXScale);
			_destRect.h=(_srcRect.h*texYScale);
	
			_destRect.x=destX;
			_destRect.y=destY;
	
			SDL_RenderCopy(mainWindowRenderer, passedTexture, &_srcRect, &_destRect );
			SDL_SetTextureAlphaMod(passedTexture, oldAlpha);
		#elif GBREND == GBREND_SF2D
			sf2d_draw_texture_scale_blend(passedTexture,destX,destY,texXScale,texYScale,RGBA8(255,255,255,alpha));
		#endif
	}

#endif