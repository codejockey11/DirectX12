#ifndef _CORTHOTILE
#define _CORTHOTILE

#include "standard.h"

#include "CSprite.h"
#include "CVertex.h"

class COrthoTile
{
public:

	BYTE type;

	BYTE clip;

	CVertex2 position;

	CSprite* sprite;

	COrthoTile();

	COrthoTile(BYTE t, BYTE c, CSprite* s);

	COrthoTile(COrthoTile* t, BYTE s);

	~COrthoTile();

	VOID Draw();

	VOID Copy(COrthoTile* t);

	enum
	{
		OT_NONE = 0,

		OT_CLIP,

		OT_REDSTART,
		OT_BLUESTART,

		OT_LOCK,
		OT_KEY,

		MAXORTHOTILETYPE = 0xFF
	};
};
#endif