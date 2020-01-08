#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <goodbrew/config.h>
#include <goodbrew/base.h>
#include <goodbrew/graphics.h>
#include <goodbrew/text.h>

float fixX(float a){
	return a;
}
float fixY(float a){
	return a;
}
int main(int argc, char const *argv[]){
	generalGoodInit();
	initGraphics(640,480,0);

	crossFont* loadedSimpleBitmap = loadFont("./vincent-font.sfl",getResonableFontSize(GBTXT_BITMAP));
	crossFont* loadedFancyBitmap = loadFont("./liberation-sans-bitmap.sfl",-1);
	crossFont* loadedFancyFont = loadFont("./LiberationSans.ttf",getResonableFontSize(GBTXT));
	
	startDrawing();
	gbDrawText(loadedSimpleBitmap, 0, 0, "Simple bitmap font", 255,255,255);
	gbDrawText(loadedFancyBitmap, 0, textHeight(loadedSimpleBitmap), "Fancy bitmap font", 255,255,255);
	gbDrawTextf(loadedFancyFont, 0, textHeight(loadedSimpleBitmap)+textHeight(loadedFancyBitmap), 0, 255, 0, 255, "%s %d %s", "Based on",5,"seconds, Magical Girl Lyrical Nanoha has a cute dub");
	endDrawing();

	wait(500);

	freeFont(loadedSimpleBitmap);
	freeFont(loadedFancyBitmap);
	freeFont(loadedFancyFont);

	generalGoodQuit();
	return 0;
}
