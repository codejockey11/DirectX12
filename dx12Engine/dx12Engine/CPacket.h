#ifndef _CPacket
#define _CPacket

#include "standard.h"

class CPacket
{
public:

	enum
	{
		DATA_LENGTH = 32
	};

	int			fromClient;
	int			toClient;
	BYTE		audience;
	BYTE		type;

	int			length;

	BYTE		data[CPacket::DATA_LENGTH];

	CPacket();
	
	CPacket(int fc, int tc, BYTE a, BYTE t);
	CPacket(int fc, int tc, BYTE a, BYTE t, BYTE d, int size);
	CPacket(int fc, int tc, BYTE a, BYTE t, BYTE* d, int size);
	CPacket(int fc, int tc, BYTE a, BYTE t, const char* d, int size);

	~CPacket();

	void Clear();

	enum
	{
		PT_EMPTY = 0,

		PT_PLAYERORTHOENTER,
		PT_PLAYERORTHOEXIT,

		PT_PLAYERENTER,
		PT_PLAYEREXIT,

		PT_CONNECT,
		PT_DISCONNECT,

		PT_STARTSERVER,
		PT_STOPSERVER,
		PT_ACCEPTED,

		PT_PING,
		PT_PONG,

		PT_TOGGLECONSOLE,
		PT_CONSOLEPAGING,
		PT_CONSOLEINPUT,

		PT_LOADBRUSHMAP,
		PT_LOADTERRAIN,
		PT_LOADORTHOLEVEL,

		PT_CHAT,

		PT_SINGLE,
		PT_TEAM,
		PT_GLOBAL,
		PT_LOCAL,

		PT_MOVE_DOWN,
		PT_MOVE_UP,
		PT_MOVE_LEFT,
		PT_MOVE_RIGHT,

		PT_FORWARD,
		PT_BACKWARD,
		PT_STRAFELEFT,
		PT_STRAFERIGHT,

		PT_ATTACK,

		PT_RENDERFRAME,
		PT_WINDOWMODE,

		PT_FPSMOVE,

		PT_DEFAULT,

		PT_EXIT
	};
};
#endif