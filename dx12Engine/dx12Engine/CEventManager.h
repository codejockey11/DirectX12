#ifndef _CEventManager
#define _CEventManager

#include "standard.h"

#include "CClient.h"
#include "CGlobalObjects.h"
#include "CPacket.h"
#include "CServer.h"
#include "CString.h"

class CEventManager
{
public:

	CGlobalObjects* globalObjects;

	CServer* server;
	
	CClient* client;

	bool isRunning;
	
	bool consoleVisible;
	
	bool isExiting;

	CEventManager();

	CEventManager(CGlobalObjects* g);

	~CEventManager();

	void SendEvent(CPacket* p);
	
	void ProcessEvent(CPacket* p);

private:

	DWORD rv;

	CPacket* packet;

	HANDLE hThread;

	UINT receiveThreadId;

	CString* chatText;

	SIZE_T pingRate;

	void StartServer();
	
	void ClientConnect(CPacket* p);
	
	void Exit(CPacket* p);
	
	void ConsoleInput(CPacket* p);
	
	void CommandFromConsole();
};
#endif