#ifndef GOODBREWGRAPHICSINCLUDED_H
#define GOODBREWGRAPHICSINCLUDED_H
 
#if DOFIXCOORDS == 1
	int fixX(int x);
	int fixY(int y);
	#define EASYFIXCOORDS(x, y) FixCoords(x,y)
#else
	#define EASYFIXCOORDS(x,y)
#endif

extern int _goodbrewRealScreenWidth;
extern int _goodbrewRealScreenHeight;

void gbCoverUnused(int _usedWidth, int _usedHeight, int r, int g, int b); // Assuming that _usedWidth and _usedHeight are the dimensions of the space you're using, and you've centered everything
void drawRectangle(int x, int y, int w, int h, int r, int g, int b, int a);
void endDrawing();
void FixCoords(int* _x, int* _y);
int getScreenHeight();
int getScreenWidth();
// _windowWidth and _windowHeight are recommendations for the Window size. Will be ignored on many platforms
void initGraphics(int _windowWidth, int _windowHeight, int* _storeWindowWidth, int* _storeWindowHeight);
void quitGraphics();
void setClearColor(int r, int g, int b);
void setWindowTitle(char* _newTitle);
void startDrawingBottom();
void startDrawing();
void FixCoords(int* _x, int* _y);
void initImages();
#endif // GOODBREWGRAPHICSINCLUDED_H
