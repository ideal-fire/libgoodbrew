#ifndef LIBGOODBREW3DSSOUNDINCLUDED_H
#define LIBGOODBREW3DSSOUNDINCLUDED_H

#include <3ds.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
typedef struct{
	OggVorbis_File _musicOggFile;
	char* _musicMusicBuffer[10];
	ndspWaveBuf _musicWaveBuffer[10];
	int _musicOggCurrentSection; // Use by libvorbis
	char _musicIsTwoBuffers;
	unsigned char _musicChannel;
	unsigned char _musicShoudLoop;
	char _musicIsDone;
}NathanMusic;
void nathanUpdateMusicIfNeeded(NathanMusic* _passedMusic);

#endif