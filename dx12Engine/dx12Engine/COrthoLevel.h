#ifndef _CORTHOLEVEL
#define _CORTHOLEVEL

#include "standard.h"

#include "CHeapArray.h"
#include "CLinkList.h"
#include "COrthoKey.h"
#include "COrthoLock.h"
#include "COrthoTile.h"
#include "CSprite.h"
#include "CTexture.h"

class COrthoLevel
{
public:

	UINT width;
	
	UINT height;

	CVertex2 cellSize;
	
	CVertex2 scaleSize;

	CLinkList* redStart;
	
	CLinkList* blueStart;

	CHeapArray* tiles;

	COrthoLevel();

	COrthoLevel(CGlobalObjects* g, const CHAR* fn, CVertex2* cs, CVertex2* ss);

	~COrthoLevel();

	VOID Draw();

private:

	CGlobalObjects* globalObjects;

	CTexture* dungeonTileset;

	FILE* fp;
	
	errno_t err;
	
	size_t br;

	BYTE readBuffer;
	
	CHAR sizeBuffer[3];

	CVertex2 position;

	UINT wc;
	
	UINT hc;

	COrthoTile* tile;

	CSprite* sprite;

	COrthoTile* lock;
	
	COrthoTile* key;
};
#endif