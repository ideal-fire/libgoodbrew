#ifndef GOODBREWCONTROLSINCLUDED_H
#define GOODBREWCONTROLSINCLUDED_H

typedef enum{
	BUTTON_A = 1,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_L,
	BUTTON_R,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_START,
	BUTTON_SELECT,
	BUTTON_TOUCH, // Use touchX, touchY variables
	BUTTON_SCROLL, // PC, use mouseScroll variable
	BUTTON_BACK, // Android


	NUMBUTTONS
}crossButton;

void controlsEnd();
void controlsStart();
#define controlsReset() controlsStart(); controlsEnd();
int fixButtonAlias(int _passedButton);
char isDown(crossButton _passedButton);
char wasJustPressed(crossButton _passedButton);
char wasJustReleased(crossButton _passedButton);

extern int touchX;
extern int touchY;

#if GBREND == GBREND_SDL
	#include <SDL2/SDL_keycode.h>
	extern SDL_Keycode lastSDLPressedKey;
	extern void XOutFunction();
	extern int mouseScroll;
	extern char lastClickWasRight;
#endif

#endif