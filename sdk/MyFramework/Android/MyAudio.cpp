
#include "MyAudio.h"

#include <SLES/OpenSLES_Android.h>
#include <android/asset_manager.h>

#include <math.h>

#pragma region Global variables

//===========================================================================================================================
//
// Global variables
//
//===========================================================================================================================

extern AAssetManager* g_pAssetManager;

#pragma endregion

#pragma region Helper macros

//===========================================================================================================================
//
// Helper macros
//
//===========================================================================================================================

#define SLES_RESULT(x, y) { if (x != SL_RESULT_SUCCESS) { smartLog(y); return SL_RESULT_UNKNOWN_ERROR; } }

// dB Helpers
#define TODB(_v)		(float)(20.0f*log10f((_v)))
#define TOMB(_v)		(float)(2000.0f*log10f((_v)))
#define FROMDB(_v)	    (powf(10,(_v)/20.0f))

#pragma endregion

#pragma region AudioSystem class

//===========================================================================================================================
//
// AudioSystem class
//
//===========================================================================================================================

AudioSystem::AudioSystem()
{
	initSLES();
}

AudioSystem::~AudioSystem()
{
	destroySLES();
}

SLresult AudioSystem::initSLES()
{
	SLresult result;
	const char* errorMessage = "ERROR: Failed in initialing OpenSL ES ...";

	result = slCreateEngine(&m_engineObject, 0, NULL, 0, NULL, NULL);
	SLES_RESULT(result, errorMessage);

	result = (*m_engineObject)->Realize(m_engineObject, SL_BOOLEAN_FALSE);
	SLES_RESULT(result, errorMessage);

	result = (*m_engineObject)->GetInterface(m_engineObject, SL_IID_ENGINE, &m_engine);
	SLES_RESULT(result, errorMessage);

	result = (*m_engine)->CreateOutputMix(m_engine, &m_outputMixObject, 0, NULL, NULL);
	SLES_RESULT(result, errorMessage);

	result = (*m_outputMixObject)->Realize(m_outputMixObject, SL_BOOLEAN_FALSE);
	SLES_RESULT(result, errorMessage);

	return result;
}

void AudioSystem::destroySLES()
{
	if (m_outputMixObject != NULL)
	{
		(*m_outputMixObject)->Destroy(m_outputMixObject);
		m_outputMixObject = NULL;
	}

	if (m_engineObject != NULL)
	{
		(*m_engineObject)->Destroy(m_engineObject);
		m_engineObject = NULL;
		m_engine = NULL;
	}
}

#pragma endregion

//===========================================================================================================================
//
// Audio class
//
//===========================================================================================================================

AudioSystem Audio::m_audioSys;

Audio::Audio()
	: m_playerObject(nullptr),
	m_playerPlay(nullptr),
	m_volumeItf(0),
	m_seekItf(0)
{
}

Audio::~Audio()
{
	destroy();
}

void SLAPIENTRY play_callback(SLPlayItf player, void *context, SLuint32 event)
{
	if (event & SL_PLAYEVENT_HEADATEND)
	{
		(*player)->SetPlayState(player, SL_PLAYSTATE_STOPPED);
	}
}

SLresult Audio::init(const MyString& filename, bool loop)
{
	SLresult result = SL_RESULT_UNKNOWN_ERROR;
	MyString errorMessage = "ERROR: Can not init audio: " + filename;

	AAsset* asset = AAssetManager_open(g_pAssetManager, filename.c_str(), AASSET_MODE_UNKNOWN);

	if (asset != NULL)
	{
		off_t start;
		off_t length;
		int fd = AAsset_openFileDescriptor(asset, &start, &length);
		if (fd < 0)
		{
			return SL_RESULT_UNKNOWN_ERROR;
		}
		AAsset_close(asset);

		// configure audio source
		SLDataLocator_AndroidFD loc_fd = { SL_DATALOCATOR_ANDROIDFD, fd, start, length };
		SLDataFormat_MIME format_mime = { SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED };
		SLDataSource audioSrc = { &loc_fd, &format_mime };

		// configure audio sink
		SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, m_audioSys.m_outputMixObject };
		SLDataSink audioSnk = { &loc_outmix, NULL };

		const unsigned int NUM_INTERFACES = 3;
		const SLInterfaceID ids[NUM_INTERFACES] = { SL_IID_PLAY, SL_IID_VOLUME, SL_IID_SEEK };
		const SLboolean req[NUM_INTERFACES] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

		result = (*m_audioSys.m_engine)->CreateAudioPlayer(
			m_audioSys.m_engine,
			&m_playerObject,
			&audioSrc,
			&audioSnk,
			NUM_INTERFACES,
			ids,
			req);
		SLES_RESULT(result, errorMessage);

		// realize the player
		result = (*m_playerObject)->Realize(m_playerObject, SL_BOOLEAN_FALSE);
		SLES_RESULT(result, errorMessage);

		// get the play interface
		result = (*m_playerObject)->GetInterface(m_playerObject, SL_IID_PLAY, &m_playerPlay);
		SLES_RESULT(result, errorMessage);
		(*m_playerPlay)->RegisterCallback(m_playerPlay, play_callback, NULL);
		(*m_playerPlay)->SetCallbackEventsMask(m_playerPlay, SL_PLAYEVENT_HEADATEND);

		// get volume interface
		result = (*(m_playerObject))->GetInterface(m_playerObject, SL_IID_VOLUME, &m_volumeItf);
		SLES_RESULT(result, errorMessage);

		// get seek interface
		result = (*(m_playerObject))->GetInterface(m_playerObject, SL_IID_SEEK, &m_seekItf);
		SLES_RESULT(result, errorMessage);

		// We are looping the sound sample so that we can keep the wave file size down.
		// You can remove this block if you dont want the sound to loop
		if (loop && m_seekItf)
		{
			SLmillisecond Dur = 0;
			result = (*(m_playerPlay))->GetDuration(m_playerPlay, &Dur);
			SLES_RESULT(result, errorMessage);

			result = (*(m_seekItf))->SetLoop(m_seekItf, SL_BOOLEAN_TRUE, 0, Dur);
			SLES_RESULT(result, errorMessage);
		}
	}

	return result;
}

void Audio::play()
{
	stop();
	if (m_playerPlay)
	{
		(*(m_playerPlay))->SetPlayState(m_playerPlay, SL_PLAYSTATE_PLAYING);
	}
}

void Audio::stop()
{
	if (m_playerPlay)
	{
		(*(m_playerPlay))->SetPlayState(m_playerPlay, SL_PLAYSTATE_STOPPED);
	}
}

void Audio::setVolume(float volume)
{
	// Clamp 0.0-1.0
	// TODO: Should we return SL_RESULT_PARAMETER_INVALID instead of clamping?
	if (volume < 0.0f) volume = 0.0f;
	if (volume > 1.0f) volume = 1.0f;

	if (m_volumeItf)
	{
		SLmillibel  mBVolume = (volume == 0.0f) ? SL_MILLIBEL_MIN : (SLmillibel)(TOMB(volume)); // Note: mB != dB/1000. mB = 0.001B; dB = 0.1B

		(*(m_volumeItf))->SetVolumeLevel(m_volumeItf, mBVolume);
	}
}

void Audio::destroy()
{
	(*m_playerObject)->Destroy(m_playerObject);

	m_playerObject = nullptr;
	m_playerPlay = nullptr;
}