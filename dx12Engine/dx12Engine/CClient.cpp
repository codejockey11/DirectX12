#include "CClient.h"

CClient::CClient()
{
	memset(this, 0x00, sizeof(CClient));
}

CClient::CClient(CGlobalObjects* g, CPacket* p)
{
	memset(this, 0x00, sizeof(CClient));

	globalObjects = g;

	packet = new CPacket();

	CClient::InitializeWinsock();

	CClient::CreateSocket(p);
}

CClient::~CClient()
{
	delete packet;
}

VOID CClient::InitializeWinsock()
{
	WORD wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		globalObjects->errorLog->WriteError("CClient::InitializeWinsock::WSAStartup:%i\n", err);

		return;
	}

	globalObjects->errorLog->WriteError("CClient::InitializeWinsock::WSAStartup:%s\n", wsaData.szDescription);
}

VOID CClient::CreateSocket(CPacket* p)
{
	i = strcspn((CHAR*)p->data, ":");

	memcpy_s(ip, 15, p->data, i);

	i++;

	c = 0;

	for (size_t x = i; x < i + 5; x++)
	{
		memcpy(&port[c], &p->data[x], 1);

		c++;
	}

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(ip, port, &hints, &result);

	if (err != 0)
	{
		globalObjects->errorLog->WriteError("CClient::CreateSocket::getaddrinfo:%i\n", err);

		return;
	}

	ptr = result;

	while (ptr != NULL)
	{
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (connectSocket == INVALID_SOCKET)
		{
			globalObjects->errorLog->WriteError("CClient::CreateSocket::socket:%s\n", globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());

			return;
		}

		if (CClient::ConnectSocket())
		{
			connectionState = CClient::CS_CONNECTING;

			break;
		}

		ptr = ptr->ai_next;
	}

	freeaddrinfo(result);

	gethostname(hostname, 132);

	globalObjects->errorLog->WriteError("CClient::CreateSocket::hostname:%s\n", hostname);
}

BYTE CClient::ConnectSocket()
{
	err = connect(connectSocket, ptr->ai_addr, (INT)ptr->ai_addrlen);

	if (err == SOCKET_ERROR)
	{
		closesocket(connectSocket);

		connectSocket = 0;

		globalObjects->errorLog->WriteError("CClient::ConnectSocket::Unable to connect to server\n");

		return FALSE;
	}

	return TRUE;
}

VOID CClient::SendEvent(CPacket* p)
{
	if (connectionState != CClient::CS_CONNECTED)
	{
		return;
	}

	totalBytes = send(connectSocket, (CHAR*)p, sizeof(CPacket), 0);

	if (totalBytes == 0)
	{
		globalObjects->errorLog->WriteError("CClient::SendEvent::send:Server Closing Connection\n");
	}
	else if (totalBytes == SOCKET_ERROR)
	{
		globalObjects->errorLog->WriteError("CClient::SendEvent::send:%s\n", globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());
	}
}

VOID CClient::Disconnect()
{
	if (connectionState != CClient::CS_CONNECTED)
	{
		return;
	}

	err = shutdown(connectSocket, SD_BOTH);

	if (err == SOCKET_ERROR)
	{
		globalObjects->errorLog->WriteError("CClient::Disconnect::shutdown:%s\n", globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());
	}

	connectionState = CClient::CS_NOTCONNECTED;

	closesocket(connectSocket);

	connectSocket = 0;

	WSACleanup();
}

VOID CClient::ClientReceive()
{
	totalBytes = recv(connectSocket, (CHAR*)packet, sizeof(CPacket), 0);

	if (totalBytes == 0)
	{
		globalObjects->errorLog->WriteError("CClient::ClientReceive::Server Closing Connection\n");

		connectionState = CClient::CS_NOTCONNECTED;
	}
	// WSAEINTR for some reason the hardware stopped the function call so skipping the error check 
	else if ((totalBytes == SOCKET_ERROR) && (WSAGetLastError() != WSAEINTR))
	{
		globalObjects->errorLog->WriteError("CClient::ClientReceive::recv:%s\n", globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());
	}
}