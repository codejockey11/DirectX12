#include "CPacket.h"

CPacket::CPacket()
{
	memset(this, 0x00, sizeof(CPacket));
}

CPacket::CPacket(int fc, int tc, BYTE a, BYTE t)
{
	memset(this, 0x00, sizeof(CPacket));

	fromClient = fc;
	toClient = tc;
	audience = a;
	type = t;
}

CPacket::CPacket(int fc, int tc, BYTE a, BYTE t, BYTE d, int size)
{
	memset(this, 0x00, sizeof(CPacket));

	fromClient = fc;
	toClient = tc;
	audience = a;
	type = t;

	length = size;

	data[0] = d;
}

CPacket::CPacket(int fc, int tc, BYTE a, BYTE t, BYTE* d, int size)
{
	memset(this, 0x00, sizeof(CPacket));

	fromClient = fc;
	toClient = tc;
	audience = a;
	type = t;

	length = size;

	if (length > CPacket::DATA_LENGTH)
	{
		length = CPacket::DATA_LENGTH;
	}

	memcpy(data, d, length);
}

CPacket::CPacket(int fc, int tc, BYTE a, BYTE t, const char* d, int size)
{
	memset(this, 0x00, sizeof(CPacket));

	fromClient = fc;
	toClient = tc;
	audience = a;
	type = t;

	length = size;

	if (length > CPacket::DATA_LENGTH)
	{
		length = CPacket::DATA_LENGTH;
	}

	memcpy(data, d, length);
}

CPacket::~CPacket()
{
}

void CPacket::Clear()
{
	memset(data, 0x00, CPacket::DATA_LENGTH);
}