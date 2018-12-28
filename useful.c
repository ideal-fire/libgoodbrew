#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <goodbrew/config.h>
#include <goodbrew/base.h>

#if !(_POSIX_C_SOURCE >= 200809L)
	#define __DEFAULTGETLINEMALLOC 50
	#define __INCREMENTGETLINEMALLOC 100
	
	ssize_t getdelim(char** lineptr, size_t* n, int delim, FILE* stream){
		if (*lineptr==NULL){
			*n = __DEFAULTGETLINEMALLOC;
			*lineptr = malloc(*n);
		}
		int _currentPos;
		for(_currentPos=0;;){
			if (_currentPos+1>=*n){
				*n = *n+__INCREMENTGETLINEMALLOC;
				*lineptr = realloc(*lineptr,*n);
			}
			int _lastRead = ((*lineptr)[_currentPos++]=fgetc(stream));
			if (ferror(stream)){
				return -1;
			}else if (_lastRead==EOF){
				--_currentPos; // Don't include EOF in buffer
				break;
			}else if(_lastRead==delim){
				break;
			}
		}
		(*lineptr)[_currentPos]='\0';
		return _currentPos;
	}
	
	ssize_t getline(char** lineptr, size_t* n, FILE* stream){
		return getdelim(lineptr,n,'\n',stream);
	}
#endif

char* formatf(va_list _startedList, const char* _stringFormat){
	va_list _doWriteArgs;
	char* _completeString;
	va_copy(_doWriteArgs,_startedList); // vsnprintf modifies the state of _getLengthArgs so that we can't use it anymore, copy it so we can use it twice in total
	_completeString = malloc(vsnprintf(NULL,0,_stringFormat,_startedList)+1); // Get the size it would've written
	va_end( _startedList );
	vsprintf(_completeString,_stringFormat,_doWriteArgs); // This should not overflow because we already got the exact size we'll need
	va_end( _doWriteArgs ); // Even though it's a copy we still need to va_end it.
	return _completeString;
}

char* easySprintf( const char* _stringFormat, ... ) {
	va_list _tempArgs;
	va_start(_tempArgs, _stringFormat);
	char* _completeString = formatf(_tempArgs,_stringFormat);
	return _completeString;
}

void seekPast(FILE* fp, unsigned char _target){
	while (1){
		int _lastRead = fgetc(fp);
		if (_lastRead==_target || _lastRead==EOF){
			break;
		}
	}
}

void seekNextLine(FILE* fp){
	seekPast(fp,0x0A);
}

char* fancyReadLine(FILE* fp){
	char* _tempReadLine = NULL;
	size_t _tempReadLength = 0;
	if (getline(&_tempReadLine,&_tempReadLength,fp)!=0){
		removeNewline(_tempReadLine);
	}
	return _tempReadLine;
}

// todo - should this be in paths.c ?
char* swapFilename(const char* _passedFilename, char* _newFilename){
	int _cachedStrlenOrig = strlen(_passedFilename);
	int _cachedStrlenNew = strlen(_newFilename);
	char* _ret = malloc(_cachedStrlenOrig+strlen(_newFilename)+1);
	int i;
	for (i=_cachedStrlenOrig-1;i>=0;--i){
		if (_passedFilename[i]=='/' || _passedFilename[i]=='\\'){
			break;
		}
	}
	memcpy(_ret,_passedFilename,i+1); // 
	memcpy(&(_ret[i+1]),_newFilename,_cachedStrlenNew);
	_ret[i+_cachedStrlenNew+1]='\0';
	return _ret;
}

#if GBPLAT == GB_ANDROID
	// itoa replacement for android which only supports base 10
	void itoa(int _num, char* _buffer, int _uselessBase){
		sprintf(_buffer, "%d", _num);
	}
#endif