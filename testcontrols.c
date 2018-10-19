#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "goodbrew/config.h"
#include "base.h"
#include "graphics.h"
#include "controls.h"

void XOutFunction(){
	exit(0);
}

int main(int argc, char const *argv[]){
	generalGoodInit();
	int _realWidth;
	initGraphics(640,480,&_realWidth,&_realWidth);

	int isR=0;

	while(1){
		controlsStart();
		if (wasJustPressed(BUTTON_A)){
			isR = !isR;
		}
		if (wasJustPressed(BUTTON_B)){
			controlsEnd();
			break;
		}
		controlsEnd();
		startDrawing();
		if (isR){
			drawRectangle(0,0,32,32,255,0,0,255);
		}else{
			drawRectangle(0,0,32,32,0,0,255,255);
		}
		endDrawing();
	}

	generalGoodQuit();
	return 0;
}