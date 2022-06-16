#ifndef _CHudObject
#define _CHudObject

#include "standard.h"

#include "CGlobalObjects.h"
#include "CSprite.h"

class CHudObject
{
public:

	CHudObject();

	~CHudObject();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;
};
#endif