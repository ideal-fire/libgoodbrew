#ifndef GOODBREWGRAPHICSINCLUDED_H
#define GOODBREWGRAPHICSINCLUDED_H
#ifdef __cplusplus
extern "C" {
#endif


#if DOFIXCOORDS == 1
	int fixX(int x);
	int fixY(int y);
	#define EASYFIXCOORDS(x, y) FixCoords(x,y)
#else
	#define EASYFIXCOORDS(x,y)
#endif

#define WINDOWFLAG_FULLSCREEN 1
#define WINDOWFLAG_RESIZABLE 2

extern int _goodbrewRealScreenWidth;
extern int _goodbrewRealScreenHeight;

void gbCoverUnused(int _usedWidth, int _usedHeight, int r, int g, int b); // Assuming that _usedWidth and _usedHeight are the dimensions of the space you're using, and you've centered everything
void drawRectangle(int x, int y, int w, int h, int r, int g, int b, int a);
void endDrawing();
void FixCoords(int* _x, int* _y);
int getScreenHeight();
int getScreenWidth();
// _windowWidth and _windowHeight are recommendations for the Window size. Will be ignored on many platforms. Need to use getScreenWidth and getScreenHeight after calling.
void initGraphics(int _windowWidth, int _windowHeight, long _passedFlags);
void quitGraphics();
void setClearColor(int r, int g, int b);
void setWindowTitle(char* _newTitle);
void startDrawingBottom();
void startDrawing();
void FixCoords(int* _x, int* _y);
void initImages();
void enableClipping(int x, int y, int w, int h);
void disableClipping();
	#ifdef __cplusplus
}
#endif
#endif // GOODBREWGRAPHICSINCLUDED_H
