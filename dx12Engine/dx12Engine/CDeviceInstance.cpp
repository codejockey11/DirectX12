#include "CDeviceInstance.h"

CDeviceInstance::CDeviceInstance()
{
	memset(this, 0x00, sizeof(CDeviceInstance));
}

CDeviceInstance::CDeviceInstance(GUID* g, const CHAR* c)
{
	memset(this, 0x00, sizeof(CDeviceInstance));

	memcpy(&guidInstance, g, sizeof(GUID));

	instanceName = new CString(c);
}

CDeviceInstance::~CDeviceInstance()
{
	delete instanceName;
}