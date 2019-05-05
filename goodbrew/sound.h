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
	typedef WavStream* crossMusic;
	typedef Wav* crossSE;
	typedef unsigned int crossPlayHandle;
#elif GBSND == GBSND_3DS
	#include "3dsSound.h"
	typedef NathanMusic* crossMusic;
	typedef NathanMusic* crossSE;
	typedef unsigned char crossPlayHandle;
#elif GBSND == GBSND_VITA
	#include "VitaSound.h"
	typedef NathanAudio* crossMusic;
	typedef crossMusic crossSE;
	typedef crossMusic crossPlayHandle;
#elif GBSND == GBSND_NONE
	typedef int crossSE;
	typedef int crossMusic;
	typedef int crossPlayHandle;
#endif

void fadeoutMusic(crossPlayHandle _passedHandle,int time);
void freeMusic(crossMusic toFree);
void freeSound(crossSE toFree);
char initAudio();
crossMusic loadMusic(char* filepath);
crossSE loadSound(char* filepath);
char mp3Supported();
char oggSupported();
crossPlayHandle playMusic(crossMusic toPlay, unsigned char _passedChannel);
crossPlayHandle playSound(crossSE toPlay, unsigned char _passedChannel);
void quitAudio();
void setMusicVolumeBefore(crossMusic _passedMusic,int vol);
void setMusicVolume(crossPlayHandle _passedMusic,int vol);
void setSFXVolumeBefore(crossSE tochange, int toval);
void setSFXVolume(crossPlayHandle tochange, int toval);
void stopMusic(crossPlayHandle toStop);
void stopSound(crossSE toStop);

#endif