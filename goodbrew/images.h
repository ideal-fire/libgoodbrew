#ifndef GOODBREWIMAGESINCLUDED_H
#define GOODBREWIMAGESINCLUDED_H
#ifdef __cplusplus
extern "C" {
#endif


#if GBREND == GBREND_SDL
	#include <SDL2/SDL.h>
	typedef SDL_Texture crossTexture;
#elif GBREND == GBREND_VITA2D
	#include <libvita2dplusbloat/vita2d.h>
	typedef vita2d_texture crossTexture;
#elif GBREND == GBREND_SF2D
	#include <sf2d.h>
	typedef sf2d_texture crossTexture;
#elif GBREND==GBREND_QUICK
	#include <allegro5/allegro.h>
	#include <allegro5/allegro_image.h>
	typedef ALLEGRO_BITMAP crossTexture;
#endif

void drawTextureAlpha(crossTexture* passedTexture, float destX, float destY, unsigned char alpha);
void drawTexturePartSizedAlpha(crossTexture* passedTexture, float destX, float destY, int destW, int destH, int partX, int partY, int partW, int partH, unsigned char alpha);
void drawTexturePartSizedTintAlpha(crossTexture* passedTexture, float destX, float destY, int destW, int destH, int partX, int partY, int partW, int partH, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void drawTexturePartSized(crossTexture* passedTexture, float destX, float destY, int destW, int destH, int partX, int partY, int partW, int partH);
void drawTextureScaled(crossTexture* passedTexture, float destX, float destY, double _scaleFactor);
void drawTextureSizedAlpha(crossTexture* passedTexture, float _drawX, float _drawY, int _scaledW, int _scaledH, unsigned char alpha);
void drawTextureSizedTint(crossTexture* passedTexture, float destX, float destY, int destW, int destH, unsigned char r, unsigned char g, unsigned char b);
void drawTextureSized(crossTexture* passedTexture, float destX, float destY, int destW, int destH);
void drawTexture(crossTexture* passedTexture, float destX, float destY);

void freeTexture(crossTexture* passedTexture);
int getTextureHeight(crossTexture* passedTexture);
int getTextureWidth(crossTexture* passedTexture);
void initImages();
crossTexture* loadJPGBuffer(void* _passedBuffer, int _passedBufferSize);
crossTexture* loadJPG(const char* path);
crossTexture* loadPNGBuffer(void* _passedBuffer, int _passedBufferSize);
crossTexture* loadPNG(const char* path);
crossTexture* loadBMPBuffer(void* _passedBuffer, int _passedBufferSize);
crossTexture* loadBMP(const char* path);
crossTexture* loadImageBuffer(void* _passedBuffer, int _passedBufferSize);
crossTexture* loadImage(const char* path);
void enableSmoothScaling(crossTexture* _passedTexture);
#ifdef __cplusplus
}
#endif
#endif /* GOODBREWIMAGESINCLUDED_H */
