#include "CKeyboardDevice.h"

CKeyboardDevice::CKeyboardDevice()
{
	memset(this, 0x00, sizeof(CKeyboardDevice));
}

CKeyboardDevice::CKeyboardDevice(CErrorLog* el)
{
	memset(this, 0x00, sizeof(CKeyboardDevice));

	errorLog = el;

	for (UINT k = 0;k < CKeyboardDevice::KEY_COUNT;k++)
	{
		keyMap[k] = new CKeyMap();
	}

	CKeyboardDevice::SetKeyMap(VK_F1, CKeyboardDevice::KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_STARTSERVER);
	CKeyboardDevice::SetKeyMap(VK_F2, CKeyboardDevice::KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_STOPSERVER);
	CKeyboardDevice::SetKeyMap(VK_F3, CKeyboardDevice::KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_CONNECT);
	CKeyboardDevice::SetKeyMap(VK_F4, CKeyboardDevice::KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_DISCONNECT);

	CKeyboardDevice::SetKeyMap(VK_F8,  CKeyboardDevice::KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_LOADBRUSHMAP);
	CKeyboardDevice::SetKeyMap(VK_F9,  CKeyboardDevice::KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_LOADTERRAIN);
	CKeyboardDevice::SetKeyMap(VK_F10, CKeyboardDevice::KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_LOADORTHOLEVEL);
	CKeyboardDevice::SetKeyMap(VK_F11, CKeyboardDevice::KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_TOGGLECONSOLE);

	CKeyboardDevice::SetKeyMap(CKeyboardDevice::VK_EQUALS, CKeyboardDevice::KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_WINDOWMODE);

	CKeyboardDevice::SetKeyMap(VK_UP,    CKeyboardDevice::KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_MOVE_UP);
	CKeyboardDevice::SetKeyMap(VK_LEFT,  CKeyboardDevice::KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_MOVE_LEFT);
	CKeyboardDevice::SetKeyMap(VK_RIGHT, CKeyboardDevice::KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_MOVE_RIGHT);
	CKeyboardDevice::SetKeyMap(VK_DOWN,  CKeyboardDevice::KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_MOVE_DOWN);

	CKeyboardDevice::SetKeyMap(VK_ESCAPE, CKeyboardDevice::KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_EXIT);

	CKeyboardDevice::SetKeyMap(CKeyboardDevice::VK_W, CKeyboardDevice::KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_FORWARD);
	CKeyboardDevice::SetKeyMap(CKeyboardDevice::VK_S, CKeyboardDevice::KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_BACKWARD);
	CKeyboardDevice::SetKeyMap(CKeyboardDevice::VK_A, CKeyboardDevice::KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_STRAFELEFT);
	CKeyboardDevice::SetKeyMap(CKeyboardDevice::VK_D, CKeyboardDevice::KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_STRAFERIGHT);

	CKeyboardDevice::SetKeyMap(VK_SPACE, CKeyboardDevice::KR_ONESHOT, CPacket::PT_GLOBAL, CPacket::PT_ATTACK);
}

CKeyboardDevice::~CKeyboardDevice()
{
	for (UINT k = 0;k < CKeyboardDevice::KEY_COUNT;k++)
	{
		delete keyMap[k]->packet;
		
		delete keyMap[k];
	}
}

void CKeyboardDevice::GetState()
{
	for (UINT k = 0;k < CKeyboardDevice::KEY_COUNT;k++)
	{
		if (keys[k])
		{
			keyMap[k]->count++;
		}
		else
		{
			keyMap[k]->count = 0;
		}
	}
}

void CKeyboardDevice::SetKeyMap(UINT n, BYTE kr, BYTE a, BYTE t)
{
	keyMap[n]->count = 0;
	keyMap[n]->repeater = kr;

	keyMap[n]->packet = new CPacket(0, 0, a, t, t, 1);
}

void CKeyboardDevice::KeyDown(UINT message, WPARAM wParam)
{
	switch (message)
	{
		case WM_KEYDOWN:
		{
			keys[wParam] = 1;

			break;
		}

		case WM_KEYUP:
		{
			keys[wParam] = 0;

			break;
		}
	}
}
