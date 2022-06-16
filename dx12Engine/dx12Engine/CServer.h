#ifndef _CSERVER
#define _CSERVER

#include "standard.h"

#include "CClientSocket.h"
#include "CGlobalObjects.h"
#include "CLinkList.h"
#include "CPacket.h"

class CServer
{
public:

	CGlobalObjects* globalObjects;

	SOCKET listenSocket;

	CLinkList* clients;

	INT client;

	BYTE acceptRunning;
	
	BYTE serverWorkerRunning;

	CServer();

	CServer(CGlobalObjects* g, const CHAR* p);

	~CServer();

	VOID InitializeWinsock();

	VOID CreateListenSocket(const CHAR* port);

	VOID BindListenSocket();

	VOID StartListenSocket();

	VOID Send(CPacket* p);

	VOID SendNetwork(CPacket* p, CClientSocket* cs);

	VOID Shutdown();

	VOID ProcessEvent(CPacket* p);

private:

	DWORD rv;

	WSADATA wsaData;

	INT totalBytes;
	
	INT err;

	struct addrinfo* result, * ptr, hints;

	HANDLE hThread[2];

	UINT listenThreadId;
	
	UINT workerThreadId;
};
#endif

