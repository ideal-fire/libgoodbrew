#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "goodbrew/config.h"
#include "base.h"
#include "graphics.h"
#include "text.h"


int main(int argc, char const *argv[]){
	generalGoodInit();

	int _realWidth;
	int _realHeight;
	initGraphics(640,480,&_realWidth,&_realHeight);
	
	double whateversize = getDefaultFontSize();
	crossFont myfont = loadFont("./LiberationSans.ttf",whateversize);
	
	startDrawing();
	gbDrawTextf(myfont, 0, 0, whateversize, 255, 0, 0, 255, "%s: %f", "size",whateversize);
	gbDrawTextf(myfont, 0, textHeight(myfont,whateversize), whateversize, 0, 255, 0, 255, "%s %d %s", "Based on",5,"seconds, magical girl lyrical nanoha has a cute dub");
	endDrawing();

	wait(500);

	generalGoodQuit();
	return 0;
}