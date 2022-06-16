#include "COrthoTile.h"

COrthoTile::COrthoTile()
{
	memset(this, 0x00, sizeof(COrthoTile));
}

COrthoTile::COrthoTile(BYTE t, BYTE c, CSprite* s)
{
	memset(this, 0x00, sizeof(COrthoTile));

	type = t;
	clip = c;
	sprite = s;
	position = s->position;
}

COrthoTile::COrthoTile(COrthoTile* t, BYTE s)
{
	memset(this, 0x00, sizeof(COrthoTile));

	type = t->type;
	clip = t->clip;
	position = t->sprite->position;

	if (s)
	{
		sprite = new CSprite(t->sprite);
	}
}

COrthoTile::~COrthoTile()
{
	delete sprite;
}

VOID COrthoTile::Draw()
{
	sprite->position = position;

	sprite->Draw();
}

VOID COrthoTile::Copy(COrthoTile* t)
{
	memset(this, 0x00, sizeof(COrthoTile));

	type = t->type;
	clip = t->clip;
	position = t->sprite->position;

	sprite = new CSprite(t->sprite);
}