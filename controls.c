#include <goodbrew/config.h>
#include <goodbrew/controls.h>
#include <string.h>

int touchX=-1;
int touchY=-1;
int mouseScroll=-1;

char currentPad[NUMBUTTONS];
char lastPad[NUMBUTTONS];
char buttonAlias[NUMBUTTONS];

#if GBPLAT == GB_VITA
	#include <psp2/ctrl.h>
#elif GBPLAT == GB_3DS
	#include <3ds/types.h>
#elif GBPLAT == GB_SWITCH
	#include <switch.h>
#elif GBREND == GBREND_SDL
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_keycode.h>
	#include <goodbrew/graphics.h> // For screen width and height
	SDL_Keycode lastSDLPressedKey=SDLK_UNKNOWN;
	extern void XOutFunction();
	char lastClickWasRight;
#endif

// so we can assign to a char
#define TOBOOL(x) ((x)>0)

//////////////////////////////////////////////////////////

int fixButtonAlias(int _passedButton){
	if (buttonAlias[_passedButton]!=0){
		return buttonAlias[_passedButton];
	}
	return _passedButton;
}

#if GBREND == GBREND_SDL
void _readSDLControls(){
	lastSDLPressedKey=SDLK_UNKNOWN;
	currentPad[BUTTON_RESIZE]=0;
	SDL_Event e;
	while(SDL_PollEvent(&e)!=0){
		switch(e.type){
		case SDL_QUIT:
			XOutFunction();
			break;
		case SDL_MOUSEWHEEL:
			mouseScroll=e.wheel.y;
			currentPad[BUTTON_SCROLL]=1;
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			;
			char _isDown;
			if (_isDown=(e.type == SDL_KEYDOWN)){
				lastSDLPressedKey = e.key.keysym.sym;
			}
			if (e.key.keysym.sym==SDLK_x){
				currentPad[BUTTON_A]=_isDown;
			}else if (e.key.keysym.sym==SDLK_z){
				currentPad[BUTTON_B]=_isDown;
			}else if (e.key.keysym.sym==SDLK_LEFT){
				currentPad[BUTTON_LEFT]=_isDown;
			}else if (e.key.keysym.sym==SDLK_RIGHT){
				currentPad[BUTTON_RIGHT]=_isDown;
			}else if (e.key.keysym.sym==SDLK_DOWN){
				currentPad[BUTTON_DOWN]=_isDown;
			}else if (e.key.keysym.sym==SDLK_UP){
				currentPad[BUTTON_UP]=_isDown;
			}else if (e.key.keysym.sym==SDLK_a){
				currentPad[BUTTON_Y]=_isDown;
			}else if (e.key.keysym.sym==SDLK_s){
				currentPad[BUTTON_X]=_isDown;
			}else if (e.key.keysym.sym==SDLK_ESCAPE || e.key.keysym.sym==SDLK_RETURN){
				currentPad[BUTTON_START]=_isDown;
			}else if (e.key.keysym.sym==SDLK_e){
				currentPad[BUTTON_SELECT]=_isDown;
			}else if (e.key.keysym.sym==SDLK_b || e.key.keysym.sym==SDLK_AC_BACK){
				currentPad[BUTTON_BACK]=_isDown;
			}else if (e.key.keysym.sym==SDLK_l){
				currentPad[BUTTON_L]=_isDown;
			}else if (e.key.keysym.sym==SDLK_r){
				currentPad[BUTTON_R]=_isDown;
			}
			break;
		case SDL_FINGERMOTION:
			if (!currentPad[BUTTON_TOUCH]){
				break;
			}
		case SDL_FINGERDOWN:
			touchX = e.tfinger.x * _goodbrewRealScreenWidth;
			touchY = e.tfinger.y * _goodbrewRealScreenHeight;
			currentPad[BUTTON_TOUCH]=1;
			lastClickWasRight=0;
			break;
		case SDL_MOUSEBUTTONDOWN: // Initial click
			SDL_GetMouseState(&touchX,&touchY);
			currentPad[BUTTON_TOUCH] = 1;
			lastClickWasRight = (e.button.button==SDL_BUTTON_RIGHT);
			break;
		case SDL_MOUSEMOTION: // Click and drag
			if (!currentPad[BUTTON_TOUCH]){
				break;
			}
			SDL_GetMouseState(&touchX,&touchY);
			currentPad[BUTTON_TOUCH] = 1;
			break;
		case SDL_FINGERUP:
			currentPad[BUTTON_TOUCH] = 0;
			break;
		case SDL_MOUSEBUTTONUP:
			currentPad[BUTTON_TOUCH] = 0;
			break;
		case SDL_WINDOWEVENT:
			if (e.window.event==SDL_WINDOWEVENT_SIZE_CHANGED){
				_goodbrewRealScreenWidth=e.window.data1;
				_goodbrewRealScreenHeight=e.window.data2;
				currentPad[BUTTON_RESIZE]=1;
			}
			break;
		}
	}
}
#endif

