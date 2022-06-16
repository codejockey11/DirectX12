#ifndef _CSoundDevice
#define _CSoundDevice

#include "standard.h"

#include "CErrorLog.h"

class CSoundDevice
{
public:

	IXAudio2* xAudio2;
	
	X3DAUDIO_HANDLE x3DInstance;

	IXAudio2MasteringVoice* masteringVoice;

	DWORD channelMask;
	
	CSoundDevice();

	CSoundDevice(CErrorLog* el);

	~CSoundDevice();

private:

	HRESULT hr;

	CErrorLog* errorLog;

	UINT32 count;

	IMMDeviceEnumerator* pEnumerator;
	IMMDeviceCollection* pCollection;
	IMMDevice* pEndpoint;
	IPropertyStore* pProps;
	LPWSTR pwszID;

	PROPVARIANT varName;

	CLSID CLSID_MMDeviceEnumerator;
	IID IID_IMMDeviceEnumerator;
};
#endif