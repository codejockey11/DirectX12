#include "CServer.h"

unsigned __stdcall ServerAcceptThread(VOID* c)
{
	CServer* sc = (CServer*)c;
#ifdef _DEBUG
	sc->globalObjects->errorLog->WriteError("ServerAcceptThread Starting\n");
#endif
	while (sc->acceptRunning)
	{
		if (sc->listenSocket)
		{
			SOCKET tempSocket = accept(sc->listenSocket, NULL, NULL);

			// WSAEINTR for some reason the hardware stopped the function call so skipping the error check 
			if ((tempSocket == INVALID_SOCKET) && (WSAGetLastError() != WSAEINTR))
			{
#ifdef _DEBUG
				sc->globalObjects->errorLog->WriteError("CServer::ServerAcceptThread::accept:INVALID_SOCKET:%s\n", sc->globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());
#endif
				closesocket(tempSocket);

				tempSocket = 0;
			}
			else if ((tempSocket == SOCKET_ERROR) && (WSAGetLastError() != WSAEINTR))
			{
				sc->globalObjects->errorLog->WriteError("CServer::ServerAcceptThread::accept:SOCKET_ERROR:%s\n", sc->globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());
			}
			else
			{
				CClientSocket* cs = new CClientSocket(sc->client, tempSocket);

				sc->clients->Add(cs, cs->clientNbr);

				sc->client++;

				CPacket* packet = new CPacket(0, cs->clientNbr, CPacket::PT_SINGLE, CPacket::PT_ACCEPTED);

				sc->Send(packet);

				delete packet;
			}
		}
	}
#ifdef _DEBUG
	sc->globalObjects->errorLog->WriteError("ServerAcceptThread Ending\n");
#endif
	_endthreadex(0);

	return 0;
}

unsigned __stdcall ServerWorkerThread(VOID* c)
{
	CServer* sc = (CServer*)c;
#ifdef _DEBUG
	sc->globalObjects->errorLog->WriteError("ServerWorkerThread Starting\n");
#endif
	while (sc->serverWorkerRunning)
	{
		CLinkListNode* lln = sc->clients->list;

		while (lln->element)
		{
			CClientSocket* cs = (CClientSocket*)lln->element;

			if (lln->deleteMe)
			{
				lln = sc->clients->Delete(lln);
			}
			else if (cs->socket)
			{
				CPacket* packet = new CPacket();

				INT totalBytes = recv(cs->socket, (CHAR*)packet, sizeof(CPacket), 0);

				if (totalBytes > 0)
				{
					sc->ProcessEvent(packet);
				}
				else if (totalBytes == 0)
				{
#ifdef _DEBUG
					sc->globalObjects->errorLog->WriteError("CServer::ServerWorkerThread::Client Closing Connection:%i\n", cs->clientNbr);
#endif
					lln->deleteMe = TRUE;

					closesocket(cs->socket);

					cs->socket = 0;
				}
				// WSAEINTR for some reason the hardware stopped the function call so skipping the error check 
				else if ((totalBytes == SOCKET_ERROR) && (WSAGetLastError() != WSAEINTR))
				{
					sc->globalObjects->errorLog->WriteError("CServer::ServerWorkerThread::recv:%s\n", sc->globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());
				}

				delete packet;

				lln = lln->next;
			}
		}
	}
#ifdef _DEBUG
	sc->globalObjects->errorLog->WriteError("ServerWorkerThread Ending\n");
#endif
	_endthreadex(0);

	return 0;
}

CServer::CServer()
{
	memset(this, 0x00, sizeof(CServer));
}

CServer::CServer(CGlobalObjects* g, const CHAR* p)
{
	memset(this, 0x00, sizeof(CServer));

	globalObjects = g;

	clients = new CLinkList();

	CServer::InitializeWinsock();
	CServer::CreateListenSocket(p);
	CServer::BindListenSocket();
	CServer::StartListenSocket();

	client = 1;
}

CServer::~CServer()
{
	delete clients;
}

VOID CServer::InitializeWinsock()
{
	WORD wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		globalObjects->errorLog->WriteError("CServer::InitializeWinsock::WSAStartup:%i\n", err);

		return;
	}

	globalObjects->errorLog->WriteError("CServer::InitializeWinsock::WSAStartup:%s\n", wsaData.szDescription);
}

