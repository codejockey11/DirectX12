#include "CFrametime.h"

CFrametime::CFrametime()
{
	memset(this, 0x00, sizeof(CFrametime));

	startTime = GetTickCount64();
	
	currTime = GetTickCount64();
}

CFrametime::~CFrametime()
{
}

void CFrametime::Frame()
{
	currTime = GetTickCount64();

	if ((currTime - countTime) > 1000)
	{
		countTime = currTime;

		rframecount = framecount;

		rframetime = frametime;

		framecount = 0;
	}

	counter++;

	framecount++;

	frametime = (float)((currTime - startTime) / 1000.0F);

	startTime = currTime;
}
