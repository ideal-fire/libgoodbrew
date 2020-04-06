#include <goodbrew/config.h>
#include <goodbrew/base.h>
#include <goodbrew/graphics.h>
#include <goodbrew/goodbrewInternal.h>
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
#elif GBREND == GBREND_QUICK
	#include <allegro5/allegro.h>
	#include <allegro5/allegro_primitives.h>
	ALLEGRO_DISPLAY* aDisplay;
	static ALLEGRO_COLOR _allegroClearColor;
#endif

// Used to fix touch coords on Android.
// Init with dummy values in case used in division
int _goodbrewRealScreenWidth=1;
int _goodbrewRealScreenHeight=1;

static int _goodbrewDrawOffX=0;
static int _goodbrewDrawOffY=0;

static int _goodbrewClearR=0;
static int _goodbrewClearG=0;
static int _goodbrewClearB=0;

GETTERFUNC(gbGetDrawOffX,int,_goodbrewDrawOffX);
GETTERFUNC(gbGetDrawOffY,int,_goodbrewDrawOffY);
SETTERFUNC(gbSetDrawOffX,int,_goodbrewDrawOffX);
SETTERFUNC(gbSetDrawOffY,int,_goodbrewDrawOffY);

void FixCoords(float* _x, float* _y){
	*_x=*_x+_goodbrewDrawOffX;
	*_y=*_y+_goodbrewDrawOffY;
	#if DOFIXCOORDS == 1
		*_x = fixX(*_x);
		*_y = fixY(*_y);
	#endif
}
int getScreenWidth(){
	return _goodbrewRealScreenWidth;
}
int getScreenHeight(){
	return _goodbrewRealScreenHeight;
}
void setWindowTitle(char* _newTitle){
	#if GBREND == GBREND_SDL
		SDL_SetWindowTitle(mainWindow,_newTitle);
	#elif GBREND == GBREND_QUICK
		al_set_window_title(aDisplay,_newTitle);
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
		#endif
		if (_passedFlags & WINDOWFLAG_RESIZABLE){
			SDL_SetWindowResizable(mainWindow,SDL_TRUE);
		}
		if (mainWindow==NULL){
			printf("Failed to create window %s\n",SDL_GetError());
			return;
		}
		if (mainWindowRenderer==NULL){
			long _sdlWinFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
			if (_passedFlags & WINDOWFLAG_EXTRAFEATURES){
				_sdlWinFlags|=SDL_RENDERER_TARGETTEXTURE;
			}
			mainWindowRenderer = SDL_CreateRenderer( mainWindow, -1, _sdlWinFlags);
			if (mainWindowRenderer==NULL){
				printf("libgoodbrew: falling back to no vsync\n");
				mainWindowRenderer = SDL_CreateRenderer( mainWindow, -1, 0);
				if (mainWindowRenderer==NULL){
					printf("failed to create renderer\n");
					return;
				}
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
	#elif GBREND == GBREND_QUICK
		if (!al_init()) {
			fprintf(stderr,"Could not init Allegro.\n");
			exit(1);
		}
		int f=0;
		if (_passedFlags & WINDOWFLAG_FULLSCREEN){
			f|=ALLEGRO_FULLSCREEN;
		}else{
			f|=ALLEGRO_WINDOWED;
		}
		if (_passedFlags & WINDOWFLAG_RESIZABLE){
			f|=ALLEGRO_RESIZABLE;
		}
		al_set_new_display_flags(f);
		al_install_mouse();
		al_install_keyboard();
		if (!(aDisplay = al_create_display(_windowWidth,_windowHeight))) {
			fprintf(stderr,"Error creating display\n");
			exit(1);
		}
		_allegroClearColor=al_map_rgb(255,255,255);
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
	#elif GBREND == GBREND_QUICK
		al_clear_to_color(_allegroClearColor);
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
	#elif GBREND == GBREND_QUICK
		al_flip_display();
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
	#elif GBREND == GBREND_QUICK
		// TODO - allegro quit
	#endif
}
void setClearColor(int r, int g, int b){
	#if GBREND == GBREND_SDL
		SDL_SetRenderDrawColor( mainWindowRenderer, r, g, b, 255 );
	#elif GBREND == GBREND_VITA2D
		vita2d_set_clear_color(RGBA8(r, g, b, 255));
	#elif GBREND == GBREND_SF2D
		sf2d_set_clear_color(RGBA8(r, g, b, 255));
	#elif GBREND == GBREND_QUICK
		_allegroClearColor=al_map_rgb(r,g,b);
	#endif
	_goodbrewClearR=r;
	_goodbrewClearG=g;
	_goodbrewClearB=b;
}
void getClearColor(unsigned char* r, unsigned char* g, unsigned char* b){
	*r=_goodbrewClearR;
	*g=_goodbrewClearG;
	*b=_goodbrewClearB;
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
	#elif GBREND == GBREND_QUICK
		al_draw_filled_rectangle(x,y,x+w,y+h,al_map_rgba(r,g,b,a));
	#endif
}
void drawRectangle(float x, float y, int w, int h, int r, int g, int b, int a){
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
	#elif GBREND == GBREND_QUICK
		al_set_clipping_rectangle(x,y,w,h);
	#endif
}
void disableClipping(){
	#if GBREND == GBREND_VITA2D
		vita2d_disable_clipping();
	#elif GBREND == GBREND_SDL
		SDL_RenderSetClipRect(mainWindowRenderer,NULL);
	#elif GBREND == GBREND_SF2D
		// Not supported
	#elif GBREND == GBREND_QUICK
		al_set_clipping_rectangle(0,0,getScreenWidth(),getScreenHeight());
	#endif
}