VOID CServer::CreateListenSocket(const CHAR* port)
{
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(NULL, port, &hints, &result);

	if (err != 0)
	{
		globalObjects->errorLog->WriteError("CServer::CreateListenSocket::getaddrinfo:%i\n", err);

		return;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (listenSocket == INVALID_SOCKET)
	{
		globalObjects->errorLog->WriteError("CServer::CreateListenSocket::socket:%s\n", globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());

		freeaddrinfo(result);
	}
}

VOID CServer::BindListenSocket()
{
	err = bind(listenSocket, result->ai_addr, (INT)result->ai_addrlen);

	if (err == SOCKET_ERROR)
	{
		globalObjects->errorLog->WriteError("CServer::BindListenSocket::bind:%s\n", globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());

		freeaddrinfo(result);

		closesocket(listenSocket);

		listenSocket = 0;
	}

	freeaddrinfo(result);
}

VOID CServer::StartListenSocket()
{
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		globalObjects->errorLog->WriteError("CServer::StartListenSocket::listen:%s\n", globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());

		closesocket(listenSocket);

		return;
	}

	acceptRunning = TRUE;
	serverWorkerRunning = TRUE;

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, &ServerAcceptThread, (VOID*)this, 0, &listenThreadId);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, &ServerWorkerThread, (VOID*)this, 0, &workerThreadId);
}

VOID CServer::Send(CPacket* p)
{
	switch (p->audience)
	{
		case CPacket::PT_SINGLE:
		{
			CLinkListNode* lln = clients->Search(p->toClient);

			if (p->type == CPacket::PT_PING)
			{
				p->toClient = p->fromClient;
				p->fromClient = 0;
				p->type = CPacket::PT_PONG;
			}

			CServer::SendNetwork(p, (CClientSocket*)lln->element);

			break;
		}

		case CPacket::PT_GLOBAL:
		{
			CLinkListNode* lln = clients->list;

			while (lln->element)
			{
				CServer::SendNetwork(p, (CClientSocket*)lln->element);

				lln = lln->next;
			}

			break;
		}
	}
}

VOID CServer::SendNetwork(CPacket* p, CClientSocket* cs)
{
	if (!acceptRunning)
	{
		return;
	}

	totalBytes = send(cs->socket, (CHAR*)p, sizeof(CPacket), 0);

	if (totalBytes == SOCKET_ERROR)
	{
		globalObjects->errorLog->WriteError("CServer::SendNetwork::send:%s\n", globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());
	}
}

VOID CServer::Shutdown()
{
	if (!acceptRunning)
	{
		return;
	}

	globalObjects->errorLog->WriteError("CServer::Shutdown::shutdown\n");

	acceptRunning = FALSE;
	serverWorkerRunning = FALSE;

	rv = WaitForSingleObjectEx(hThread[0], 2000, FALSE);

	CloseHandle(hThread[0]);

	rv = WaitForSingleObjectEx(hThread[1], 2000, FALSE);

	CloseHandle(hThread[1]);

	closesocket(listenSocket);

	listenSocket = 0;

	CLinkListNode* lln = clients->list;

	while (lln->element)
	{
		CClientSocket* cs = (CClientSocket*)lln->element;

		err = shutdown(cs->socket, SD_BOTH);

		if (err == SOCKET_ERROR)
		{
			globalObjects->errorLog->WriteError("CServer::Shutdown::shutdown:%s\n", globalObjects->errorLog->GetWinsockErrorMessage(WSAGetLastError())->name->GetText());
		}

		closesocket(cs->socket);

		cs->socket = 0;

		lln = lln->next;
	}

	WSACleanup();
}

VOID CServer::ProcessEvent(CPacket* p)
{
	if (p->type != CPacket::PT_DISCONNECT)
	{
		CServer::Send(p);

		return;
	}

	CLinkListNode* lln = clients->Search(p->fromClient);

	if (lln)
	{
		CClientSocket* cs = (CClientSocket*)lln->element;

		globalObjects->errorLog->WriteError("CServer::ProcessEvent::Client Closing Connection:%i\n", cs->clientNbr);

		closesocket(cs->socket);

		cs->socket = 0;

		lln->deleteMe = TRUE;
	}

}