// It's easiest to do controls this way. There are different ways of storing and reading controls and this makes them all compatible easily.
void controlsStart(){
	// TODO - Work on touch for homebrew systems
	#if GBPLAT == GB_VITA
		SceCtrlData _pad;
		sceCtrlPeekBufferPositive(0, &_pad, 1);
		currentPad[BUTTON_A] = 		TOBOOL(_pad.buttons & SCE_CTRL_CROSS);
		currentPad[BUTTON_B] = 		TOBOOL(_pad.buttons & SCE_CTRL_CIRCLE);
		currentPad[BUTTON_X] = 		TOBOOL(_pad.buttons & SCE_CTRL_TRIANGLE);
		currentPad[BUTTON_Y] = 		TOBOOL(_pad.buttons & SCE_CTRL_SQUARE);
		currentPad[BUTTON_L] = 		TOBOOL(_pad.buttons & SCE_CTRL_LTRIGGER);
		currentPad[BUTTON_R] = 		TOBOOL(_pad.buttons & SCE_CTRL_RTRIGGER);
		currentPad[BUTTON_UP] = 	TOBOOL(_pad.buttons & SCE_CTRL_UP);
		currentPad[BUTTON_DOWN] = 	TOBOOL(_pad.buttons & SCE_CTRL_DOWN);
		currentPad[BUTTON_LEFT] = 	TOBOOL(_pad.buttons & SCE_CTRL_LEFT);
		currentPad[BUTTON_RIGHT] = 	TOBOOL(_pad.buttons & SCE_CTRL_RIGHT);
		currentPad[BUTTON_START] = 	TOBOOL(_pad.buttons & SCE_CTRL_START);
		currentPad[BUTTON_SELECT] = TOBOOL(_pad.buttons & SCE_CTRL_SELECT);
	#elif GBPLAT == GB_3DS
		hidScanInput();
		u32 _pad = hidKeysHeld();
		
		currentPad[BUTTON_A] 		= TOBOOL(_pad & KEY_A)
		currentPad[BUTTON_B] 		= TOBOOL(_pad & KEY_B)
		currentPad[BUTTON_X] 		= TOBOOL(_pad & KEY_X)
		currentPad[BUTTON_Y] 		= TOBOOL(_pad & KEY_Y)
		currentPad[BUTTON_L] 		= TOBOOL(_pad & KEY_L)
		currentPad[BUTTON_R] 		= TOBOOL(_pad & KEY_R)
		currentPad[BUTTON_UP] 		= TOBOOL(_pad & KEY_UP)
		currentPad[BUTTON_DOWN] 	= TOBOOL(_pad & KEY_DOWN)
		currentPad[BUTTON_LEFT] 	= TOBOOL(_pad & KEY_LEFT)
		currentPad[BUTTON_RIGHT] 	= TOBOOL(_pad & KEY_RIGHT)
		currentPad[BUTTON_START] 	= TOBOOL(_pad & KEY_START)
		currentPad[BUTTON_SELECT] 	= TOBOOL(_pad & KEY_SELECT)
	#elif GBPLAT == GB_SWITCH
		hidScanInput();
		u64 _pad = hidKeysHeld(CONTROLLER_P1_AUTO);

		currentPad[BUTTON_A] 		= TOBOOL(_pad & KEY_A);
		currentPad[BUTTON_B] 		= TOBOOL(_pad & KEY_B);
		currentPad[BUTTON_X] 		= TOBOOL(_pad & KEY_X);
		currentPad[BUTTON_Y] 		= TOBOOL(_pad & KEY_Y);
		currentPad[BUTTON_L] 		= TOBOOL(_pad & KEY_ZL);
		currentPad[BUTTON_R] 		= TOBOOL(_pad & KEY_ZR);
		currentPad[BUTTON_UP] 		= TOBOOL(_pad & KEY_UP);
		currentPad[BUTTON_DOWN] 	= TOBOOL(_pad & KEY_DOWN);
		currentPad[BUTTON_LEFT] 	= TOBOOL(_pad & KEY_LEFT);
		currentPad[BUTTON_RIGHT] 	= TOBOOL(_pad & KEY_RIGHT);
		currentPad[BUTTON_START] 	= TOBOOL(_pad & KEY_PLUS);
		currentPad[BUTTON_SELECT] 	= TOBOOL(_pad & KEY_MINUS);
	#elif GBREND == GBREND_SDL
		// This is only a function because I don't want all this messy code here
		_readSDLControls();
	#else
		#warning no control code for this setup
	#endif
}

void controlsEnd(){
	memcpy(lastPad,currentPad,sizeof(currentPad));
}

char wasJustReleased(crossButton _passedButton){
	return lastPad[fixButtonAlias(_passedButton)] && !currentPad[fixButtonAlias(_passedButton)];
}
char wasJustPressed(crossButton _passedButton){
	return !lastPad[fixButtonAlias(_passedButton)] && currentPad[fixButtonAlias(_passedButton)];
}
char isDown(crossButton _passedButton){
	return currentPad[fixButtonAlias(_passedButton)];
}

#define controlsReset() \
	controlsStart(); \
	controlsEnd();
