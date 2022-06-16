#ifndef _CORTHOKEY
#define _CORTHOKEY

#include "standard.h"

#include "CSprite.h"

class COrthoKey
{
public:

	CVertex2 coordinates;
	
	CVertex2 position;

	INT number;

	CSprite* sprite;

	COrthoKey();

	~COrthoKey();
};
#endif