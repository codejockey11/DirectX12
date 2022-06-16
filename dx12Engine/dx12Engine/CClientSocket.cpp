#include "CClientSocket.h"

CClientSocket::CClientSocket()
{
	memset(this, 0x00, sizeof(CClientSocket));
}

CClientSocket::CClientSocket(UINT cn, SOCKET s)
{
	memset(this, 0x00, sizeof(CClientSocket));

	clientNbr = cn;
	socket = s;
}

CClientSocket::~CClientSocket()
{
}