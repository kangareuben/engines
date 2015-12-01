#include "AudioManager.h"

AudioManager::AudioManager()
{
	ALfloat position[3] = { 0, 0, 0 };
	ALfloat velocity[3] = { 0, 0, 0 };
	ALfloat orientation[6] = { 1, 0, 0, 0, 1, 0 };
	alListenerfv(AL_POSITION, position);
	alListenerfv(AL_VELOCITY, velocity);
	alListenerfv(AL_ORIENTATION, orientation);
}

AudioManager::~AudioManager()
{
}

void AudioManager::Play()
{
	//Generate an AL buffer
	alGenBuffers(1, &uiBuffer);

	if (!ALFWLoadWaveToBuffer((char*)("stereo.wav"), uiBuffer))
	{
		ALFWprintf("Failed to load %s\n", "stereo.wav");
	}

	//Generate a source to play back data from the buffer
	alGenSources(1, &uiSource);

	//Attach source to buffer
	alSourcei(uiSource, AL_BUFFER, uiBuffer);

	//Play source
	alSourcePlay(uiSource);

	/*do
	{
	Sleep(100);
	alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
	} while (iState == AL_PLAYING);*/
}

void AudioManager::Play(char* fileName)
{
	//Generate an AL buffer
	alGenBuffers(1, &uiBuffer);

	if (!ALFWLoadWaveToBuffer((char*)fileName, uiBuffer))
	{
		ALFWprintf("Failed to load %s\n", fileName);
	}

	//Generate a source to play back data from the buffer
	alGenSources(1, &uiSource);

	//Attach source to buffer
	alSourcei(uiSource, AL_BUFFER, uiBuffer);

	//Play source
	alSourcePlay(uiSource);
}

void AudioManager::Play(char* fileName, ALfloat* position, ALfloat* velocity, ALfloat* orientation)
{
	//Generate an AL buffer
	alGenBuffers(1, &uiBuffer);

	if (!ALFWLoadWaveToBuffer((char*)fileName, uiBuffer))
	{
		ALFWprintf("Failed to load %s\n", fileName);
	}

	//Generate a source to play back data from the buffer
	alGenSources(1, &uiSource);

	//Attach source to buffer
	alSourcei(uiSource, AL_BUFFER, uiBuffer);

	//Set the source position, velocity, and orientation
	alSourcefv(uiSource, AL_POSITION, position);
	alSourcefv(uiSource, AL_VELOCITY, velocity);
	alSourcefv(uiSource, AL_DIRECTION, orientation);

	//Play source
	alSourcePlay(uiSource);
}

void AudioManager::Cleanup()
{
	//Clean up OpenAL
	alSourceStop(uiSource);
	alDeleteSources(1, &uiSource);
	alDeleteBuffers(1, &uiBuffer);
}
