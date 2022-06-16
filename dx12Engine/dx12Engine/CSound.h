#ifndef _CSound
#define _CSound

#include "standard.h"

#include "CErrorLog.h"
#include "CSoundDevice.h"
#include "CString.h"
#include "CWavLoader.h"

class CSound
{
public:

	enum
	{
		SE_REVERB = 0,
		SE_ECHO,
		SE_REVERBXAPO,

		EFFECT_COUNT = 3,
		
		SOURCE_VOICE_COUNT = 16
	};

	CString* filename;

	BYTE initialized;

	CSound();

	CSound(CErrorLog* el, CSoundDevice* d, const char* fn, BYTE loop);

	~CSound();

	void SetEffectParameters();

	void ToggleEffect(BYTE e);

	void StartSound();

	void StopSound();

	void Shutdown();

private:

	HRESULT hr;

	CErrorLog* errorLog;
	
	CSoundDevice* soundDevice;

	CWavLoader* wavLoader;

	XAUDIO2_EFFECT_DESCRIPTOR effectDesc[CSound::EFFECT_COUNT];

	XAUDIO2_EFFECT_CHAIN effectChain;

	IXAudio2SourceVoice* sourceVoice[CSound::SOURCE_VOICE_COUNT];

	XAUDIO2_BUFFER buffer;

	XAUDIO2_VOICE_STATE vs;

	BYTE isOn[CSound::EFFECT_COUNT];

	IUnknown* xapoEffect[CSound::EFFECT_COUNT];

	XAUDIO2FX_REVERB_PARAMETERS	reverbParameters;
	
	FXECHO_PARAMETERS xapoEcho;
	
	FXREVERB_PARAMETERS xapoReverb;

	void SetEffectChain();
};
#endif