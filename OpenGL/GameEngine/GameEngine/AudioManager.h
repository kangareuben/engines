#pragma once
#include "ALFramework.h"
#include <iostream>
using namespace std;
class AudioManager
{
public:
	AudioManager();
	~AudioManager();
	void Play();
	void Play(char*);
	void Play(char*, ALfloat*, ALfloat*, ALfloat*);
	void IncrementCurSource();
	void Cleanup();
	ALuint uiBuffers[3];
	int curSource;
	ALuint uiSources[25];
	ALint iState;
};
