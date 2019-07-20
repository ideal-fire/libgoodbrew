#ifndef GOODBREWTEXTINCLUDED_H
#define GOODBREWTEXTINCLUDED_H
#ifdef __cplusplus
extern "C" {
#endif


#define BITMAP_FONT_FAKE_SIZE 28

struct goodbrewfont{
	void* data;
	char type;
	double size;
};
typedef struct goodbrewfont* crossFont;

double getResonableFontSize(char _passedType);
void freeFont(crossFont _passedFont);
void gbDrawTextAlpha(crossFont _passedFont, int x, int y, const char* text, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void gbDrawTextf(crossFont _passedFont, int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a, const char* _formatString, ...);
void gbDrawText(crossFont _passedFont, int x, int y, const char* text, unsigned char r, unsigned char g, unsigned char b);
crossFont loadFont(const char* filename, double _passedSize);
int textHeight(crossFont _passedFont);
int textWidth(crossFont _passedFont, const char* message);

#ifdef __cplusplus
}
#endif
#endif // GOODBREWTEXTINCLUDED_H
