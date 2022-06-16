#ifndef _CMouseDevice
#define _CMouseDevice

#include "standard.h"

#include "CErrorLog.h"
#include "CKeyMap.h"
#include "CPacket.h"
#include "CWindow.h"
#include "CVertex.h"

class CMouseDevice
{
public:

	enum
	{
		LMB = 0,
		MMB,
		RMB,
		SCR,

		KR_ONESHOT = 0,
		KR_REPEATER = 1,

		BUTTON_COUNT = 4
	};

	LONG lX;
	LONG lY;
	LONG lZ;
	
	BYTE rgbButtons[4];
	
	CVertex2 position;

	POINT pos;

	POINT movePos;

	POINT prevPos;

	bool positionSaved;

	POINT savePos;

	CKeyMap* keyMap[CMouseDevice::BUTTON_COUNT];

	float sensitivity;

	CMouseDevice();

	CMouseDevice(CErrorLog* el, CWindow* w, float ms);

	~CMouseDevice();

	void GetState();

	void SetKeyMap(UINT n, bool kr, BYTE t, BYTE a);

	void MouseMove();

	void MouseClick(UINT message, WPARAM wParam);

	void MouseWheel(WPARAM wParam);

	void FirstPersonReset();

	void SaveLastPosition();

	void SetLastPosition();

private:

	HRESULT hr;

	CErrorLog* errorLog;
	
	CWindow* window;
};
#endif