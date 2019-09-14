#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#include <goodbrew/config.h>
#include <goodbrew/base.h>

#if GBPLAT == GB_WINDOWS
	#include <windows.h>
#endif
#if GBPLAT == GB_LINUX
	#include <sys/types.h>
	#include <sys/stat.h>
#endif
#if GBPLAT == GB_ANDROID
	// For mkdir
	#include <sys/stat.h>
#endif
#if GBPLAT == GB_VITA
	#include <psp2/kernel/threadmgr.h>
	#include <psp2/kernel/processmgr.h>
	#include <psp2/io/fcntl.h>
#endif
#if GBPLAT == GB_3DS
	FS_Archive _sdArchive=0;
#endif

void generalGoodInit(){
	#if GBPLAT == GB_3DS
		FSUSER_OpenArchive(&_sdArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
		romfsInit();
	#elif GBPLAT == GB_SWITCH
		romfsInit();
	#endif
}
void generalGoodQuit(){
	#if GBPLAT == GB_3DS
		romfsExit();
	#endif
}
// Waits for a number of miliseconds.
void wait(int miliseconds){
	#if GBPLAT == GB_VITA
		sceKernelDelayThread(miliseconds*1000);
	#elif GBREND == GBREND_SDL
		SDL_Delay(miliseconds);
	#elif GBPLAT == GB_3DS
		svcSleepThread(miliseconds*1000000);
	#elif GBPLAT == GB_WINDOWS
		Sleep(miliseconds);
	#elif GBPLAT == GB_SWITCH
		//https://github.com/switchbrew/libnx/blob/a12eb11eab9742afc8c90c0805b02fd1341a7bbc/nx/include/switch/kernel/svc.h
		svcSleepThread(miliseconds*1000000);
	#elif GBPLAT == GB_LINUX
		usleep(miliseconds*1000);
	#endif
}

u64 getMilli(){
	#if GBPLAT == GB_VITA
		return  (sceKernelGetProcessTimeWide() / 1000);
	#elif GBREND == GBREND_SDL
		return SDL_GetTicks();
	#elif GBPLAT == GB_3DS
		return osGetTime();
	#elif GBPLAT == GB_WINDOWS
		LARGE_INTEGER s_frequency;
		char s_use_qpc = QueryPerformanceFrequency(&s_frequency);
		if (s_use_qpc) {
		    LARGE_INTEGER now;
		    QueryPerformanceCounter(&now);
		    return (1000LL * now.QuadPart) / s_frequency.QuadPart;
		} else {
		    return GetTickCount();
		}
	#else
		struct timespec _myTime;
		clock_gettime(CLOCK_MONOTONIC, &_myTime);
		return _myTime.tv_nsec/1000000;
	#endif
}
signed char checkFileExist(const char* location){
	#if GBPLAT == GB_VITA
		SceUID fileHandle = sceIoOpen(location, SCE_O_RDONLY, 0777);
		if (fileHandle < 0){
			return 0;
		}else{
			sceIoClose(fileHandle);
			return 1;
		}
	#elif GBPLAT == GB_WINDOWS || GBPLAT == GB_LINUX || GBPLAT == GB_ANDROID
		return (access( location, F_OK ) != -1);
	#else
		FILE* fp = fopen(location,"r");
		if (fp==NULL){
			return 0;
		}
		fclose(fp);
		return 1;
	#endif
}
void createDirectory(const char* path){
	#if GBPLAT == GB_VITA
		sceIoMkdir(path,0777);
	#elif GBPLAT == GB_WINDOWS
		mkdir(path);
	#elif GBPLAT == GB_LINUX || GBPLAT == GB_ANDROID
		mkdir(path,0777);
	#elif GBPLAT == GB_3DS
		FSUSER_CreateDirectory(_sdArchive, fsMakePath(PATH_ASCII, path), 0);
	#endif
}
/*
================================================
== ETC
=================================================
*/
#if GBPLAT == GB_3DS
	void utf2ascii(char* dst, u16* src){
		if(!src || !dst)return;
		while(*src)*(dst++)=(*(src++))&0xFF;
		*dst=0x00;
	}
#endif
char dirOpenWorked(crossDir passedir){
	#if GBPLAT == GB_WINDOWS || GBPLAT == GB_LINUX || GBPLAT == GB_ANDROID
		return !(passedir==NULL);
	#elif GBPLAT == GB_VITA
		return (passedir>=0);
	#elif GBPLAT == GB_3DS
		return (passedir!=0);
	#else
		return 0;
	#endif
}
crossDir openDirectory(const char* filepath){
	#if GBPLAT == GB_WINDOWS || GBPLAT == GB_LINUX || GBPLAT == GB_ANDROID
		return opendir(filepath);
	#elif GBPLAT == GB_VITA
		return (sceIoDopen(filepath));
	#elif GBPLAT == GB_3DS
		// Should not end in slash
		char _tempFixedFilepath[strlen(filepath)+1];
		strcpy(_tempFixedFilepath,filepath);
		if (_tempFixedFilepath[strlen(filepath)-1]==47){
			_tempFixedFilepath[strlen(filepath)-1]=0;
		}
		FS_Path _stupidPath=fsMakePath(PATH_ASCII, _tempFixedFilepath);
		crossDir _openedDirectory;
		if (FSUSER_OpenDirectory(&_openedDirectory, _sdArchive, _stupidPath)!=0){
			return 0;
		}
		return _openedDirectory;
	#else
		return 0;
	#endif
}
char* getDirectoryResultName(crossDirStorage* passedStorage){
	#if GBPLAT == GB_WINDOWS || GBPLAT == GB_LINUX || GBPLAT == GB_ANDROID
		return ((*passedStorage)->d_name);
	#elif GBPLAT == GB_VITA
		//WriteToDebugFile
		return ((passedStorage)->d_name);
	#elif GBPLAT == GB_3DS
		return (char*)(passedStorage->name);
	#else
		return NULL;
	#endif
}
// Return 0 if not work
int directoryRead(crossDir* passedir, crossDirStorage* passedStorage){
	#if GBPLAT == GB_WINDOWS || GBPLAT == GB_LINUX || GBPLAT == GB_ANDROID
		*passedStorage = readdir (*passedir);
		if (*passedStorage != NULL){
			if (strcmp((*passedStorage)->d_name,".")==0 || strcmp((*passedStorage)->d_name,"..")==0){
				return directoryRead(passedir,passedStorage);
			}
		}
		if (*passedStorage == NULL){
			return 0;
		}else{
			return 1;
		}
	#elif GBPLAT == GB_VITA
		return sceIoDread(*passedir,passedStorage);
	#elif GBPLAT == GB_3DS
		u32 _didReadSomething=0;
		FSDIR_Read(*passedir, &_didReadSomething, 1, passedStorage);
		if (_didReadSomething!=0){
			u16* _tempName = passedStorage->name;
			char _tempBuffer[256]; // plz no bigger
			utf2ascii(_tempBuffer,_tempName);
			int _foundStrlen = strlen(_tempBuffer);
			memcpy(_tempName,_tempBuffer,_foundStrlen+1);
		}
		return _didReadSomething;
	#else
		return 0;
	#endif
}
void directoryClose(crossDir passedir){
	#if GBPLAT == GB_WINDOWS || GBPLAT == GB_LINUX || GBPLAT == GB_ANDROID
		closedir(passedir);
	#elif GBPLAT == GB_VITA
		sceIoDclose(passedir);
	#elif GBPLAT == GB_3DS
		FSDIR_Close(passedir);
	#endif
}
char directoryExists(const char* filepath){
	crossDir _tempdir = openDirectory(filepath);
	if (dirOpenWorked(_tempdir)==1){
		directoryClose(_tempdir);
		return 1;
	}else{
		return 0;
	}
}
/*
==========================================================
== CROSS PLATFORM FILE WRITING AND READING
==========================================================
*/
#if GBPLAT == GB_VITA
	void _fixVitaFile(vitaFile* _passedFile){
		fclose(_passedFile->fp);
		_passedFile->fp = fopen(_passedFile->filename,"rb");
		fseek(_passedFile->fp,_passedFile->internalPosition,SEEK_SET);
	}
#endif
// Removes all 0x0D and 0x0A from last two characters of string by moving null character.
void removeNewline(char* _toRemove){
	int _cachedStrlen = strlen(_toRemove);
	if (_cachedStrlen==0){
		return;
	}
	if (_toRemove[_cachedStrlen-1]==0x0A){ // Last char is UNIX newline
		if (_cachedStrlen>=2 && _toRemove[_cachedStrlen-2]==0x0D){ // If it's a Windows newline
			_toRemove[_cachedStrlen-2]='\0';
		}else{ // Well, it's at very least a UNIX newline
			_toRemove[_cachedStrlen-1]='\0';
		}
	}
}
size_t crossfwrite(void* buffer, size_t size, size_t count, crossFile stream){
	#if GBPLAT == GB_VITA
		size_t _writtenElements = fwrite(buffer,size,count,stream->fp);
		if (_writtenElements==0 && count!=0 && feof(stream->fp)==0){
			_fixVitaFile(stream);
			return crossfwrite(buffer,size,count,stream);
		}
		stream->internalPosition += size*_writtenElements;
		return _writtenElements;
	#elif GBREND == GBREND_SDL
		return SDL_RWwrite(stream,buffer,size,count);
	#else
		return fwrite(buffer,size,count,stream);
	#endif
}
// Returns number of elements read
size_t crossfread(void* buffer, size_t size, size_t count, crossFile stream){
	#if GBPLAT == GB_VITA
		size_t _readElements = fread(buffer,size,count,stream->fp);
		if (_readElements==0 && count!=0 && feof(stream->fp)==0){
			_fixVitaFile(stream);
			return crossfread(buffer,size,count,stream);
		}
		stream->internalPosition += size*_readElements;
		return _readElements;
	#elif GBREND == GBREND_SDL
		return SDL_RWread(stream,buffer,size,count);
	#else
		return fread(buffer,size,count,stream);
	#endif
}
crossFile crossfopen(const char* filename, const char* mode){
	#if GBPLAT == GB_VITA
		vitaFile* _returnFile = malloc(sizeof(vitaFile));
		_returnFile->fp=fopen(filename,mode);
		_returnFile->filename = malloc(strlen(filename)+1);
			strcpy(_returnFile->filename,filename);
		_returnFile->internalPosition=0;
		return _returnFile;
	#elif GBREND == GBREND_SDL
		return SDL_RWFromFile(filename,mode);
	#else
		return fopen(filename,mode);
	#endif
}
// Returns 0 on success.
// Returns negative number of failure
int crossfclose(crossFile stream){
	#if GBPLAT == GB_VITA
		fclose(stream->fp);
		free(stream->filename);
		return 0;
	#elif GBREND == GBREND_SDL
		return SDL_RWclose(stream);
	#else
		return fclose(stream);
	#endif
}
// stream, offset, CROSSFILE_START, CROSSFILE_END, CROSSFILE_CUR
// Returns 0 if worked
int crossfseek(crossFile stream, long int offset, int origin){
	int _trueOrigin;
	if (origin==CROSSFILE_START){
		#if GBREND == GBREND_SDL
			_trueOrigin = RW_SEEK_SET;
		#else
			_trueOrigin = SEEK_SET;
		#endif
	}else if (origin==CROSSFILE_CUR){
		#if GBREND == GBREND_SDL
			_trueOrigin = RW_SEEK_CUR;
		#else
			_trueOrigin = SEEK_CUR;
		#endif
	}else if (origin==CROSSFILE_END){
		#if GBREND == GBREND_SDL
			_trueOrigin = RW_SEEK_END;
		#else
			_trueOrigin = SEEK_END;
		#endif
	}

	#if GBPLAT == GB_VITA
		int _seekReturnValue;
		_seekReturnValue = fseek(stream->fp,offset,_trueOrigin);
		// If seek failed but it shouldn't have because we're not at the end of the file yet.
		if (_seekReturnValue!=0 && feof(stream->fp)==0){
			_fixVitaFile(stream);
			return crossfseek(stream,offset,_trueOrigin);
		}else{
			stream->internalPosition = ftell(stream->fp);
		}
		return 0;
	#elif GBREND == GBREND_SDL
		return (SDL_RWseek(stream,offset,_trueOrigin)==-1);
	#else
		return fseek(stream,offset,_trueOrigin);
	#endif
}
// THIS DOES NOT DO EXACTLY WHAT IT SHOULD
int crossungetc(int c, crossFile stream){
	if (crossfseek(stream,-1,CROSSFILE_CUR)==0){
		return c;
	}
	return EOF;
}
long int crossftell(crossFile fp){
	#if GBPLAT == GB_VITA
		return fp->internalPosition;
	#elif GBREND == GBREND_SDL
		return SDL_RWseek(fp,0,RW_SEEK_CUR);
	#else
		return ftell(fp);
	#endif
}
// No platform specific code here
int crossgetc(crossFile fp){
	unsigned char _readChar;
	if (crossfread(&_readChar,1,1,fp)==0){
		return EOF;
	}
	return _readChar;
}
char crossfeof(crossFile fp){
	#if GBPLAT == GB_VITA
		return feof(fp->fp);
	#elif GBREND == GBREND_SDL
		if (crossgetc(fp)==EOF){
			return 1;
		}
		crossfseek(fp,-1,CROSSFILE_CUR);
		return 0;
	#else
		return feof(fp);
	#endif
}
// Checks if the byte is the one for a newline
// If it's 0D, it seeks past the 0A that it assumes is next and returns 1
signed char isNewLine(crossFile fp, unsigned char _temp){
	if (_temp==0x0D){
		crossfseek(fp,1,CROSSFILE_CUR);
		return 1;
	}
	if (_temp=='\n' || _temp==0x0A){
		// It seems like the other newline char is skipped for me?
		return 1;
	}
	return 0;
}
