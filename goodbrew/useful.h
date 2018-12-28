#ifndef GOODBREWUSEFULSTUFFINCLUDED_H
#define GOODBREWUSEFULSTUFFINCLUDED_H

#include <stdio.h>

char* formatf(va_list _startedList, const char* _stringFormat);
char* easySprintf( const char* _stringFormat, ... );
void seekPast(FILE* fp, unsigned char _target);
void seekNextLine(FILE* fp);
char* fancyReadLine(FILE* fp);
char* swapFilename(const char* _passedFilename, char* _newFilename);

#if !(_POSIX_C_SOURCE >= 200809L)
	ssize_t getdelim(char** lineptr, size_t* n, int delim, FILE* stream);
	ssize_t getline(char** lineptr, size_t* n, FILE* stream);
#endif

#endif