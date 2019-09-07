#include <goodbrew/config.h>
#include <goodbrew/base.h>
#include <goodbrew/graphics.h>

#if DOFIXCOORDS == 1
	void FixCoords(int* _x, int* _y){
		*_x = fixX(*_x);
		*_y = fixY(*_y);
	}
#else
	void FixCoords(int* _x, int* _y){
	}
#endif
// Renderer stuff
#if GBREND == GBREND_SDL
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	SDL_Window* mainWindow;
	SDL_Renderer* mainWindowRenderer;
#elif GBREND == GBREND_VITA2D
	#include <vita2d.h>
#elif GBREND == GBREND_SF2D
	#include <3ds.h>
	#include <stdio.h>
	#include <sf2d.h>
	#include <sfil.h>
	#include <3ds/svc.h>
#endif

// Used to fix touch coords on Android.
// Init with dummy values in case used in division
int _goodbrewRealScreenWidth=1;
int _goodbrewRealScreenHeight=1;

int getScreenWidth(){
	return _goodbrewRealScreenWidth;
}
int getScreenHeight(){
	return _goodbrewRealScreenHeight;
}
void setWindowTitle(char* _newTitle){
	#if GBREND == GBREND_SDL
		SDL_SetWindowTitle(mainWindow,_newTitle);
	#endif
}
void initGraphics(int _windowWidth, int _windowHeight, long _passedFlags){
	#if GBREND == GBREND_SDL
		SDL_Init(SDL_INIT_VIDEO);
		mainWindowRenderer=NULL;
		//
		if (GBPLAT==GB_ANDROID || _passedFlags & WINDOWFLAG_FULLSCREEN){
			SDL_DisplayMode displayMode;
			if (SDL_GetCurrentDisplayMode(0,&displayMode)==0){
				_windowWidth=displayMode.w;
				_windowHeight=displayMode.h;
			}else{
				SDL_Log("Failed to get display mode\n");
				return;
			}
		}
		#if GBPLAT == GB_SWITCH
			// For some reason, the other code doesn't work. I have to do it this way.
			windowWidth=1280;
			windowHeight=720;
			SDL_CreateWindowAndRenderer(windowWidth, windowHeight, 0, &mainWindow, &mainWindowRenderer);
		#else
			mainWindow = SDL_CreateWindow( "TestWindow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowWidth, _windowHeight, SDL_WINDOW_SHOWN);
			if (_passedFlags & WINDOWFLAG_RESIZABLE){
				SDL_SetWindowResizable(mainWindow,SDL_TRUE);
			}
		#endif
		if (mainWindowRenderer==NULL){
			mainWindowRenderer = SDL_CreateRenderer( mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (mainWindowRenderer==NULL){
				mainWindowRenderer = SDL_CreateRenderer( mainWindow, -1, 0);
			}
		}
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
		SDL_SetRenderDrawBlendMode(mainWindowRenderer,SDL_BLENDMODE_BLEND);
		/*#if GBPLAT == GB_WINDOWS || GBPLAT == GB_LINUX
			// Set a solid white icon.
			SDL_Surface* tempIconSurface;
			Uint16* _surfacePixels = malloc(sizeof(Uint16)*16*16);
			char i, j;
			for (i=0;i<16;++i){
				for (j=0;j<16;++j){
					_surfacePixels[i*16+j]=0xffff;
				}
			}
			tempIconSurface = SDL_CreateRGBSurfaceFrom(_surfacePixels,16,16,16,16*2,0x0f00,0x00f0,0x000f,0xf000);
			SDL_SetWindowIcon(mainWindow, tempIconSurface);
			SDL_FreeSurface(tempIconSurface);
			free(_surfacePixels);
			#endif*/
	#elif GBREND == GBREND_VITA2D
		vita2d_init();
		_windowWidth=960;
		_windowHeight=544;
	#elif GBREND == GBREND_SF2D
		sf2d_init();
		_windowWidth=400;
		_windowHeight=240;
	#else
		#error Forgot to make graphics init function
	#endif
	_goodbrewRealScreenWidth = _windowWidth;
	_goodbrewRealScreenHeight = _windowHeight;
}
#if CAPHUGEFPS == 1
	static u64 _goodbrewFrameStartTime;
#endif
void startDrawing(){
	#if GBREND == GBREND_VITA2D
		vita2d_start_drawing();
		vita2d_clear_screen();
	#elif GBREND == GBREND_SDL
		SDL_RenderClear(mainWindowRenderer);
		#if CAPHUGEFPS == 1
			_goodbrewFrameStartTime = getMilli();
		#endif
	#elif GBREND == GBREND_SF2D
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
	#endif
}

void endDrawing(){
	#if GBREND == GBREND_VITA2D
		vita2d_end_drawing();
		vita2d_swap_buffers();
		vita2d_wait_rendering_done();
	#elif GBREND == GBREND_SDL
		SDL_RenderPresent(mainWindowRenderer);
		// this is the only one that may not limit to 60 fps
		#if CAPHUGEFPS == 1
			if (getMilli()-_goodbrewFrameStartTime==0){
				wait(1);
			}
		#endif	
	#elif GBREND == GBREND_SF2D
		sf2d_end_frame();
		sf2d_swapbuffers();
	#endif
}
#if GBREND == GBREND_SF2D
	void startDrawingBottom(){
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
	}
#endif
void quitGraphics(){
	#if GBREND == GBREND_SF2D
		sf2d_fini();
	#elif GBREND == GBREND_VITA2D
		// TODO - vita2d quit
	#elif GBREND == GBREND_SDL
		// TODO - SDL quit
	#endif
}

void setClearColor(int r, int g, int b){
	#if GBREND == GBREND_SDL
		SDL_SetRenderDrawColor( mainWindowRenderer, r, g, b, 255 );
	#elif GBREND == GBREND_VITA2D
		vita2d_set_clear_color(RGBA8(r, g, b, 255));
	#elif GBREND == GBREND_SF2D
		sf2d_set_clear_color(RGBA8(r, g, b, 255));
	#endif
}

// Evade fix coords
void _drawRectangle(int x, int y, int w, int h, int r, int g, int b, int a){
	#if GBREND == GBREND_VITA2D
		vita2d_draw_rectangle(x,y,w,h,RGBA8(r,g,b,a));
	#elif GBREND == GBREND_SDL
		unsigned char oldr;
		unsigned char oldg;
		unsigned char oldb;
		unsigned char olda;
		SDL_GetRenderDrawColor(mainWindowRenderer,&oldr,&oldg,&oldb,&olda);
		SDL_SetRenderDrawColor(mainWindowRenderer,r,g,b,a);
		SDL_Rect tempRect;
		tempRect.x=x;
		tempRect.y=y;
		tempRect.w=w;
		tempRect.h=h;
		SDL_RenderFillRect(mainWindowRenderer,&tempRect);
		SDL_SetRenderDrawColor(mainWindowRenderer,oldr,oldg,oldb,olda);
	#elif GBREND == GBREND_SF2D
		sf2d_draw_rectangle(x,y,w,h,RGBA8(r,g,b,a));
	#endif
}

void drawRectangle(int x, int y, int w, int h, int r, int g, int b, int a){
	EASYFIXCOORDS(&x,&y);
	_drawRectangle(x,y,w,h,r,g,b,a);
}

void gbCoverUnused(int _usedWidth, int _usedHeight, int r, int g, int b){
	int _halfWastedHeight = (_goodbrewRealScreenHeight-_usedHeight)/2;
	_drawRectangle(0,0,_goodbrewRealScreenWidth,_halfWastedHeight,r,g,b,255);
	_drawRectangle(0,_goodbrewRealScreenHeight-_halfWastedHeight,_goodbrewRealScreenWidth,_halfWastedHeight,r,g,b,255);
	_halfWastedHeight = (_goodbrewRealScreenWidth-_usedWidth)/2;
	_drawRectangle(0,0,_halfWastedHeight,_goodbrewRealScreenHeight,r,g,b,255);
	_drawRectangle(_goodbrewRealScreenWidth-_halfWastedHeight,0,_halfWastedHeight,_goodbrewRealScreenHeight,r,g,b,255);
}

void enableClipping(int x, int y, int w, int h){
	#if GBREND == GBREND_VITA2D
		vita2d_enable_clipping(x,y,x+w,y+h);
	#elif GBREND == GBREND_SDL
		SDL_Rect _passedRegion;
		_passedRegion.x=x;
		_passedRegion.y=y;
		_passedRegion.w=w;
		_passedRegion.h=h;
		SDL_RenderSetClipRect(mainWindowRenderer,&_passedRegion);
	#elif GBREND == GBREND_SF2D
		// Not supported
	#endif
}
void disableClipping(){
	#if GBREND == GBREND_VITA2D
		vita2d_disable_clipping();
	#elif GBREND == GBREND_SDL
		SDL_RenderSetClipRect(mainWindowRenderer,NULL);
	#elif GBREND == GBREND_SF2D
		// Not supported
	#endif
}
