#ifndef _CClient
#define _CClient

#include "standard.h"

#include "CGlobalObjects.h"
#include "CPacket.h"

class CClient
{
public:

	CGlobalObjects* globalObjects;

	CPacket* packet;

	BYTE connectionState;
	
	SOCKET connectSocket;

	INT clientNbr;

	CHAR hostname[132];

	CClient();

	CClient(CGlobalObjects* g, CPacket* p);

	~CClient();

	BYTE ConnectSocket();

	VOID InitializeWinsock();

	VOID CreateSocket(CPacket* p);

	VOID SendEvent(CPacket* p);

	VOID Disconnect();

	VOID ClientReceive();

	enum
	{
		CS_NOTCONNECTED = 0,
		CS_CONNECTING,
		CS_CONNECTED,

		MAXCONNECTIONSTATE = 0xFF
	};

private:

	struct addrinfo* result, * ptr, hints;

	WSADATA	wsaData;

	INT totalBytes;

	CHAR ip[15];
	
	CHAR port[6];

	INT err;

	SIZE_T i;
	INT c;
};
#endif