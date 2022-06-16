#ifndef _CBUTTON
#define _CBUTTON

#include "standard.h"

#include "CGlobalObjects.h"
#include "CHudImage.h"
#include "CIndexBuffer.h"
#include "CPacket.h"
#include "CVertexBuffer.h"

class CButton
{
public:

	BOOL isMouseOver;

	CVertex2 position;
	CVertex2 size;

	CPacket* packet;

	CButton();

	CButton(CGlobalObjects* g, const CHAR* imageOff, const CHAR* imageOn, CVertex2* s, CVertex2* p, BYTE a, BYTE t);

	~CButton();

	VOID Draw();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	CHudImage* textureOff;
	CHudImage* textureOn;
	CHudImage* textureDraw;

	INT soundCount;
};
#endif