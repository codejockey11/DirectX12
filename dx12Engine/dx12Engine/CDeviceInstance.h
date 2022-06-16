#ifndef _CDeviceInstance
#define _CDeviceInstance

#include "standard.h"

#include "CString.h"

class CDeviceInstance
{
public:

	CString* instanceName;

	GUID guidInstance;

	CDeviceInstance();

	CDeviceInstance(GUID* g, const CHAR* c);

	~CDeviceInstance();
};
#endif