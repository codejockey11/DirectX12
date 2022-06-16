#ifndef _CKeyMap
#define _CKeyMap

#include "standard.h"

#include "CPacket.h"

class CKeyMap
{
public:

	UINT count;

	BYTE repeater;

	CPacket* packet;

	CKeyMap();

	~CKeyMap();
};
#endif