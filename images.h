#ifndef GOODBREWIMAGESINCLUDED_H
#define GOODBREWIMAGESINCLUDED_H

#if GBREND == GBREND_SDL
	#include <SDL2/SDL.h>
	typedef SDL_Texture* crossTexture;
#endif
#if GBREND == GBREND_VITA2D
	#include <vita2d.h>
	typedef vita2d_texture* crossTexture;
#endif
#if GBREND == GBREND_SF2D
	#include <sf2d.h>
	typedef sf2d_texture* crossTexture;
#endif

void drawTextureAlpha(const crossTexture passedTexture, int _destX, int _destY, unsigned char alpha);
void drawTexturePartScaleRotate(const crossTexture texture, int x, int y, int tex_x, int tex_y, int tex_w, int tex_h, double x_scale, double y_scale, double rad);
void drawTexturePartScaleTintAlpha(const crossTexture passedTexture, int destX, int destY, int texX, int texY, int texW, int texH, double texXScale, double texYScale, unsigned char r, unsigned char g, unsigned b, unsigned char a);
void drawTexturePartScaleTint(const crossTexture passedTexture, int destX, int destY, int texX, int texY, int texW, int texH, double texXScale, double texYScale, unsigned char r, unsigned char g, unsigned char b);
void drawTexturePartScale(const crossTexture passedTexture, int destX, int destY, int texX, int texY, int texW, int texH, double texXScale, double texYScale);
void drawTextureScaleAlpha(const crossTexture passedTexture, int destX, int destY, double texXScale, double texYScale, unsigned char alpha);
void drawTextureScaleSize(const crossTexture passedTexture, int destX, int destY, double texXScale, double texYScale);
void drawTextureScaleTint(const crossTexture passedTexture, int destX, int destY, double texXScale, double texYScale, unsigned char r, unsigned char g, unsigned char b);
void drawTextureScale(const crossTexture passedTexture, int destX, int destY, double texXScale, double texYScale);
void drawTexture(const crossTexture passedTexture, int _destX, int _destY);
void freeTexture(crossTexture passedTexture);
int getTextureHeight(const crossTexture passedTexture);
int getTextureWidth(const crossTexture passedTexture);
crossTexture loadJPGBuffer(const void* _passedBuffer, int _passedBufferSize);
crossTexture loadJPG(const char* path);
crossTexture loadPNGBuffer(const void* _passedBuffer, int _passedBufferSize);
crossTexture loadPNG(const char* path);
 
#endif /* GOODBREWIMAGESINCLUDED_H */