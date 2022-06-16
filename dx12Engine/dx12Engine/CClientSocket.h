#ifndef _CClientSocket
#define _CClientSocket

#include "standard.h"

class CClientSocket
{
public:
		
	UINT clientNbr;
		
	SOCKET socket;

	CClientSocket();
	
	CClientSocket(UINT cn, SOCKET s);

	~CClientSocket();
};
#endif