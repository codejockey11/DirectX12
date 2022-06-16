#ifndef _CORTHOLOCK
#define _CORTHOLOCK

#include "standard.h"

#include "CSprite.h"

class COrthoLock
{
public:

	CVertex2 coordinates;

	CVertex2 position;

	INT number;

	CSprite* sprite;

	COrthoLock();

	~COrthoLock();
};
#endif