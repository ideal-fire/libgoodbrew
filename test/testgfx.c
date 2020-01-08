#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <goodbrew/config.h>
#include <goodbrew/base.h>
#include <goodbrew/graphics.h>
#include <goodbrew/controls.h>

// fixX function may be required depending on how libgoodbrew is compiled. we return the position as is.
float fixX(float a){
	return a;
}
float fixY(float a){
	return a;
}
void XOutFunction(){
}
int main(int argc, char const *argv[]){
	generalGoodInit();
	initGraphics(640,480,WINDOWFLAG_RESIZABLE);
	setWindowTitle("pretty");

	int myr=0;
	int myg=0;
	int myb=0;

	// if the user is using a window manager that resizes the window when it opens, the size may have changed.
	// to notice the size changes, a control loop is needed.
	controlsStart();
	controlsEnd();
	// change color depending on if the window size changed
	if (getScreenWidth()==640 && getScreenHeight()==480){
		myg=255;
	}else{
		myb=255;
	}

	startDrawing();
	drawRectangle(0,0,getScreenWidth(),getScreenHeight(),myr,myg,myb,255);
	endDrawing();

	wait(500);

	generalGoodQuit();
	return 0;
}
