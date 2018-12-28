#ifndef GOODBREWTEXTINCLUDED_H
#define GOODBREWTEXTINCLUDED_H

struct goodbrewfont;
typedef struct goodbrewfont* crossFont;

void freeFont(crossFont _passedFont);
void gbDrawTextAlpha(crossFont _passedFont, int x, int y, const char* text, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void gbDrawTextf(crossFont _passedFont, int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a, const char* _formatString, ...);
void gbDrawText(crossFont _passedFont, int x, int y, const char* text, unsigned char r, unsigned char g, unsigned char b);
crossFont loadFont(const char* filename, double _passedSize);
int textHeight(crossFont _passedFont);
int textWidth(crossFont _passedFont, const char* message);

#endif // GOODBREWTEXTINCLUDED_H