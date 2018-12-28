#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <goodbrew/config.h>
#include <goodbrew/base.h>
#include <goodbrew/graphics.h>
#include <goodbrew/text.h>

int fixX(int a){
	return a;
}
int fixY(int a){
	return a;
}

int main(int argc, char const *argv[]){
	generalGoodInit();

	int _realWidth;
	int _realHeight;
	initGraphics(640,480,&_realWidth,&_realHeight);

	crossFont myfont = loadFont("./liberation-sans-bitmap.sfl",-1);
	
	startDrawing();
	gbDrawTextf(myfont, 0, textHeight(myfont), 0, 255, 0, 255, "%s %d %s", "Based on",5,"seconds, Magical Girl Lyrical Nanoha has a cute dub");
	endDrawing();

	wait(500);

	freeFont(myfont);

	generalGoodQuit();
	return 0;
}