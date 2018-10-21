#ifndef LIBGOODBREWSOUNDINCLUDED_H
#define LIBGOODBREWSOUNDINCLUDED_H


#if GBSND == GBSND_SDL
	#ifndef _3DS
		#include <SDL2/SDL.h>
		#include <SDL2/SDL_mixer.h>
	#else
		#include <SDL/SDL.h>
		#include <SDL/SDL_mixer.h>
	#endif
	typedef Mix_Chunk* crossSE;
	typedef Mix_Music* crossMusic;
	typedef void* crossPlayHandle;
#elif GBSND == GBSND_SOLOUD
	#include <soloud_c.h>
	typedef WavStream crossMusic;
	typedef Wav crossSE;
	typedef unsigned int crossPlayHandle;
#elif GBSND == GBSND_3DS
	#include "3dsSound.h"
	typedef crossMusic NathanMusic;
	typedef crossSE NathanMusic;
	typedef unsigned char crossPlayHandle;
#elif GBSND == GBSND_VITA
	#include "VitaSound.h"
	typedef NathanAudio crossMusic;
	typedef crossMusic crossSE;
	typedef crossMusic* crossPlayHandle;
#elif GBSND == GBSND_NONE
	typedef int crossSE;
	typedef int crossMusic;
	typedef int crossPlayHandle;
#endif

#endif