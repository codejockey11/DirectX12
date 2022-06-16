#include "COrthoLevel.h"

COrthoLevel::COrthoLevel()
{
	memset(this, 0x00, sizeof(COrthoLevel));
}

COrthoLevel::COrthoLevel(CGlobalObjects* g, const CHAR* fn, CVertex2* cs, CVertex2* ss)
{
	memset(this, 0x00, sizeof(COrthoLevel));

	globalObjects = g;


	err = fopen_s(&fp, fn, "r");

	if (err)
	{
		globalObjects->errorLog->WriteError("COrthoLevel::COrthoLevel::Ortholevel not found:%s\n", fn);

		fp = 0;

		return;
	}

	br = fread_s(&sizeBuffer, 3 * sizeof(CHAR), sizeof(CHAR), 3, fp);

	width = atoi(sizeBuffer);

	br = fread_s(&sizeBuffer, 3 * sizeof(CHAR), sizeof(CHAR), 3, fp);

	height = atoi(sizeBuffer);

	br = fread_s(&readBuffer, sizeof(BYTE), sizeof(BYTE), 1, fp);



	dungeonTileset = globalObjects->textureMgr->GetTexture("dungeonTileset.png");

	redStart = new CLinkList();
	blueStart = new CLinkList();

	cellSize = *cs;

	scaleSize.X = ss->X * (UINT)((FLOAT)globalObjects->window->width / (FLOAT)globalObjects->window->height);
	scaleSize.Y = ss->Y * (UINT)((FLOAT)globalObjects->window->width / (FLOAT)globalObjects->window->height);

	position.X = (FLOAT)((globalObjects->window->width) - (scaleSize.X * width)) / 2;
	position.Y = (FLOAT)((globalObjects->window->height) - (scaleSize.Y * height)) / 2;

	tiles = new CHeapArray(sizeof(COrthoTile), TRUE, TRUE, 2, width, height);


	CVertex2 sz(2048.0F, 1536.0F);

	br = fread_s(&readBuffer, sizeof(BYTE), sizeof(BYTE), 1, fp);

	while (!feof(fp))
	{
		switch (readBuffer)
		{
			case ' ':
			{
				CVertex2 p(17.0F, 16.0F);

				sprite = new CSprite(globalObjects, dungeonTileset, &sz, &cellSize, &p, &scaleSize, &position);

				tile = new COrthoTile(COrthoTile::OT_NONE, FALSE, sprite);

				break;
			}

			case 'A':
			{
				CVertex2 p(17.0F, 17.0F);

				sprite = new CSprite(globalObjects, dungeonTileset, &sz, &cellSize, &p, &scaleSize, &position);

				tile = new COrthoTile(COrthoTile::OT_CLIP, TRUE, sprite);

				break;
			}

			case 'R':
			{
				CVertex2 p(4.0F, 16.0F);

				sprite = new CSprite(globalObjects, dungeonTileset, &sz, &cellSize, &p, &scaleSize, &position);

				tile = new COrthoTile(COrthoTile::OT_REDSTART, FALSE, sprite);

				break;
			}

			case 'B':
			{
				CVertex2 p(27.0F, 16.0F);

				sprite = new CSprite(globalObjects, dungeonTileset, &sz, &cellSize, &p, &scaleSize, &position);

				tile = new COrthoTile(COrthoTile::OT_BLUESTART, FALSE, sprite);

				break;
			}

			case 'L':
			{
				CVertex2 p(39.0F, 17.0F);

				sprite = new CSprite(globalObjects, dungeonTileset, &sz, &cellSize, &p, &scaleSize, &position);

				tile = new COrthoTile(COrthoTile::OT_LOCK, TRUE, sprite);

				break;
			}

			case 'K':
			{
				CVertex2 p(29.0F, 16.0F);

				sprite = new CSprite(globalObjects, dungeonTileset, &sz, &cellSize, &p, &scaleSize, &position);

				tile = new COrthoTile(COrthoTile::OT_KEY, TRUE, sprite);

				break;
			}

			case '\n':
			{
				sprite = NULL;
				tile = NULL;

				break;
			}

			default:
			{
				sprite = NULL;
				tile = NULL;

				break;
			}
		}

		if ((tile) && (sprite))
		{
			COrthoTile* t = (COrthoTile*)tiles->GetElement(2, wc, hc);

			t->Copy(tile);

			delete tile;


			switch (t->type)
			{
				case COrthoTile::OT_LOCK:
				{
					lock = new COrthoTile(t, FALSE);

					break;
				}

				case COrthoTile::OT_KEY:
				{
					key = new COrthoTile(t, FALSE);

					break;
				}

				case COrthoTile::OT_REDSTART:
				{
					tile = new COrthoTile(t, FALSE);

					redStart->Add(tile, NULL);

					break;
				}

				case COrthoTile::OT_BLUESTART:
				{
					tile = new COrthoTile(t, FALSE);

					blueStart->Add(tile, NULL);

					break;
				}
			}

			wc++;

			if (wc == width)
			{
				position.X = (FLOAT)((globalObjects->window->width) - (scaleSize.X * width)) / 2;

				position.Y += scaleSize.Y;

				wc = 0;

				hc++;
			}
			else
			{
				position.X += scaleSize.X;
			}
		}

		br = fread_s(&readBuffer, sizeof(BYTE), sizeof(BYTE), 1, fp);
	}

	fclose(fp);
}

COrthoLevel::~COrthoLevel()
{
	delete blueStart;
	delete redStart;
	delete tiles;
}

VOID COrthoLevel::Draw()
{
	for (UINT h = 0; h < height; h++)
	{
		for (UINT w = 0; w < width; w++)
		{
			COrthoTile* ot =  (COrthoTile*)tiles->GetElement(2, w, h);
			
			ot->Draw();
		}
	}
}