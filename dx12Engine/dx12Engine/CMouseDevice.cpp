#include "CMouseDevice.h"

CMouseDevice::CMouseDevice()
{
	memset(this, 0x00, sizeof(CMouseDevice));
}

CMouseDevice::CMouseDevice(CErrorLog* el, CWindow* w, float ms)
{
	memset(this, 0x00, sizeof(CMouseDevice));

	errorLog = el;
	
	window = w;
	
	sensitivity = ms;

	GetCursorPos(&pos);

	prevPos.x = pos.x;
	prevPos.y = pos.y;

	position.p.x = (float)pos.x;
	position.p.y = (float)pos.y;

	positionSaved = false;

	for (UINT k = 0;k < CMouseDevice::BUTTON_COUNT;k++)
	{
		keyMap[k] = new CKeyMap();
	}

	CMouseDevice::SetKeyMap(CMouseDevice::LMB, CMouseDevice::KR_ONESHOT, CPacket::PT_GLOBAL, CPacket::PT_ATTACK);

	CMouseDevice::SetKeyMap(CMouseDevice::MMB, CMouseDevice::KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_DEFAULT);

	CMouseDevice::SetKeyMap(CMouseDevice::RMB, CMouseDevice::KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_FPSMOVE);
}

CMouseDevice::~CMouseDevice()
{
	for (UINT k = 0;k < CMouseDevice::BUTTON_COUNT;k++)
	{
		delete keyMap[k]->packet;
		
		delete keyMap[k];
	}
}

void CMouseDevice::GetState()
{
	// logic for mouse location to buttons on screen
	GetCursorPos(&pos);

	position.p.x = (float)pos.x;
	position.p.y = (float)pos.y;

	if (window->windowed)
	{
		ScreenToClient(window->hWnd, &pos);
		
		position.p.x = (float)pos.x;
		position.p.y = (float)pos.y + window->returnFromFullscreen;
	}

	// setting keymap for events
	for (UINT k = 0; k < CMouseDevice::BUTTON_COUNT; k++)
	{
		if (rgbButtons[k])
		{
			keyMap[k]->count++;
		}
		else
		{
			keyMap[k]->count = 0;
		}
	}
}

void CMouseDevice::SetKeyMap(UINT n, bool kr, BYTE a, BYTE t)
{
	keyMap[n]->count = 0;
	keyMap[n]->repeater = kr;

	keyMap[n]->packet = new CPacket(0, 0, a, t, t, 1);
}

void CMouseDevice::MouseMove()
{
	GetCursorPos(&movePos);

	lX = movePos.x - prevPos.x;
	lY = movePos.y - prevPos.y;
}

void CMouseDevice::MouseClick(UINT message, WPARAM wParam)
{
	switch (message)
	{
		case WM_LBUTTONDOWN:
		{
			rgbButtons[CMouseDevice::LMB] = 1;

			break;
		}

		case WM_RBUTTONDOWN:
		{
			rgbButtons[CMouseDevice::RMB] = 1;

			break;
		}

		case WM_LBUTTONUP:
		{
			rgbButtons[CMouseDevice::LMB] = 0;

			break;
		}

		case WM_RBUTTONUP:
		{
			rgbButtons[CMouseDevice::RMB] = 0;

			break;
		}

		case WM_MBUTTONDOWN:
		{
			switch (wParam)
			{
				case MK_MBUTTON:
				{
					rgbButtons[CMouseDevice::MMB] = 1;

					break;
				}
			}

			break;
		}

		case WM_MBUTTONUP:
		{
			rgbButtons[CMouseDevice::MMB] = 0;

			break;
		}
	}
}

void CMouseDevice::MouseWheel(WPARAM wParam)
{
	lZ = GET_WHEEL_DELTA_WPARAM(wParam);
}

void CMouseDevice::FirstPersonReset()
{
	// resetting mouse move items
	lX = 0;
	lY = 0;
	lZ = 0;

	SetCursorPos(window->middle.x, window->middle.y);

	prevPos.x = window->middle.x;
	prevPos.y = window->middle.y;
}

void CMouseDevice::SaveLastPosition()
{
	if (positionSaved)
	{
		return;
	}

	GetCursorPos(&savePos);

	positionSaved = true;

	ShowCursor(false);
}

void CMouseDevice::SetLastPosition()
{
	SetCursorPos(savePos.x, savePos.y);

	positionSaved = false;

	ShowCursor(true);
}