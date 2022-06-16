#include "CSound3D.h"

CSound3D::CSound3D()
{
	memset(this, 0x00, sizeof(CSound3D));
}

CSound3D::CSound3D(CErrorLog* el, CSoundDevice* d, const CHAR* fn, BOOL loop, X3DAUDIO_VECTOR* p, float r)
{
	memset(this, 0x00, sizeof(CSound3D));

	errorLog = el;
	soundDevice = d;
	range = r;

	filename = new CString(fn);

	wavLoader = new CWavLoader(errorLog, fn);

	hr = soundDevice->xAudio2->CreateSourceVoice(&sourceVoice, &wavLoader->wfx);

	if (hr < 0)
	{
		errorLog->WriteError("CSound3D::CSound3D::CreateSourceVoice:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	buffer.AudioBytes = wavLoader->header.subchunk2Size;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.pAudioData = wavLoader->data;

	if (loop)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	emitter.Position.x = p->x;
	emitter.Position.y = p->y;
	emitter.Position.z = p->z;

	emitter.ChannelCount = 1;
	emitter.CurveDistanceScaler = 1.0f;

	dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX;

	// deviceDetails overly complicated just use 2 channels
	matrix = new FLOAT32[2];
	
	memset(matrix, 0x00, 2 * sizeof(FLOAT32));
	
	dspSettings.SrcChannelCount = wavLoader->wfx.nChannels;
	dspSettings.DstChannelCount = 2;
	dspSettings.pMatrixCoefficients = matrix;
}

CSound3D::~CSound3D()
{
	delete filename;
	delete wavLoader;
	
	sourceVoice->Stop(0);
	
	sourceVoice->FlushSourceBuffers();

	sourceVoice->DestroyVoice();
}

void CSound3D::SetListener(X3DAUDIO_VECTOR* p, X3DAUDIO_VECTOR* l, X3DAUDIO_VECTOR* u)
{
	memset(&listener, 0x00, sizeof(X3DAUDIO_LISTENER));

	listener.Position = *p;
	listener.OrientFront = *l;
	listener.OrientTop = *u;

	distance.x = listener.Position.x - emitter.Position.x;
	distance.y = listener.Position.y - emitter.Position.y;
	distance.z = listener.Position.z - emitter.Position.z;
	
	XMVECTOR res = DirectX::XMVector3Length(DirectX::XMLoadFloat3(&distance));

	if (DirectX::XMVectorGetX(res) > range)
	{
		count = 0;
		
		CSound3D::StopSound();
	}
	else if (count == 0)
	{
		count = 1;
		
		CSound3D::StartSound();
	}

	X3DAudioCalculate(soundDevice->x3DInstance, &listener, &emitter, dwCalcFlags, &dspSettings);

	// just going to code 2 channels
	sourceVoice->SetOutputMatrix(soundDevice->masteringVoice, wavLoader->wfx.nChannels, 2, dspSettings.pMatrixCoefficients, 0);
}

void CSound3D::Shutdown()
{
	sourceVoice->FlushSourceBuffers();
	
	sourceVoice->DestroyVoice();
}

void CSound3D::StartSound()
{
	hr = sourceVoice->SubmitSourceBuffer(&buffer);

	if (hr < 0)
	{
		errorLog->WriteError("CSound3D::StartSound::SubmitSourceBuffer:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = sourceVoice->Start(0);

	if (hr < 0)
	{
		errorLog->WriteError("CSound3D::StartSound::Start:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}
}

void CSound3D::StopSound()
{
	hr = sourceVoice->Stop(0);

	if (hr < 0)
	{
		errorLog->WriteError("CSound3D::StopSound::Stop:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}
}
