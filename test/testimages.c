#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <goodbrew/config.h>
#include <goodbrew/base.h>
#include <goodbrew/graphics.h>
#include <goodbrew/images.h>

float fixX(float a){
	return a;
}
float fixY(float a){
	return a;
}
int main(int argc, char const *argv[]){
	generalGoodInit();
	initGraphics(640,480,0);
	initImages();
	setWindowTitle("Test happy");

	crossTexture* mytex = loadPNG("./test.png");
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
