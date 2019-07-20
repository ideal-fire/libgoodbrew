#ifndef GOODBREWUSEFULSTUFFINCLUDED_H
#define GOODBREWUSEFULSTUFFINCLUDED_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

char readABit(FILE* fp, char* _destBuffer, long* _numRead, long _maxRead);
const char* findCharBackwards(const char* _startHere, const char* _endHere, int _target);
void lowCopyFile(const char* _srcPath, const char* _destPath, char _canMakeDirs);
void copyFile(const char* _srcPath, const char* _destPath);
char* formatf(va_list _startedList, const char* _stringFormat);
char* easySprintf( const char* _stringFormat, ... );
void seekPast(FILE* fp, unsigned char _target);
void seekNextLine(FILE* fp);
char* fancyReadLine(FILE* fp);
char* swapFilename(const char* _passedFilename, char* _newFilename);
char* easyCombineStrings(int _numStrings, ...);
#if !(_POSIX_C_SOURCE >= 200809L)
	ssize_t getdelim(char** lineptr, size_t* n, int delim, FILE* stream);
	ssize_t getline(char** lineptr, size_t* n, FILE* stream);
#endif
#ifdef __cplusplus
}
#endif
#endif
