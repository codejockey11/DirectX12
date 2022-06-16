#ifndef _CSound3D
#define _CSound3D

#include "standard.h"

#include "CErrorLog.h"
#include "CSoundDevice.h"
#include "CString.h"
#include "CWavLoader.h"

class CSound3D
{
public:
	
	CString*			filename;

	X3DAUDIO_LISTENER	listener;
	X3DAUDIO_EMITTER	emitter;
	X3DAUDIO_CONE		emitterCone;
		
	X3DAUDIO_DSP_SETTINGS dspSettings;

	X3DAUDIO_VECTOR		listenerPos;
	X3DAUDIO_VECTOR		emitterPos;

	FLOAT		listenerAngle;
		
	BOOL		useListenerCone;
	BOOL		useInnerRadius;
	BOOL		useRedirectToLFE;

	CSound3D();
	
	CSound3D(CErrorLog* el, CSoundDevice* d, const CHAR* fn, BOOL loop, X3DAUDIO_VECTOR* p, float r);

	~CSound3D();

	void SetListener(X3DAUDIO_VECTOR* p, X3DAUDIO_VECTOR* l, X3DAUDIO_VECTOR* u);
	void StartSound();
	void StopSound();
	void Shutdown();

private:
	
	HRESULT			hr;
	CErrorLog*		errorLog;
	CSoundDevice*	soundDevice;
	CWavLoader*		wavLoader;

	IXAudio2SourceVoice*	sourceVoice;

	XAUDIO2_BUFFER	buffer;

	FLOAT32*		matrix;

	DWORD			dwCalcFlags;

	X3DAUDIO_VECTOR	distance;
	INT				count;
	FLOAT			range;
};
#endif