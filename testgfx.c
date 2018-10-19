#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <goodbrew/config.h>
#include <goodbrew/base.h>
#include <goodbrew/graphics.h>


int main(int argc, char const *argv[]){
	generalGoodInit();

	int _realWidth;
	int _realHeight;
	initGraphics(640,480,&_realWidth,&_realHeight);
	setWindowTitle("Test happy");

	int myr=0;
	int myg=0;
	int myb=0;

	if (_realWidth==640 && _realHeight==480){
		myg=255;
	}else{
		myb=255;
	}

	startDrawing();
	drawRectangle(0,0,_realWidth,_realHeight,myr,myg,myb,255);
	endDrawing();

	wait(500);

	generalGoodQuit();
	return 0;
}