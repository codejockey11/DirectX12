#include "COrthoPlayer.h"

COrthoPlayer::COrthoPlayer()
{
	memset(this, 0x00, sizeof(COrthoPlayer));
}

COrthoPlayer::COrthoPlayer(CGlobalObjects* g, COrthoLevel* l, FLOAT r, CVertex2* cs, CVertex2* ss, CVertex2* coord, const CHAR* n)
{
	memset(this, 0x00, sizeof(COrthoPlayer));

	globalObjects = g;

	level = l;
	rate = r;

	cellSize = *cs;

	scaleSize.X = ss->X * (UINT)((FLOAT)globalObjects->window->width / (FLOAT)globalObjects->window->height);
	scaleSize.Y = ss->Y * (UINT)((FLOAT)globalObjects->window->width / (FLOAT)globalObjects->window->height);

	position = *coord;

	coordinates = *coord;

	name = new CString(n);

	CVertex2 v1(2048.0F, 1536.0F);

	CVertex2 v2(0.0F, 4.0F);

	sprite = new CSprite(globalObjects, globalObjects->textureMgr->GetTexture("dungeonTileset.png"), &v1, &cellSize, &v2, &scaleSize, &position);
}

COrthoPlayer::~COrthoPlayer()
{
	delete name;
	delete sprite;
}

VOID COrthoPlayer::PlayerMove(CPacket* p)
{
	direction.X = 0.0F;
	direction.Y = 0.0F;

	distance = rate * globalObjects->frametime->frametime;

	switch (p->type)
	{
	case CPacket::PT_MOVE_UP:
	{
		direction.Y = -1.0F;

		break;
	}

	case CPacket::PT_MOVE_DOWN:
	{
		direction.Y = 1.0F;

		break;
	}

	case CPacket::PT_MOVE_LEFT:
	{
		direction.X = -1.0F;

		break;
	}

	case CPacket::PT_MOVE_RIGHT:
	{
		direction.X = 1.0F;

		break;
	}
	}

	direction.RadiusNormalize();

	CVertex2 pp = position;

	position.X += direction.X * distance;
	position.Y += direction.Y * distance;

	sprite->position = position;
	sprite->UpdatePosition();

	if (COrthoPlayer::CheckLevelCollision())
	{
		position = pp;
	}
	else
	{
		coordinates.X += (direction.X * distance);
		coordinates.Y += (direction.Y * distance);
	}


	if (position.X < (sprite->scaleSize.X / 2.0F))
	{
		position.X = (sprite->scaleSize.X / 2.0F);
	}

	if (position.Y < (sprite->scaleSize.Y / 2.0F))
	{
		position.Y = (sprite->scaleSize.Y / 2.0F);
	}

	if (globalObjects->window->windowed)
	{
		if (position.X > globalObjects->window->width - (sprite->scaleSize.X / 2.0F))
		{
			position.X = globalObjects->window->width - (sprite->scaleSize.X / 2.0F);
		}

		if (position.Y > globalObjects->window->height - (sprite->scaleSize.Y / 2.0F) - globalObjects->window->returnFromFullscreen)
		{
			position.Y = globalObjects->window->height - (sprite->scaleSize.Y / 2.0F) - globalObjects->window->returnFromFullscreen;
		}
	}
	else
	{
		if (position.X > globalObjects->window->width - (sprite->scaleSize.X / 2.0F))
		{
			position.X = globalObjects->window->width - (sprite->scaleSize.X / 2.0F);
		}

		if (position.Y > globalObjects->window->height - (sprite->scaleSize.Y / 2.0F))
		{
			position.Y = globalObjects->window->height - (sprite->scaleSize.Y / 2.0F);
		}
	}

	sprite->position = position;
	sprite->UpdatePosition();
}

BYTE COrthoPlayer::CheckLevelCollision()
{
	for (UINT y = 0; y < level->height; y++)
	{
		for (UINT x = 0; x < level->width; x++)
		{
			COrthoTile* tile = (COrthoTile*)level->tiles->GetElement(2, x, y);

			if (tile)
			{
				if (tile->clip == COrthoTile::OT_CLIP)
				{
					CVertex v(sprite->box->min.X, sprite->box->min.Y, 0.0F);

					if (tile->sprite->box->CheckPointInBox(&v))
					{
						return TRUE;
					}

					v = CVertex(sprite->box->max.X, sprite->box->min.Y, 0.0F);

					if (tile->sprite->box->CheckPointInBox(&v))
					{
						return TRUE;
					}

					v = CVertex(sprite->box->min.X, sprite->box->max.Y, 0.0F);

					if (tile->sprite->box->CheckPointInBox(&v))
					{
						return TRUE;
					}

					v = CVertex(sprite->box->max.X, sprite->box->max.Y, 0.0F);

					if (tile->sprite->box->CheckPointInBox(&v))
					{
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}