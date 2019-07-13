#ifndef GOODBREWIMAGESINCLUDED_H
#define GOODBREWIMAGESINCLUDED_H

#if GBREND == GBREND_SDL
	#include <SDL2/SDL.h>
	typedef SDL_Texture* crossTexture;
#endif
#if GBREND == GBREND_VITA2D
	#include <libvita2dplusbloat/vita2d.h>
	typedef vita2d_texture* crossTexture;
#endif
#if GBREND == GBREND_SF2D
	#include <sf2d.h>
	typedef sf2d_texture* crossTexture;
#endif

void drawTexture(const crossTexture passedTexture, int destX, int destY); //
void drawTextureSized(const crossTexture passedTexture, int destX, int destY, int destW, int destH); //
void drawTextureAlpha(const crossTexture passedTexture, int destX, int destY, unsigned char alpha);
void drawTextureScaled(const crossTexture passedTexture, int destX, int destY, double _scaleFactor);
void drawTexturePartSized(const crossTexture passedTexture, int destX, int destY, int destW, int destH, int partX, int partY, int partW, int partH); //
void drawTextureSizedAlpha(const crossTexture passedTexture, int destX, int destY, int partW, int partH, unsigned char alpha);
void drawTextureSizedTint(const crossTexture passedTexture, int destX, int destY, int destW, int destH, unsigned char r, unsigned char g, unsigned char b);
void drawTexturePartSizedAlpha(const crossTexture passedTexture, int destX, int destY, int destW, int destH, int partX, int partY, int partW, int partH, unsigned char alpha); //
void drawTexturePartSizedTintAlpha(const crossTexture passedTexture, int destX, int destY, int destW, int destH, int partX, int partY, int partW, int partH, unsigned char r, unsigned char g, unsigned b, unsigned char a); //

void freeTexture(crossTexture passedTexture);
int getTextureHeight(const crossTexture passedTexture);
int getTextureWidth(const crossTexture passedTexture);
void initImages();
crossTexture loadJPGBuffer(void* _passedBuffer, int _passedBufferSize);
crossTexture loadJPG(const char* path);
crossTexture loadPNGBuffer(void* _passedBuffer, int _passedBufferSize);
crossTexture loadPNG(const char* path);
crossTexture loadImage(const char* path);

#endif /* GOODBREWIMAGESINCLUDED_H */
