
#include "MyAudio.h"

#if defined WIN32 || defined _WIN32
#include <SLES-Win32/OpenSLES_SRSLabs.h>
#include <SLES-Win32/OpenSLES_Android.h>
#elif defined __ANDROID__
#include <SLES/OpenSLES_Android.h>
#endif

#include <math.h>

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

#pragma region Helper functions

//===========================================================================================================================
//
// Helper functions
//
//===========================================================================================================================

#if defined(_WIN32) || defined(WIN32)
// This will get the window handle of a console program. This not work on a windowed program.
static HWND GetConsoleHandle(void)
{
	const int   BufferSize = 512;
	LPSTR       pOldTitle = (LPSTR)calloc(BufferSize, sizeof(TCHAR));
	LPSTR       pNewTitle = (LPSTR)calloc(BufferSize, sizeof(TCHAR));
	HWND        WindowHandle = NULL;

	// Get current title
	GetConsoleTitle(pOldTitle, BufferSize);

	// Create new unique title
	wsprintf(pNewTitle, TEXT("OpenSLES-123456"));
	SetConsoleTitle(pNewTitle);
	Sleep(10);                                         // Make sure the window get the new title before we continue
	WindowHandle = FindWindow(NULL, pNewTitle);        // Find the window with unique title
	SetConsoleTitle(pOldTitle);                        // Restore old title

	// Free buffers
	free(pOldTitle);
	free(pNewTitle);
	return(WindowHandle);
}
#endif // WIN32

static SLresult GetMimeType(const SLchar* pUri, SLchar* pMimeType)
{
	if (!pUri || !pMimeType)
		return SL_RESULT_PARAMETER_INVALID;

	*pMimeType = '\0';
	if (strstr((const char*)pUri, ".wav"))        strcpy((char*)pMimeType, "audio/wave");
	else if (strstr((const char*)pUri, ".ogg"))   strcpy((char*)pMimeType, "audio/ogg");
	else if (strstr((const char*)pUri, ".mp3"))   strcpy((char*)pMimeType, "audio/mpeg");

	return (*pMimeType != '\0') ? SL_RESULT_SUCCESS : SL_RESULT_PARAMETER_INVALID;
}

#pragma endregion

#pragma region AudioSystem class

//===========================================================================================================================
//
// AudioSystem class
//
//===========================================================================================================================

AudioSystem::AudioSystem()
{
	initSLES(SL_BOOLEAN_FALSE);
}

AudioSystem::~AudioSystem()
{
	destroySLES();
}

SLresult AudioSystem::initSLES(SLboolean bAddPresetReverb)
{
	SLresult 			        Result = 0;
	SLuint32                    nInterfaces = 0;
	SLInterfaceID               Interfaces[10];
	SLboolean                   bRequired[10];
	SLAndroidConfigurationItf   ConfigItf = 0;

	// Create Engine
#if defined WIN32 || defined _WIN32
	Interfaces[nInterfaces] = SL_IID_ANDROIDCONFIGURATION; 
	bRequired[nInterfaces] = SL_BOOLEAN_TRUE; 
	++nInterfaces;
#endif

	slCreateEngine(&m_engineObj, 0, 0, nInterfaces, Interfaces, bRequired);
	SLES_RESULT(Result, "ERROR: Can not create audio system.");

	Result = (*m_engineObj)->Realize(m_engineObj, SL_BOOLEAN_FALSE);
	SLES_RESULT(Result, "ERROR: Can not create audio system.");

	Result = (*m_engineObj)->GetInterface(m_engineObj, SL_IID_ENGINE, &m_engineItf);
	SLES_RESULT(Result, "ERROR: Can not create audio system.");

#if defined WIN32 || defined _WIN32
	{
		HWND hWnd = 0;
		hWnd = GetConsoleHandle();

		Result = (*m_engineObj)->GetInterface(m_engineObj, SL_IID_ANDROIDCONFIGURATION, &ConfigItf);
		SLES_RESULT(Result, "ERROR: Can not create audio system.");

		if (ConfigItf) 
		{
			Result = (*ConfigItf)->SetConfiguration(ConfigItf, SL_SRSLABS_CONFIGKEY_WINDOW_HANDLE, &hWnd, sizeof(hWnd));
			SLES_RESULT(Result, "ERROR: Can not create audio system.");
		}
	}
#endif // WIN32

	// Create Output Mix
	if (bAddPresetReverb)
	{
		Interfaces[nInterfaces] = SL_IID_PRESETREVERB; 
		bRequired[nInterfaces] = SL_BOOLEAN_TRUE; 
		nInterfaces++;
	}

	Result = (*m_engineItf)->CreateOutputMix(m_engineItf, &m_outputMixObj, nInterfaces, Interfaces, bRequired);
	SLES_RESULT(Result, "ERROR: Can not create audio system.");

	Result = (*m_outputMixObj)->Realize(m_outputMixObj, SL_BOOLEAN_FALSE);
	SLES_RESULT(Result, "ERROR: Can not create audio system.");

	Result = (*m_outputMixObj)->GetInterface(m_outputMixObj, SL_IID_OUTPUTMIX, &m_outputMixItf);
	SLES_RESULT(Result, "ERROR: Can not create audio system.");

	if (bAddPresetReverb)
	{
		Result = (*m_outputMixObj)->GetInterface(m_outputMixObj, SL_IID_PRESETREVERB, &m_presetReverbItf);
		SLES_RESULT(Result, "ERROR: Can not create audio system.");
	}

	return Result;
}

