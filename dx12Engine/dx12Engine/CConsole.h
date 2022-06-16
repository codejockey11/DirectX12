#ifndef _CCONSOLE
#define _CCONSOLE

#include "standard.h"

#include "CErrorLog.h"
#include "CFontManager.h"
#include "CGlobalObjects.h"
#include "CIndexBuffer.h"
#include "CLinkList.h"
#include "CPacket.h"
#include "CString.h"
#include "CTextureManager.h"
#include "CVertexBuffer.h"
#include "CVideoDevice.h"

class CConsoleVars
{
public:

	FLOAT	fog;
	FLOAT	timeOfDay;
	BYTE		drawWire;

	CConsoleVars()
	{
		memset(this, 0x00, sizeof(CConsoleVars));

		timeOfDay = 1.0F;
	};

	~CConsoleVars() {};
};

class CConsole
{
public:

	CString*				commandLine;
	CLinkList<CString>*	entries;
	CPacket*				packet;
	CConsoleVars*			variables;
		
	INT					entryNbr;
	INT					cursorLocation;
	INT					scroll;
	INT					lines;

	RECT					corners;
		
	CConsole();
	
	CConsole(CGlobalObjects* g, const CHAR* in, const CHAR* fn);

	~CConsole();

	VOID DisplayConsole();
	
	VOID AddCHAR(CHAR wParam);
	
	VOID AddEventMessage(const CHAR* format, ...);
	
	VOID Paging(CHAR wParam);

	VOID Reset();

private:

	HRESULT			hr;

	CGlobalObjects*	globalObjects;

	CTexture*		background;
	CDXFont*			font;

	CVertex2d		vertices[4];

	CVertexBuffer*	vertexBuffer;
	CIndexBuffer*		indexBuffer;

	DWORD			color;
	DWORD			index[6];

	INT				start;

	CHAR				text[1024];

	va_list			argptr;
};
#endif