#ifndef _CFrametime
#define _CFrametime

#include "standard.h"

class CFrametime
{
public:

	float frametime;

	int rframecount;
	
	float rframetime;

	ULONGLONG currTime;

	UINT64 counter;

	CFrametime();
	
	~CFrametime();

	void Frame();

private:

	ULONGLONG startTime;
	
	ULONGLONG countTime;

	int framecount;
};
#endif