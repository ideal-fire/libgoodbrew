#ifndef GOODBREWUSEFULSTUFFINCLUDED_H
#define GOODBREWUSEFULSTUFFINCLUDED_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <goodbrew/base.h>

char readABit(crossFile* fp, char* _destBuffer, long* _numRead, long _maxRead);
const char* findCharBackwards(const char* _startHere, const char* _endHere, int _target);
void lowCopyFile(const char* _srcPath, const char* _destPath, char _canMakeDirs);
void copyFile(const char* _srcPath, const char* _destPath);
char* formatf(va_list _startedList, const char* _stringFormat);
char* easySprintf( const char* _stringFormat, ... );
void seekPast(crossFile* fp, unsigned char _target);
void seekNextLine(crossFile* fp);
char* fancyReadLine(crossFile* fp);
char* swapFilename(const char* _passedFilename, char* _newFilename);
char* easyCombineStrings(int _numStrings, ...);
int	crossReadInt(crossFile* fp);
ssize_t goodbrewGetDelim(char** lineptr, size_t* n, int delim, void* stream, char _iscrossfile);
ssize_t crossgetline(char** lineptr, size_t* n, crossFile* stream);
#if !(_POSIX_C_SOURCE >= 200809L)
	ssize_t getdelim(char** lineptr, size_t* n, int delim, FILE* stream);
	ssize_t getline(char** lineptr, size_t* n, FILE* stream);
#endif
#ifdef __cplusplus
}
#endif
#endif
