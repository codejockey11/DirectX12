#include "CSoundDevice.h"

CSoundDevice::CSoundDevice()
{
	memset(this, 0x00, sizeof(CSoundDevice));
}

CSoundDevice::CSoundDevice(CErrorLog* el)
{
	memset(this, 0x00, sizeof(CSoundDevice));

	errorLog = el;

	CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

	// Get the audio devices on the system
	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);

	if (hr < 0)
	{
		errorLog->WriteError("CSoundDevice::CSoundDevice::CoCreateInstance:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pCollection);

	if (hr < 0)
	{
		errorLog->WriteError("CSoundDevice::CSoundDevice::EnumAudioEndpoints:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = pCollection->GetCount(&count);

	if (hr < 0)
	{
		errorLog->WriteError("CSoundDevice::CSoundDevice::GetCount:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	for (UINT i = 0; i < count; i++)
	{
		// Get pointer to endpoint number i.
		hr = pCollection->Item(i, &pEndpoint);

		// Get the endpoint ID string.
		hr = pEndpoint->GetId(&pwszID);

		hr = pEndpoint->OpenPropertyStore(STGM_READ, &pProps);

		// Initialize container for property value.
		PropVariantInit(&varName);

		// Get the endpoint's friendly-name property.
		hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);

		// Print endpoint friendly name and endpoint ID.
		errorLog->WriteError("CSoundDevice::DeviceInformation::%d:%S\n", i, varName.pwszVal);
		errorLog->WriteError("CSoundDevice::DeviceInformation::%d:%S\n", i, pwszID);

		CoTaskMemFree(pwszID);
		pwszID = NULL;
		PropVariantClear(&varName);

		pProps->Release();
		pEndpoint->Release();
	}

	pEnumerator->Release();
	pCollection->Release();


	// Initialize XAudio2
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (hr < 0)
	{
		errorLog->WriteError("CSoundDevice::CSoundDevice::CoInitializeEx:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = XAudio2Create(&xAudio2, XAUDIO2_DEBUG_ENGINE, XAUDIO2_DEFAULT_PROCESSOR);

	if (hr < 0)
	{
		errorLog->WriteError("CSoundDevice::CSoundDevice::XAudio2Create:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = xAudio2->CreateMasteringVoice(&masteringVoice);

	if (hr < 0)
	{
		errorLog->WriteError("CSoundDevice::CSoundDevice::CreateMasteringVoice:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	masteringVoice->GetChannelMask(&channelMask);

	X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, x3DInstance);
}

CSoundDevice::~CSoundDevice()
{
	if (xAudio2)
	{
		xAudio2->Release();
	}

	CoUninitialize();
}