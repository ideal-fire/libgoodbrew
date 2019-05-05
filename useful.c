#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <goodbrew/config.h>
#include <goodbrew/base.h>

#define COPYBUFF 16000

#define SEPARATOR '/'
#if GBPLAT == GB_WINDOWS
	#undef SEPARATOR
	#define SEPARATOR '\\'
#endif


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

const char* findCharBackwards(const char* _startHere, const char* _endHere, int _target){
	do{
		if (_startHere[0]==_target){
			return _startHere;
		}
		--_startHere;
	}while(_startHere>_endHere);
	return NULL;
}

char readABit(crossFile fp, char* _destBuffer, long* _numRead, long _maxRead){
	if (crossfeof(fp)){
		return 1;
	}
	*_numRead = crossfread(_destBuffer,1,_maxRead,fp);
	return 0;
}

void lowCopyFile(const char* _srcPath, const char* _destPath, char _canMakeDirs){
	crossFile _destfp = crossfopen(_destPath,"wb");
	if (_destfp!=NULL){
		crossFile _sourcefp = crossfopen(_srcPath,"rb");
		if (_sourcefp!=NULL){
			char* _currentBit = malloc(COPYBUFF);
			long _lastRead;
			while (!readABit(_sourcefp,_currentBit,&_lastRead,COPYBUFF)){
				if (crossfwrite(_currentBit,1,_lastRead,_destfp)!=_lastRead){
					printf("wrote wrong number of bytes.\n");
				}
			}
			free(_currentBit);
			crossfclose(_sourcefp);
		}else{
			printf("Failed to open for reading %s\n",_srcPath);
		}
		crossfclose(_destfp);
	}else{
		// Make all directories that need to be made for the destination to work
		char _shouldRetry=0;
		if (_canMakeDirs){
			char* _tempPath = strdup(_destPath);
			int _numMakeDirs=0;
			while(1){
				char* _possibleSeparator=(char*)findCharBackwards(&(_tempPath[strlen(_tempPath)-1]),_tempPath,SEPARATOR);
				if (_possibleSeparator!=NULL && _possibleSeparator!=_tempPath){
					_possibleSeparator[0]='\0';
					if (directoryExists(_tempPath)){ // When the directory that does exist is found break to create the missing ones in order.
						break;
					}else{
						++_numMakeDirs;
					}
				}else{
					break;
				}
			}
			if (_numMakeDirs>0){
				_shouldRetry=1;
				int i;
				for (i=0;i<_numMakeDirs;++i){
					_tempPath[strlen(_tempPath)]=SEPARATOR;
					createDirectory(_tempPath);
				}
			}
			free(_tempPath);
		}

		if (_shouldRetry){
			lowCopyFile(_srcPath,_destPath,0);
		}else{
			printf("Failed to open for writing %s\n",_destPath);
		}
	}
}

void copyFile(const char* _srcPath, const char* _destPath){
	lowCopyFile(_srcPath,_destPath,1);
}


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

void seekPast(crossFile fp, unsigned char _target){
	while (1){
		int _lastRead = crossgetc(fp);
		if (_lastRead==_target || _lastRead==EOF){
			break;
		}
	}
}

void seekNextLine(crossFile fp){
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