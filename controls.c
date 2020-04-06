#include <goodbrew/config.h>
#include <goodbrew/controls.h>
#include <stdio.h>
#include <string.h>

int touchX=-1;
int touchY=-1;
int mouseScroll=-1;
char lastClickWasRight;

char currentPad[NUMBUTTONS];
char lastPad[NUMBUTTONS];
int buttonAlias[NUMBUTTONS];

extern void XOutFunction();
#if GBPLAT == GB_VITA
	#include <psp2/ctrl.h>
	#include <psp2/touch.h>
#elif GBPLAT == GB_3DS
	#include <3ds/types.h>
#elif GBPLAT == GB_SWITCH
	#include <switch.h>
#elif GBREND == GBREND_SDL
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_keycode.h>
	#include <goodbrew/graphics.h> // For screen width and height
	SDL_Keycode lastSDLPressedKey=SDLK_UNKNOWN;
#elif GBREND == GBREND_QUICK
	#include <allegro5/allegro.h>
	static ALLEGRO_KEYBOARD_STATE curControls;
	static ALLEGRO_KEYBOARD_STATE prevControls;
	static ALLEGRO_MOUSE_STATE curMouseState;
	static int prevMouseBallZ;
#endif

// so we can assign to a char
#define TOBOOL(x) ((x)>0)

//////////////////////////////////////////////////////////
#if GBREND == GBREND_QUICK
	int buttonToQuickMap[NUMBUTTONS]={
		0,
		ALLEGRO_KEY_X+1, //a
		ALLEGRO_KEY_Z+1,
		ALLEGRO_KEY_S+1, //x
		ALLEGRO_KEY_A+1,
		ALLEGRO_KEY_L+1,
		ALLEGRO_KEY_R+1,
		ALLEGRO_KEY_UP+1,
		ALLEGRO_KEY_DOWN+1,
		ALLEGRO_KEY_LEFT+1,
		ALLEGRO_KEY_RIGHT+1,
		ALLEGRO_KEY_ENTER+1, // start
		ALLEGRO_KEY_E+1,
		0, // touch
		0, // scroll
		ALLEGRO_KEY_B+1, // back
		0, //resize		
	};
