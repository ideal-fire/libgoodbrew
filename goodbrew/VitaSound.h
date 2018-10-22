#ifndef LIBGOODBREWVITASOUNDINCLUDED_H
#define LIBGOODBREWVITASOUNDINCLUDED_H
#include <samplerate.h>
#include <psp2/kernel/threadmgr.h>
#include <stdio.h>

typedef struct{
	void* mainAudioStruct; // malloc'd and needs a free function
	void** audioBuffers;  // malloc'd

	float* tempFloatSource; // Temp buffer to hold converted audio data
	float* tempFloatConverted; // malloc'd. This is the destination buffer for the converted audio data before it's copied back to the main buffer.

	SRC_DATA usualConverterData;
	SRC_STATE* personalConverter; // Needs a free function

	signed int nextBufferLoadOffset;

	unsigned int unscaledSamples; // Number of samples to load from the file and convert
	unsigned int scaledSamples; // Number of samples to play and size of destination buffer

	unsigned int numBuffers; // Number of audio buffers we can swap between.
	signed char fileFormat; // Tells you what the file format is, OGG, WAV, etc.
	
	SceUID playerThreadId; // Thread ID for the thread that's playing this sound.
	signed int audioPort;

	signed char shouldLoop; // bool
	signed int volume; // Current volume
	signed char myStreamStatus; // If we should stream or not

	signed char totalPlaying; // Non-streamed data can be played from multiple threads.
	signed char isFirstTime;
	signed char quitStatus; // 0 if should not quit, 1 if should quit, -1 if already quit
	signed char isFadingOut;
	unsigned int fadeoutPerSwap; // In Vita volume units
	signed int lastVolume; // Used to detect volume changes

	FILE* _internalFilePointer; // Do not mess with normally please
}NathanAudio;

#define FILE_FORMAT_NONE 0
#define FILE_FORMAT_OGG 1
#define FILE_FORMAT_MP3 2
// Planned
#define FILE_FORMAT_WAV 3

#endif