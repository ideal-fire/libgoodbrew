#ifndef GOODBREWBASEHINCLUDED
#define GOODBREWBASEHINCLUDED

#include <stdint.h>
#include <stdio.h>

#if GBREND == GBREND_SDL
	#include <SDL2/SDL.h> // For file functions
#endif
#if GBPLAT == GB_3DS
	#include <3ds.h>
	#include <3ds/svc.h>
#endif
#if GBPLAT == GB_WINDOWS || GBPLAT == GB_LINUX || GBPLAT == GB_ANDROID
	#include <dirent.h>
#endif

//////////////////////////////

typedef uint8_t 	u8;
typedef uint16_t 	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;
typedef int8_t		s8;
typedef int16_t		s16;
typedef int32_t		s32;
typedef int64_t		s64;

//////////////////////////////

enum corssFileSeekPoint{
	CROSSFILE_START,
	CROSSFILE_CUR,
	CROSSFILE_END
};

#if GBPLAT == GB_VITA
	typedef struct{
		char* filename; // Malloc
		int internalPosition;
		FILE* fp;
	}vitaFile;
	typedef vitaFile* crossFile;

	#include <psp2/types.h>
	#include <dirent.h>
#elif GBREND == GBREND_SDL
	typedef SDL_RWops* crossFile;
#else
	typedef FILE* crossFile;
#endif

//////////////////////////////

// Directory paths should end in a slash
#if GBPLAT == GB_WINDOWS || GBPLAT == GB_LINUX
	typedef DIR* crossDir;
	typedef struct dirent* crossDirStorage;
#elif GBPLAT == GB_VITA
	typedef SceUID crossDir;
	typedef SceIoDirent crossDirStorage;
#elif GBPLAT == GB_3DS
	typedef Handle crossDir;
	typedef FS_DirectoryEntry crossDirStorage;
#else
	#warning No directory listing yet
	typedef int crossDir;
	typedef int crossDirStorage;
#endif

//////////////////////////////

signed char checkFileExist(const char* location);
void createDirectory(const char* path);
int crossfclose(crossFile stream);
char crossfeof(crossFile fp);
crossFile crossfopen(const char* filename, const char* mode);
size_t crossfread(void* buffer, size_t size, size_t count, crossFile stream);
int crossfseek(crossFile stream, long int offset, int origin);
long int crossftell(crossFile fp);
int crossgetc(crossFile fp);
int crossungetc(int c, crossFile stream);
void directoryClose(crossDir passedir);
char directoryExists(const char* filepath);
int directoryRead(crossDir* passedir, crossDirStorage* passedStorage);
char dirOpenWorked(crossDir passedir);
void generalGoodInit();
void generalGoodQuit();
char* getDirectoryResultName(crossDirStorage* passedStorage);
u64 getMilli();
signed char isNewLine(crossFile fp, unsigned char _temp);
crossDir openDirectory(const char* filepath);
void removeNewline(char* _toRemove);
void utf2ascii(char* dst, u16* src);
void wait(int miliseconds);

#endif