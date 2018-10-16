#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "goodbrew/config.h"
#include "base.h"
#include "graphics.h"
#include "images.h"


int main(int argc, char const *argv[]){
	generalGoodInit();

	int _realWidth;
	int _realHeight;
	initGraphics(640,480,&_realWidth,&_realHeight);
	initImages();
	setWindowTitle("Test happy");

	crossTexture mytex = loadPNG("./test.png");
	

	if (mytex==NULL){
		printf("oh\n");
	}

	startDrawing();
	drawTexture(mytex,0,0);
	endDrawing();

	wait(500);

	generalGoodQuit();
	return 0;
}