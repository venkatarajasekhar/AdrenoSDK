
#pragma once

#include "MyUtils.h"
#include <SLES/OpenSLES.h>

#pragma region AudioSystem class

//===========================================================================================================================
//
// AudioSystem class
//
//===========================================================================================================================

class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();

private:
	SLresult initSLES();
	void destroySLES();

private:
	SLObjectItf m_engineObject;
	SLEngineItf m_engine;
	SLObjectItf m_outputMixObject;

private:
	friend class Audio;
};

#pragma endregion

//===========================================================================================================================
//
// Audio class
//
//===========================================================================================================================

class Audio
{
public:
	Audio();
	~Audio();

	SLresult init(const MyString& filename, bool loop = false);
	void play();
	void stop();
	void setVolume(float volume);

private:
	void destroy();

private:
	SLObjectItf m_playerObject;
	SLPlayItf   m_playerPlay;

	SLVolumeItf m_volumeItf;
	SLSeekItf   m_seekItf;

private:
	static AudioSystem m_audioSys;
};

