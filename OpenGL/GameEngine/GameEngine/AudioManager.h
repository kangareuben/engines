#pragma once
#include "ALFramework.h"
class AudioManager
{
public:
	AudioManager();
	~AudioManager();
	void Play();
	void Play(char*);
	void Play(char*, ALfloat*, ALfloat*, ALfloat*);
	void Cleanup();
	ALuint uiBuffer;
	ALuint uiSource;
	ALint iState;
};
