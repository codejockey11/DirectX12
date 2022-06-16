#include "CSound.h"

CSound::CSound()
{
	memset(this, 0x00, sizeof(CSound));
}

CSound::CSound(CErrorLog* el, CSoundDevice* d, const CHAR* fn, BYTE loop)
{
	memset(this, 0x00, sizeof(CSound));

	errorLog = el;
	soundDevice = d;

	filename = new CString(fn);

	wavLoader = new CWavLoader(errorLog, fn);

	for (UINT i = 0; i < 16; i++)
	{
		hr = soundDevice->xAudio2->CreateSourceVoice(&sourceVoice[i], &wavLoader->wfx);

		if (hr < 0)
		{
			errorLog->WriteError("CSound::CSound::CreateSourceVoice:%s\n", errorLog->GetComErrorMessage(hr));

			return;
		}
	}

	buffer.AudioBytes = wavLoader->header.subchunk2Size;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.pAudioData = wavLoader->data;

	if (loop)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}


	// Princeton Digital Reverb
	hr = XAudio2CreateReverb(&xapoEffect[CSound::SE_REVERB]);

	if (hr < 0)
	{
		errorLog->WriteError("CSound::CSound::XAudio2CreateReverb:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = CreateFX(__uuidof(FXEcho), &xapoEffect[CSound::SE_ECHO]);

	if (hr < 0)
	{
		errorLog->WriteError("CSound::CSound::CreateFX::FXEcho:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = CreateFX(__uuidof(FXReverb), &xapoEffect[CSound::SE_REVERBXAPO]);

	if (hr < 0)
	{
		errorLog->WriteError("CSound::CSound::CreateFX::FXReverb:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	xapoReverb.Diffusion = FXREVERB_DEFAULT_DIFFUSION;
	xapoReverb.RoomSize = FXREVERB_DEFAULT_ROOMSIZE;

	xapoEcho.Delay = 250.0F;	// FXECHO_DEFAULT_DELAY;
	xapoEcho.Feedback = 0.50f;	// FXECHO_DEFAULT_FEEDBACK;
	xapoEcho.WetDryMix = 0.50f;	// FXECHO_DEFAULT_WETDRYMIX;

	reverbParameters.ReflectionsDelay = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY;
	reverbParameters.ReverbDelay = XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY;
	reverbParameters.RearDelay = XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY;
	reverbParameters.PositionLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	reverbParameters.PositionRight = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	reverbParameters.PositionMatrixLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	reverbParameters.PositionMatrixRight = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	reverbParameters.EarlyDiffusion = XAUDIO2FX_REVERB_DEFAULT_EARLY_DIFFUSION;
	reverbParameters.LateDiffusion = XAUDIO2FX_REVERB_DEFAULT_LATE_DIFFUSION;
	reverbParameters.LowEQGain = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN;
	reverbParameters.LowEQCutoff = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF;
	reverbParameters.HighEQGain = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN;
	reverbParameters.HighEQCutoff = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF;
	reverbParameters.RoomFilterFreq = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ;
	reverbParameters.RoomFilterMain = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN;
	reverbParameters.RoomFilterHF = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF;
	reverbParameters.ReflectionsGain = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN;
	reverbParameters.ReverbGain = XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN;
	reverbParameters.DecayTime = XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME;
	reverbParameters.Density = XAUDIO2FX_REVERB_DEFAULT_DENSITY;
	reverbParameters.RoomSize = XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE;
	reverbParameters.WetDryMix = XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX;

	//XAUDIO2FX_REVERB_I3DL2_PARAMETERS ppp = XAUDIO2FX_I3DL2_PRESET_STONEROOM;
	//ReverbConvertI3DL2ToNative(&ppp, &reverbParameters);

	effectDesc[CSound::SE_REVERB].InitialState = FALSE;
	effectDesc[CSound::SE_REVERB].OutputChannels = wavLoader->wfx.nChannels;
	effectDesc[CSound::SE_REVERB].pEffect = xapoEffect[CSound::SE_REVERB];

	effectDesc[CSound::SE_ECHO].InitialState = FALSE;
	effectDesc[CSound::SE_ECHO].OutputChannels = wavLoader->wfx.nChannels;
	effectDesc[CSound::SE_ECHO].pEffect = xapoEffect[CSound::SE_ECHO];

	effectDesc[CSound::SE_REVERBXAPO].InitialState = FALSE;
	effectDesc[CSound::SE_REVERBXAPO].OutputChannels = wavLoader->wfx.nChannels;
	effectDesc[CSound::SE_REVERBXAPO].pEffect = xapoEffect[CSound::SE_REVERBXAPO];


	effectChain.EffectCount = CSound::EFFECT_COUNT;
	effectChain.pEffectDescriptors = effectDesc;

	CSound::SetEffectChain();

	CSound::SetEffectParameters();

	//CSound::ToggleEffect(CSound::SE_ECHO);
	//CSound::ToggleEffect(CSound::SE_REVERB);
	//CSound::ToggleEffect(CSound::SE_REVERBXAPO);

	initialized = TRUE;
}

CSound::~CSound()
{
	delete filename;
	delete wavLoader;

	if (xapoEffect[CSound::SE_REVERB])
	{
		xapoEffect[CSound::SE_REVERB]->Release();
	}
	
	if (xapoEffect[CSound::SE_ECHO])
	{
		xapoEffect[CSound::SE_ECHO]->Release();
	}

	if (xapoEffect[CSound::SE_REVERBXAPO])
	{
		xapoEffect[CSound::SE_REVERBXAPO]->Release();
	}
}

VOID CSound::Shutdown()
{
	for (UINT i = 0; i < 16; i++)
	{
		hr = sourceVoice[i]->FlushSourceBuffers();

		sourceVoice[i]->DestroyVoice();
	}
}

VOID CSound::SetEffectChain()
{
	for (UINT i = 0; i < 16; i++)
	{
		hr = sourceVoice[i]->SetEffectChain(&effectChain);

		if (hr < 0)
		{
			errorLog->WriteError("CSound::SetEffectChain::SetEffectChain:%s\n", errorLog->GetComErrorMessage(hr));

			return;
		}
	}
}

VOID CSound::SetEffectParameters()
{
	for (UINT i = 0; i < 16; i++)
	{
		hr = sourceVoice[i]->SetEffectParameters(CSound::SE_REVERB, &reverbParameters, sizeof(XAUDIO2FX_REVERB_PARAMETERS), XAUDIO2_COMMIT_NOW);

		if (hr < 0)
		{
			errorLog->WriteError("CSound::SetEffectParameters:CSound::SE_REVERB:%s\n", errorLog->GetComErrorMessage(hr));

			return;
		}

		hr = sourceVoice[i]->SetEffectParameters(CSound::SE_ECHO, &xapoEcho, sizeof(FXECHO_PARAMETERS), XAUDIO2_COMMIT_NOW);

		if (hr < 0)
		{
			errorLog->WriteError("CSound::SetEffectParameters:CSound::SE_ECHO:%s\n", errorLog->GetComErrorMessage(hr));

			return;
		}

		hr = sourceVoice[i]->SetEffectParameters(CSound::SE_REVERBXAPO, &xapoReverb, sizeof(FXREVERB_PARAMETERS), XAUDIO2_COMMIT_NOW);

		if (hr < 0)
		{
			errorLog->WriteError("CSound::SetEffectParameters:CSound::SE_REVERBXAPO:%s\n", errorLog->GetComErrorMessage(hr));

			return;
		}
	}
}

VOID CSound::ToggleEffect(BYTE e)
{
	if (isOn[e])
	{
		isOn[e] = FALSE;

		for (UINT i = 0; i < 16; i++)
		{
			hr = sourceVoice[i]->DisableEffect(e);

			if (hr < 0)
			{
				errorLog->WriteError("CSound::ToggleEffect::DisableEffect:%s\n", errorLog->GetComErrorMessage(hr));

				return;
			}
		}
	}
	else
	{
		isOn[e] = TRUE;

		for (UINT i = 0; i < 16; i++)
		{
			hr = sourceVoice[i]->EnableEffect(e);

			if (hr < 0)
			{
				errorLog->WriteError("CSound::ToggleEffect::EnableEffect:%s\n", errorLog->GetComErrorMessage(hr));

				return;
			}
		}
	}
}

VOID CSound::StartSound()
{
	for (UINT i = 0; i < 16; i++)
	{
		sourceVoice[i]->GetState(&vs);

		if (vs.BuffersQueued == 1)
		{
		}
		else
		{
			hr = sourceVoice[i]->SubmitSourceBuffer(&buffer);

			if (hr < 0)
			{
				errorLog->WriteError("CSound::StartSound::SubmitSourceBuffer:%s\n", errorLog->GetComErrorMessage(hr));

				return;
			}

			hr = sourceVoice[i]->Start(0);

			if (hr < 0)
			{
				errorLog->WriteError("CSound::StartSound::Start:%s\n", errorLog->GetComErrorMessage(hr));

				return;
			}

			break;
		}
	}
}

VOID CSound::StopSound()
{
	for (UINT i = 0; i < 16; i++)
	{
		hr = sourceVoice[i]->Stop(0);

		if (hr < 0)
		{
			errorLog->WriteError("CSound::StopSound::Stop:%s\n", errorLog->GetComErrorMessage(hr));

			return;
		}
	}
}