#endif
int fixButtonAlias(crossButton _passedButton){
	if (buttonAlias[_passedButton]!=0){
		return buttonAlias[_passedButton];
	}
	return _passedButton;
}
void lowSetButtonState(crossButton _passedButton, char _newCurStatus, char _newLastStatus){
	crossButton _realButton = fixButtonAlias(_passedButton);
	currentPad[_realButton]=_newCurStatus;
	lastPad[_realButton]=_newLastStatus;
}
void setJustPressed(crossButton _passedButton){
	lowSetButtonState(_passedButton,1,0);
}
void setDown(crossButton _passedButton){
	lowSetButtonState(_passedButton,1,1);
}
void setJustReleased(crossButton _passedButton){
	lowSetButtonState(_passedButton,0,1);
}
#if GBREND == GBREND_SDL
void _readSDLControls(){
	lastSDLPressedKey=SDLK_UNKNOWN;
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
			SDL_GetMouseState(&touchX,&touchY);
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
	currentPad[BUTTON_RESIZE]=0;
	#if GBPLAT == GB_VITA
		SceTouchData _curTouch;
		sceTouchPeek(SCE_TOUCH_PORT_FRONT,&_curTouch,1);
		if (_curTouch.reportNum>0){
			currentPad[BUTTON_TOUCH]=1;
			touchX=_curTouch.report[_curTouch.reportNum-1].x/2;
			touchY=_curTouch.report[_curTouch.reportNum-1].y/2;
		}else{
			currentPad[BUTTON_TOUCH]=0;
		}
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
		
		currentPad[BUTTON_A] 		= TOBOOL(_pad & KEY_A);
		currentPad[BUTTON_B] 		= TOBOOL(_pad & KEY_B);
		currentPad[BUTTON_X] 		= TOBOOL(_pad & KEY_X);
		currentPad[BUTTON_Y] 		= TOBOOL(_pad & KEY_Y);
		currentPad[BUTTON_L] 		= TOBOOL(_pad & KEY_L);
		currentPad[BUTTON_R] 		= TOBOOL(_pad & KEY_R);
		currentPad[BUTTON_UP] 		= TOBOOL(_pad & KEY_UP);
		currentPad[BUTTON_DOWN] 	= TOBOOL(_pad & KEY_DOWN);
		currentPad[BUTTON_LEFT] 	= TOBOOL(_pad & KEY_LEFT);
		currentPad[BUTTON_RIGHT] 	= TOBOOL(_pad & KEY_RIGHT);
		currentPad[BUTTON_START] 	= TOBOOL(_pad & KEY_START);
		currentPad[BUTTON_SELECT] 	= TOBOOL(_pad & KEY_SELECT);
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
		_readSDLControls();
	#elif GBREND == GBREND_QUICK
		al_get_keyboard_state(&curControls);
		al_get_mouse_state(&curMouseState);
		// mouse
		touchX=curMouseState.x;
		touchY=curMouseState.y;
		if (al_mouse_button_down(&curMouseState,1) || al_mouse_button_down(&curMouseState,2)){
			currentPad[BUTTON_TOUCH]=1;
			lastClickWasRight=al_mouse_button_down(&curMouseState,2);
		}else{
			currentPad[BUTTON_TOUCH]=0;
		}
		mouseScroll=curMouseState.z-prevMouseBallZ;
		prevMouseBallZ=curMouseState.z;
	#else
		#warning no control code for this setup
	#endif
}
void controlsEnd(){
	memcpy(lastPad,currentPad,sizeof(currentPad));
	#if GBREND == GBREND_QUICK
		prevControls=curControls;
	#endif
}
char controlsInit(){
	#if GBPLAT == GB_VITA
		sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT,1);
	#endif
	#if GBREND == GBREND_QUICK
		al_get_keyboard_state(&curControls);
		al_get_mouse_state(&curMouseState);
		prevMouseBallZ=curMouseState.z;
	#endif
	return 0;
}
//
char wasJustReleased(crossButton _passedButton){
	crossButton _aliased = fixButtonAlias(_passedButton);
	#if GBREND == GBREND_QUICK
		if (buttonToQuickMap[_aliased]){
			return !al_key_down(&curControls,buttonToQuickMap[_aliased]-1) && al_key_down(&prevControls,buttonToQuickMap[_aliased]-1);
		}
	#endif
	return lastPad[_aliased] && !currentPad[_aliased];
}
char wasJustPressed(crossButton _passedButton){
	crossButton _aliased = fixButtonAlias(_passedButton);
	#if GBREND == GBREND_QUICK
		if (buttonToQuickMap[_aliased]){
			return al_key_down(&curControls,buttonToQuickMap[_aliased]-1) && !al_key_down(&prevControls,buttonToQuickMap[_aliased]-1);
		}
	#endif
	return !lastPad[_aliased] && currentPad[_aliased];
}
char isDown(crossButton _passedButton){
	crossButton _aliased = fixButtonAlias(_passedButton);
	#if GBREND == GBREND_QUICK
		if (buttonToQuickMap[_aliased]){
			return al_key_down(&curControls,buttonToQuickMap[_aliased]-1);
		}
	#endif
	return currentPad[_aliased];
}
char wasIsDown(crossButton _passedButton){
	crossButton _aliased = fixButtonAlias(_passedButton);
	#if GBREND == GBREND_QUICK
		if (buttonToQuickMap[_aliased]){
			return al_key_down(&curControls,buttonToQuickMap[_aliased]-1) ||(&prevControls,buttonToQuickMap[_aliased]-1);
		}
	#endif
	return currentPad[_aliased] || lastPad[_aliased];
}

#define controlsReset() \
	controlsStart(); \
	controlsEnd();
