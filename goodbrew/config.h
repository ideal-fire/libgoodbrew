#define GB_LINUX 1
#define GB_WINDOWS 2
#define GB_VITA 3
#define GB_3DS 4
#define GB_SWITCH 5
#define GB_ANDROID 6

#define GBSND_NONE 0
#define GBSND_SDL 1
#define GBSND_SOLOUD 2
#define GBSND_3DS 3
#define GBSND_VITA 4

#define GBREND_NONE 0
#define GBREND_VITA2D 1
#define GBREND_SDL 2
#define GBREND_SF2D 3

#define GBTXT_NONE 0
#define GBTXT_BITMAP 1
#define GBTXT_VITA2D 2
#define GBTXT_FONTCACHE 3

#if _WIN32
	#define GBPLAT GB_WINDOWS
#elif __ANDROID__
	#define GBPLAT GB_ANDROID
#elif __unix__
	#define GBPLAT GB_LINUX
#elif __vita__
	#define GBPLAT GB_VITA
#elif _3DS
	#define GBPLAT GB_3DS
#elif __SWITCH__
	#define GBPLAT GB_SWITCH
#else
	#error Could not get platform
#endif

// If true, define
//	int fixX(int x)
//	int fixY(int y)
#define DOFIXCOORDS 0

#if GBPLAT == GB_WINDOWS || GBPLAT == GB_LINUX || GBPLAT == GB_ANDROID
	#define GBREND GBREND_SDL
	#define GBTXT GBTXT_FONTCACHE
	#define GBSND GBSND_SDL
#elif GBPLAT == GB_VITA
	#define GBREND GBREND_VITA2D
	#define GBTXT GBTXT_VITA2D
	#define GBSND GBSND_VITA
#elif GBPLAT == GB_3DS
	#define GBREND GBREND_SF2D
	#define GBTXT GBTXT_BITMAP
	#define GBSND GBSND_3DS
#elif GBPLAT == GB_SWITCH
	#define GBREND GBREND_SDL
	#define GBTXT GBTXT_FONTCACHE
	#define GBSND GBSND_NONE
#endif

// Define these manually in your code anywhere
extern char* vitaAppId;
extern char* androidPackageName;