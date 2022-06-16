#ifndef _CWindow
#define _CWindow

#include "standard.h"

class CWindow
{
public:

	UINT posX;
	UINT posY;

	UINT width;
	UINT height;

	HWND hWnd;
	
	HINSTANCE instance;

	BYTE windowed;
	
	BYTE visible;

	POINT middle;

	RECT windowRect;
	
	RECT clientRect;

	POINT clientToScreen;

	UINT returnFromFullscreen;

	float dpi;

	CWindow();

	CWindow(HINSTANCE i, BYTE m, UINT w, UINT h, const char* className, const char* title);

	~CWindow();

	RECT GetWindowRectangle();

	RECT GetClientRectangle();

	POINT GetClientToScreen();

	float GetDpi();
};
#endif