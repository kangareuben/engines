#include "AudioManager.h"

AudioManager::AudioManager()
{
	ALfloat position[3] = { 0, 0, 0 };
	ALfloat velocity[3] = { 0, 0, 0 };
	ALfloat orientation[6] = { 1, 0, 0, 0, 1, 0 };
	alListenerfv(AL_POSITION, position);
	alListenerfv(AL_VELOCITY, velocity);
	alListenerfv(AL_ORIENTATION, orientation);

	//Generate 3 AL buffers
	alGenBuffers(3, uiBuffers);

	//Generate 25 sources to play back data from the buffer
	alGenSources(25, uiSources);

	//Load wave files to buffers
	if (!ALFWLoadWaveToBuffer((char*)("snap.wav"), uiBuffers[0]))
	{
		ALFWprintf("Failed to load %s\n", "snap.wav");
	}

	if (!ALFWLoadWaveToBuffer((char*)("tick.wav"), uiBuffers[1]))
	{
		ALFWprintf("Failed to load %s\n", "tick.wav");
	}

	if (!ALFWLoadWaveToBuffer((char*)("ow.wav"), uiBuffers[2]))
	{
		ALFWprintf("Failed to load %s\n", "ow.wav");
	}

	curSource = 0;
}

AudioManager::~AudioManager()
{
}

void AudioManager::Play()
{
	//Attach source to buffer
	alSourcei(uiSources[curSource], AL_BUFFER, uiBuffers[0]);

	//Play source
	alSourcePlay(uiSources[curSource]);

	IncrementCurSource();
}

void AudioManager::Play(char* fileName)
{
	if (fileName == "snap.wav")
	{
		//Attach source to buffer
		alSourcei(uiSources[curSource], AL_BUFFER, uiBuffers[0]);
	}
	else if (fileName == "tick.wav")
	{
		//Attach source to buffer
		alSourcei(uiSources[curSource], AL_BUFFER, uiBuffers[1]);
	}
	else
	{
		//Attach source to buffer
		alSourcei(uiSources[curSource], AL_BUFFER, uiBuffers[2]);
	}

	//Play source
	alSourcePlay(uiSources[curSource]);

	IncrementCurSource();
}

void AudioManager::Play(char* fileName, ALfloat* position, ALfloat* velocity, ALfloat* orientation)
{
	if (fileName == "snap.wav")
	{
		//Attach source to buffer
		alSourcei(uiSources[curSource], AL_BUFFER, uiBuffers[0]);
	}
	else if (fileName == "tick.wav")
	{
		//Attach source to buffer
		alSourcei(uiSources[curSource], AL_BUFFER, uiBuffers[1]);
	}
	else
	{
		//Attach source to buffer
		alSourcei(uiSources[curSource], AL_BUFFER, uiBuffers[2]);
	}

	//Set the source position, velocity, and orientation
	alSourcefv(uiSources[curSource], AL_POSITION, position);
	alSourcefv(uiSources[curSource], AL_VELOCITY, velocity);
	alSourcefv(uiSources[curSource], AL_DIRECTION, orientation);

	//Play source
	alSourcePlay(uiSources[curSource]);

	IncrementCurSource();
}

void AudioManager::IncrementCurSource()
{
	curSource++;
	if (curSource == 25)
	{
		curSource = 0;
	}
}

void AudioManager::Cleanup()
{
	//Clean up OpenAL
	for (int i = 0; i < 25; i++)
	{
		alSourceStop(uiSources[i]);
	}
	alDeleteSources(25, uiSources);
	alDeleteBuffers(3, uiBuffers);
}
