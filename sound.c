#include <goodbrew/config.h>
#include <goodbrew/sound.h>
#if GBSND == GBSND_SOLOUD
	Soloud* mySoLoudEngine;
#endif
#if GBSND == GBSND_VITA
	#include "VitaSoundCode.h"
#endif
char mp3Supported(){
	return (GBSND == GBSND_SDL || GBSND == GBSND_VITA);
}
char oggSupported(){
	return (GBSND==GBSND_SDL || GBSND==GBSND_SOLOUD || GBSND==GBSND_3DS || GBSND==GBSND_VITA);
}
// Returns not 0 if didn't work
char initAudio(){
	#if GBSND == GBSND_SDL
		SDL_Init( SDL_INIT_AUDIO );
		Mix_Init(MIX_INIT_OGG);
		#if PLATFORM != PLAT_3DS
			Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
		#else
			Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 );
		#endif
		return 0;
	#elif GBSND == GBSND_SOLOUD
		mySoLoudEngine = Soloud_create();
		Soloud_init(mySoLoudEngine);
		return 0;
	#elif GBSND == GBSND_3DS
		if (nathanInit3dsSound()==0){
			return 1;
		}
		int i;
		for (i=0;i<=20;i++){
			nathanInit3dsChannel(i);
		}
		return 0;
	#elif GBSND == GBSND_VITA
		// Not needed?
		return 0;
	#else
		return 1;
	#endif
}
void quitAudio(){
	#if GBSND == GBSND_SDL
		// TODO
	#elif GBSND == GBSND_SOLOUD
		// TODO
	#elif GBSND == GBSND_3DS
		ndspExit();
	#elif GBSND == GBSND_VITA
		// Not needed?
	#endif
}
void setMusicVolume(crossPlayHandle _passedMusic,int vol){
	#if GBSND == GBSND_SDL
		Mix_VolumeMusic(vol);
	#elif GBSND == GBSND_SOLOUD
		Soloud_setVolume(mySoLoudEngine,_passedMusic,(float)((float)vol/(float)128));
	#elif GBSND == GBSND_3DS
		nathanSetChannelVolume(_passedMusic,(float)(((double)vol)/128));
	#elif GBSND == GBSND_VITA
		mlgsnd_setVolume(_passedMusic,vol); // On the 128 scale already
	#endif
}
void setMusicVolumeBefore(crossMusic* _passedMusic,int vol){
	#if GBSND == GBSND_SDL
		Mix_VolumeMusic(vol);
	#elif GBSND == GBSND_SOLOUD
		WavStream_setVolume(_passedMusic,(float)((float)vol/(float)128));
	#elif GBSND == GBSND_3DS
		setMusicVolume(_passedMusic->_musicChannel, vol);
	#elif GBSND == GBSND_VITA
		setMusicVolume(_passedMusic,vol);
	#endif
}
void setSFXVolumeBefore(crossSE* tochange, int toval){
	#if GBSND == GBSND_SDL
		Mix_VolumeChunk(tochange,toval);
	#elif GBSND == GBSND_SOLOUD
		Wav_setVolume(tochange,(float)((float)toval/(float)128));
	#elif GBSND == GBSND_3DS
		setMusicVolumeBefore(tochange,toval);
	#elif GBSND == GBSND_VITA
		setMusicVolume(tochange,toval);
	#endif
}
void setSFXVolume(crossPlayHandle tochange, int toval){
	#if GBSND == GBSND_SDL
		Mix_Volume(tochange,toval);
	#elif GBSND == GBSND_SOLOUD
		setMusicVolume(tochange,toval);
	#elif GBSND == GBSND_3DS
		setMusicVolume(tochange,toval);
	#elif GBSND == GBSND_VITA
		setMusicVolume(tochange,toval);
	#endif
}
void fadeoutMusic(crossPlayHandle _passedHandle,int time){
	#if GBSND == GBSND_SDL
		Mix_FadeOutMusic(time);
	#elif GBSND == GBSND_SOLOUD
		Soloud_fadeVolume(mySoLoudEngine,_passedHandle,0,(double)((double)time/(double)1000));
		Soloud_scheduleStop(mySoLoudEngine,_passedHandle,(double)((double)time/(double)1000));
	#elif GBSND == GBSND_3DS
		float _perTenthSecond=(float)((float)1/((double)time/(double)100));
		if (_perTenthSecond==0){
			_perTenthSecond=.00001;
		}
		float _currentFadeoutVolume=1;
		while (_currentFadeoutVolume>0){
			if (_currentFadeoutVolume<_perTenthSecond){
				_currentFadeoutVolume=0;
			}else{
				_currentFadeoutVolume-=_perTenthSecond;
			}
			nathanSetChannelVolume(_passedHandle,_currentFadeoutVolume);
			svcSleepThread(100000000); // Wait for one tenth of a second
		}
		ndspChnWaveBufClear(_passedHandle);
	#elif GBSND == GBSND_VITA
		mlgsnd_fadeoutMusic(_passedHandle,time);
	#endif
}
crossSE* loadSound(char* filepath){
	#if GBSND == GBSND_SDL
		return Mix_LoadWAV(filepath);
	#elif GBSND == GBSND_SOLOUD
		crossSE* _myLoadedSoundEffect = Wav_create();
		Wav_load(_myLoadedSoundEffect,filepath);
		return _myLoadedSoundEffect;
	#elif GBSND == GBSND_3DS
		NathanMusic* _tempReturn = malloc(sizeof(NathanMusic));
		nathanLoadSoundEffect(_tempReturn,filepath);
		return _tempReturn;
	#elif GBSND == GBSND_VITA
		return _mlgsnd_loadAudioFilename(filepath,0,1);
	#elif GBSND == SND_NONE
		return 0;
	#endif
}
crossMusic* loadMusic(char* filepath){
	#if GBSND == GBSND_SDL
		return Mix_LoadMUS(filepath);
	#elif GBSND == GBSND_SOLOUD
		crossMusic* _myLoadedMusic = WavStream_create();
		WavStream_load(_myLoadedMusic,filepath);
		return _myLoadedMusic;
	#elif GBSND == GBSND_3DS
		NathanMusic* _tempReturn = malloc(sizeof(NathanMusic));
		nathanLoadMusic(_tempReturn,filepath,1);
		return _tempReturn;
	#elif GBSND == GBSND_VITA
		return mlgsnd_loadMusicFilename(filepath);
	#elif GBSND == SND_NONE
		return 0;
	#endif
}
void stopMusic(crossPlayHandle toStop){
	#if GBSND == GBSND_SDL
		Mix_HaltMusic();
	#elif GBSND == GBSND_SOLOUD
		Soloud_stop(mySoLoudEngine,toStop);
	#elif GBSND == GBSND_3DS
		ndspChnReset(toStop);
		ndspChnInitParams(toStop);
		nathanInit3dsChannel(toStop);
		ndspChnWaveBufClear(toStop);
	#elif GBSND == GBSND_VITA
		mlgsnd_stopMusic(toStop);
	#endif
}
void stopSound(crossSE* toStop){
	#if GBSND == GBSND_SDL
		//#warning CANT STOP SOUND WITH SDL_MIXER
	#elif GBSND == GBSND_SOLOUD
		Wav_stop(toStop);
	#elif GBSND == GBSND_3DS
		stopMusic(toStop->_musicChannel);
	#elif GBSND == GBSND_VITA
		stopMusic(toStop);
	#endif
}
crossPlayHandle playSound(crossSE* toPlay, unsigned char _passedChannel){
	#if GBSND == GBSND_SDL
		return Mix_PlayChannel( -1, toPlay, 0 );
	#elif GBSND == GBSND_SOLOUD
		return Soloud_play(mySoLoudEngine,toPlay);
	#elif GBSND == GBSND_3DS
		ndspChnWaveBufClear(_passedChannel);
		nathanPlaySound(toPlay, _passedChannel);
		return toPlay->_musicChannel;
	#elif GBSND == GBSND_VITA
		mlgsnd_play(toPlay);
		return toPlay;
	#elif GBSND == SND_NONE
		return 0;
	#endif
}
crossPlayHandle playMusic(crossMusic* toPlay, unsigned char _passedChannel){
	#if GBSND == GBSND_SDL
		Mix_PlayMusic(toPlay,-1);
		return -1;
	#elif GBSND == GBSND_SOLOUD
		WavStream_setLooping(toPlay,1);
		return Soloud_play(mySoLoudEngine,toPlay);
	#elif GBSND == GBSND_3DS
		nathanPlayMusic(toPlay,_passedChannel);
		return toPlay->_musicChannel;
	#elif GBSND == GBSND_VITA
		return playSound(toPlay,_passedChannel);
	#elif GBSND == SND_NONE
		return 1;
	#endif
}
void freeSound(crossSE* toFree){
	#if GBSND == GBSND_SDL
		Mix_FreeChunk(toFree);
	#elif GBSND == GBSND_SOLOUD
		Wav_destroy(toFree);
	#elif GBSND == GBSND_3DS
		nathanFreeSound(toFree);
		free(toFree);
	#elif GBSND == GBSND_VITA
		mlgsnd_freeMusic(toFree);
	#endif
}
void freeMusic(crossMusic* toFree){
	#if GBSND == GBSND_SDL
		Mix_FreeMusic(toFree);
	#elif GBSND == GBSND_SOLOUD
		WavStream_destroy(toFree);
	#elif GBSND == GBSND_3DS
		nathanFreeMusic(toFree);
		free(toFree);
	#elif GBSND == GBSND_VITA
		mlgsnd_freeMusic(toFree);
	#endif
}
