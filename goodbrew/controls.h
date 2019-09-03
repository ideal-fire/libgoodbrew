#ifndef GOODBREWCONTROLSINCLUDED_H
#define GOODBREWCONTROLSINCLUDED_H
#ifdef __cplusplus
extern "C" {
#endif


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
	BUTTON_RESIZE, // On window resize. Use getWindowWidth and getWindowHeight


	NUMBUTTONS
}crossButton;

void controlsEnd();
void controlsStart();
#define controlsReset() controlsStart(); controlsEnd();
int fixButtonAlias(crossButton _passedButton);
char isDown(crossButton _passedButton);
char wasJustPressed(crossButton _passedButton);
char wasJustReleased(crossButton _passedButton);
char wasIsDown(crossButton _passedButton); // if button was down last frame or this frame
// These four are risky. When using SDL, the button won't go up by itself.
void lowSetButtonState(crossButton _passedButton, char _newCurStatus, char _newLastStatus);
void setJustPressed(crossButton _passedButton);
void setDown(crossButton _passedButton);
void setJustReleased(crossButton _passedButton);
	
extern int touchX;
extern int touchY;

#if GBREND == GBREND_SDL
	#include <SDL2/SDL_keycode.h>
	extern SDL_Keycode lastSDLPressedKey;
	extern void XOutFunction();
	extern int mouseScroll;
	extern char lastClickWasRight;
#endif

#ifdef __cplusplus
}
#endif
	
#endif
