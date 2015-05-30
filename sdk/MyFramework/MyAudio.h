
#pragma once

#include "MyUtils.h"

#if defined WIN32 || defined _WIN32
#include <SLES-Win32/OpenSLES.h>
#elif defined __ANDROID__
#include <SLES/OpenSLES.h>
#endif

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
	SLresult initSLES(SLboolean bAddPresetReverb);
	void destroySLES();

private:
	SLObjectItf         m_engineObj;
	SLEngineItf         m_engineItf;

	SLObjectItf         m_outputMixObj;
	SLOutputMixItf      m_outputMixItf;
	SLPresetReverbItf   m_presetReverbItf;

	// Used for 3D Sample
	SLObjectItf			m_listenerObj;
	SL3DLocationItf		m_listenerLocItf;
	SLVec3D             m_listenerPos;
	SLVec3D             m_listenerDir;

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
	SLObjectItf			m_object;
	SLPlayItf			m_playItf;

	SLVolumeItf			m_volumeItf;
	SLSeekItf           m_seekItf;

	// Used for 3D Sample 
	SL3DSourceItf       m_threeDSourceItf;
	SL3DLocationItf     m_threeDLocationItf;

	// Used for Effects
	SLBassBoostItf      m_bassBoostItf;
	SLEqualizerItf      m_equalizerItf;
	SLVirtualizerItf    m_virtualizerItf;

	// Used for Streaming
	SLBufferQueueItf    m_bufferQueueItf;

private:
	static AudioSystem m_audioSys;
};