void AudioSystem::destroySLES()
{
	if (m_listenerObj)
	{
		(*m_listenerObj)->Destroy(m_listenerObj);
		m_listenerObj = NULL;
	}

	if (m_outputMixObj)
	{
		(*m_outputMixObj)->Destroy(m_outputMixObj);
		m_outputMixObj = 0;
	}

	if (m_engineObj)
	{
		(*m_engineObj)->Destroy(m_engineObj);
		m_engineObj = 0;
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
	: m_object(0),
	m_playItf(0),
	m_volumeItf(0),
	m_seekItf(0),
	m_threeDSourceItf(0),
	m_threeDLocationItf(0),
	m_bassBoostItf(0),
	m_equalizerItf(0),
	m_virtualizerItf(0),
	m_bufferQueueItf(0)
{
}

Audio::~Audio()
{
	destroy();
}

SLresult Audio::init(const MyString& filename, bool loop)
{
	SLresult					Result = 0;

	SLDataSource                DataSource;
	SLDataLocator_URI           DataLocatorURISource;
	SLDataFormat_MIME           DataFormatMIME;
	SLchar                      zMimeType[32];

	SLDataSink                  DataSink;
	SLDataLocator_OutputMix     DataLocatorSink;


	SLuint32                    nInterfaces = 0;
	SLInterfaceID               Interfaces[16];
	SLboolean                   bRequired[16];

	AudioSystem* pSystem = &m_audioSys;

	const SLchar* pzUri = (const SLchar*)filename.c_str();

	// Get the mime type
	Result = GetMimeType(pzUri, zMimeType);
	SLES_RESULT(Result, "ERROR " + filename + ": File type is not unsupported");

	// Setup the Source
	DataLocatorURISource.locatorType = SL_DATALOCATOR_URI;
	DataLocatorURISource.URI = (SLchar*)pzUri;
	DataFormatMIME.formatType = SL_DATAFORMAT_MIME;
	DataFormatMIME.containerType = SL_CONTAINERTYPE_WAV;
	DataFormatMIME.mimeType = zMimeType;

	DataSource.pLocator = &DataLocatorURISource;
	DataSource.pFormat = &DataFormatMIME;

	// Setup Sink
	DataLocatorSink.locatorType = SL_DATALOCATOR_OUTPUTMIX;
	DataLocatorSink.outputMix = pSystem->m_outputMixObj;

	DataSink.pLocator = &DataLocatorSink;
	DataSink.pFormat = 0; //Ignored with OutputMix

	// Explicit Interfaces
	Interfaces[nInterfaces] = SL_IID_VOLUME;    bRequired[nInterfaces] = SL_BOOLEAN_TRUE; nInterfaces++;
	Interfaces[nInterfaces] = SL_IID_SEEK;      bRequired[nInterfaces] = SL_BOOLEAN_TRUE; nInterfaces++;

	// Player
	Result = (*pSystem->m_engineItf)->CreateAudioPlayer(
		pSystem->m_engineItf, 
		&m_object, 
		&DataSource, 
		&DataSink, 
		nInterfaces, 
		Interfaces, 
		bRequired);
	SLES_RESULT(Result, "ERROR " + filename + ": Can not initialzing audio.");

	Result = (*(m_object))->Realize(m_object, SL_BOOLEAN_FALSE);
	SLES_RESULT(Result, "ERROR " + filename + ": Can not initialzing audio.");

	// Cache Interfaces
	Result = (*(m_object))->GetInterface(m_object, SL_IID_PLAY, &m_playItf);
	SLES_RESULT(Result, "ERROR " + filename + ": Can not initialzing audio.");

	Result = (*(m_object))->GetInterface(m_object, SL_IID_VOLUME, &m_volumeItf);
	SLES_RESULT(Result, "ERROR " + filename + ": Can not initialzing audio.");

	Result = (*(m_object))->GetInterface(m_object, SL_IID_SEEK, &m_seekItf);
	SLES_RESULT(Result, "ERROR " + filename + ": Can not initialzing audio.");

	// We are looping the sound sample so that we can keep the wave file size down.
	// You can remove this block if you dont want the sound to loop
	if (loop && m_seekItf)
	{
		SLmillisecond Dur = 0;
		Result = (*(m_playItf))->GetDuration(m_playItf, &Dur);
		SLES_RESULT(Result, "ERROR " + filename + ": Can not initialzing audio.");

		(*(m_seekItf))->SetLoop(m_seekItf, SL_BOOLEAN_TRUE, 0, Dur);
		SLES_RESULT(Result, "ERROR " + filename + ": Can not initialzing audio.");
	}

	return Result;
}

void Audio::play()
{
	stop();
	if (m_playItf)
	{
		(*(m_playItf))->SetPlayState(m_playItf, SL_PLAYSTATE_PLAYING);
	}
}

void Audio::stop()
{
	if (m_playItf)
	{
		(*(m_playItf))->SetPlayState(m_playItf, SL_PLAYSTATE_STOPPED);
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
	if (m_object)
	{
		(*(m_object))->Destroy(m_object);
		m_object = 0;
	}
}