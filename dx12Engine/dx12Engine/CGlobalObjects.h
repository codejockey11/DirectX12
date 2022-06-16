#ifndef _CGlobalObjects
#define _CGlobalObjects

#include "standard.h"

#include "CConfiguration.h"
#include "CD12FontManager.h"
#include "CErrorLog.h"
#include "CFrametime.h"
#include "CKeyboardDevice.h"
#include "CMouseDevice.h"
#include "CShaderManager.h"
#include "CSoundDevice.h"
#include "CSoundManager.h"
#include "CString.h"
#include "CTextureManager.h"
#include "CVideoDevice.h"
#include "CWindow.h"

class CGlobalObjects
{
public:

	CErrorLog* errorLog;
	CConfiguration* config;
	CD12FontManager* fontD12Mgr;
	CFrametime* frametime;
	CKeyboardDevice* keyboard;
	CMouseDevice* mouse;
	CShaderManager* shaderMgr;
	CSoundDevice* soundDevice;
	CSoundManager* soundMgr;
	CTextureManager* textureMgr;
	CVideoDevice* videoDevice;
	CWindow* window;

	CString* className;
	CString* title;

	CGlobalObjects();

	CGlobalObjects(HINSTANCE hInstance, int nCmdShow, const char* cn, const char* t, const char* log, const char* cfg);

	~CGlobalObjects();
};
#endif