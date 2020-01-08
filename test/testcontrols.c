#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <goodbrew/config.h>
#include <goodbrew/base.h>
#include <goodbrew/graphics.h>
#include <goodbrew/controls.h>

void XOutFunction(){
	exit(0);
}
float fixX(float a){
	return a;
}
float fixY(float a){
	return a;
}
int main(int argc, char const *argv[]){
	generalGoodInit();
	initGraphics(640,480,0);

	int offx;
	int offy;
	char isR=0;
	while(1){
		controlsStart();
		if (wasJustPressed(BUTTON_A)){
			isR = !isR;
		}else if (wasJustPressed(BUTTON_B)){
			controlsEnd();
			break;
		}
		if (isDown(BUTTON_LEFT) || isDown(BUTTON_RIGHT)){
			offx+=isDown(BUTTON_LEFT) ? -1 : 1;
		}
		if (isDown(BUTTON_UP) || isDown(BUTTON_DOWN)){
			offy+=isDown(BUTTON_UP) ? -1 : 1;
		}
		controlsEnd();
		startDrawing();
		drawRectangle(offx,offy,32,32,isR ? 255 : 0,0,isR ? 0 : 255,255);
		endDrawing();
	}

	generalGoodQuit();
	return 0;
}
