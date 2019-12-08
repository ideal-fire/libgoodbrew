#define GBNO 0
#define GBYES 1
#define GBPREFERRED 3 // is GBYES and bit 2
#define GBMAYBE 4
#if GBPLAT == GB_WINDOWS || GBPLAT == GB_LINUX
	#define gbHasTouch() GBYES
	#define gbHasButtons() GBPREFERRED
#elif GBPLAT == GB_ANDROID
	#define gbHasTouch() GBPREFERRED
	#define gbHasButtons() GBMAYBE
#elif GBPLAT == GB_VITA
	#define gbHasTouch() GBYES
	#define gbHasButtons() GBPREFERRED
#elif GBPLAT == GB_3DS
	#define gbHasTouch() GBNO
	#define gbHasButtons() GBPREFERRED
#elif GBPLAT == GB_SWITCH
	#define gbHasTouch() GBNO // support eventually maybe
	#define gbHasButtons() GBPREFERRED
#else
	#warning todo - make goodbrew platform.h constant for this platform
	#define gbHasTouch() GBNO
	#define gbHasButtons() GBPREFERRED
#endif
