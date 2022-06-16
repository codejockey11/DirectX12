#ifndef _CORTHOPLAYER
#define _CORTHOPLAYER

#include "standard.h"

#include "CErrorLog.h"
#include "CGlobalObjects.h"
#include "COrthoLevel.h"
#include "CPacket.h"
#include "CSprite.h"

class COrthoPlayer
{
public:

	CVertex2 coordinates;

	CVertex2 position;

	CVertex2 direction;

	CString* name;

	CSprite* sprite;

	CVertex2 cellSize;

	CVertex2 scaleSize;

	COrthoPlayer();

	COrthoPlayer(CGlobalObjects* g, COrthoLevel* l, FLOAT r, CVertex2* cs, CVertex2* ss, CVertex2* coord, const CHAR* n);

	~COrthoPlayer();

	VOID PlayerMove(CPacket* p);

	BYTE CheckLevelCollision();

private:

	CGlobalObjects* globalObjects;

	COrthoLevel* level;

	FLOAT rate;

	FLOAT distance;
};
#endif