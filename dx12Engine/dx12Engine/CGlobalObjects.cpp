#include "CGlobalObjects.h"

CGlobalObjects::CGlobalObjects()
{
	memset(this, 0x00, sizeof(CGlobalObjects));
}

CGlobalObjects::CGlobalObjects(HINSTANCE hInstance, int nCmdShow, const char* cn, const char* t, const char* log, const char* cfg)
{
	memset(this, 0x00, sizeof(CGlobalObjects));

	className = new CString(cn);
	title = new CString(t);
	errorLog = new CErrorLog(log);
	config = new CConfiguration(cfg);

	CLinkListNode* wlln = config->items->Search("windowed");

	CConfigurationItem* wm = (CConfigurationItem*)wlln->element;

	CLinkListNode* swln = config->items->Search("screenWidth");

	CConfigurationItem* sw = (CConfigurationItem*)swln->element;

	CLinkListNode* shln = config->items->Search("screenHeight");

	CConfigurationItem* sh = (CConfigurationItem*)shln->element;

	if (wlln)
	{
		if (wm->value == 1.0F)
		{
			window = new CWindow(hInstance, TRUE, (UINT)sw->value, (UINT)sh->value, className->GetText(), className->GetText());

		}
		else
		{
			window = new CWindow(hInstance, FALSE, (UINT)sw->value, (UINT)sh->value, className->GetText(), className->GetText());
		}
	}

	if (!window->hWnd)
	{
		return;
	}

	ShowWindow(window->hWnd, nCmdShow);

	SetCursorPos(window->middle.x, window->middle.y);

	
	
	CLinkListNode* mslln = config->items->Search("mouseSensitivity");

	CConfigurationItem* ms = (CConfigurationItem*)mslln->element;

	mouse = new CMouseDevice(errorLog, window, ms->value);

	keyboard = new CKeyboardDevice(errorLog);

	videoDevice = new CVideoDevice(errorLog, window);
	fontD12Mgr = new CD12FontManager(errorLog, videoDevice, window);
	soundDevice = new CSoundDevice(errorLog);
	textureMgr = new CTextureManager(errorLog, videoDevice);
	shaderMgr = new CShaderManager(errorLog);
	soundMgr = new CSoundManager(errorLog, soundDevice);
	frametime = new CFrametime();
}

CGlobalObjects::~CGlobalObjects()
{
	delete fontD12Mgr;
	delete textureMgr;
	delete shaderMgr;
	delete soundMgr;
	delete config;
	delete frametime;
	delete keyboard;
	delete mouse;
	delete soundDevice;
	delete videoDevice;
	delete errorLog;
	delete window;
	delete className;
	delete title;
}
