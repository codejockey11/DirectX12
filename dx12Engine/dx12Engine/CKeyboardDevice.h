#ifndef _CKeyboardDevice
#define _CKeyboardDevice

#include "standard.h"

#include "CErrorLog.h"
#include "CKeyMap.h"
#include "CLinkList.h"
#include "CPacket.h"

class CKeyboardDevice
{
public:

	enum
	{
		KR_ONESHOT = 1,
		KR_REPEATER,

		KEY_COUNT = 256,

		// undefined virtual keys equate to unshifted ascii value
		VK_A = 65,
		VK_D = 68,
		VK_S = 83,
		VK_W = 87,
		VK_EQUALS = 187
	};

	char keys[CKeyboardDevice::KEY_COUNT];

	CKeyMap* keyMap[CKeyboardDevice::KEY_COUNT];

	CKeyboardDevice();

	CKeyboardDevice(CErrorLog* el);

	~CKeyboardDevice();

	void GetState();

	void SetKeyMap(UINT n, BYTE kr, BYTE t, BYTE a);

	void KeyDown(UINT message, WPARAM wParam);

private:

	HRESULT hr;

	CErrorLog* errorLog;
};
#endif